////////////////////////////////////////////////////////////////////////////////////////
// Delay
// V.0.0.1 2022-02-04
////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#ifndef __DELAY
#define __DELAY
#include "wavering.hpp"
#include "utility.hpp"
#include "filters.hpp"
#include "iospecs.hpp"


class delay
{
    public:
        float       *data;
        const int   length   = 65536;
        float       tmax     = length/2;
        int         sample   = 0;    // Sample DD->sample
        float       feedback = 0.5f; // 0.0f is endless; 1.0f is one tick
        float       amount   = 0.5f;
        float*      time;
        float*      tone;
        float       eax      = 0.0f;
        float       ebx      = 0.0f;
        int         type;

        int         iPtr,  oPtr;
        float       alpha, beta;
        float       feed;

        void clr();
        delay();
        delay(const int&);
        ~delay();
};

delay::delay()
{
    data = new float[length];
    for(int i=0;i<length;i++) data[i]=0.0f;
}

delay::delay(const int& l): length(l)
{
    data = new float[length];
    tmax = length/2;
    for(int i=0;i<length;i++) data[i]=0.0f;
}

void delay::clr() 
{
    for (uint i = 0; i < length; i++) 
    {
        data[i] = 0.0f;
    }
    feed    = 0.0;
    iPtr    = 0;
    eax     = 0.0;
}

delay::~delay()
{
    delete data;
}









float fDelay(delay* DD, const float& input)
{
    if (DD->sample >= DD->length) DD->sample = 0;
    int f = DD->sample - (*DD->time * DD->tmax);
    if (f<0) f += DD->length;
    float out = DD->data[DD->sample] = input + (DD->data[f] * DD->feedback);
    DD->sample++;
    return xfade(out, input, DD->amount);
}

float fFDelay(delay* DD, const float& input)
{
    static onepole o; 
    if (DD->sample >= DD->length) DD->sample = 0;
    int f = DD->sample - (*DD->time * DD->tmax);
    if (f<0) f += DD->length;
    float eax = o.process(input);
    float out = DD->data[DD->sample] = eax + (DD->data[f] * DD->feedback);
    DD->sample++;
    DD->eax = eax;
    return xfade(out, input, DD->amount);
}

float fSDelay(delay* DD, const float& input)
{
    static onepole o; 
    static hp hpf; hpf.cutoff = *DD->tone * 0.25f + 0.22f;
    if (DD->sample >= DD->length) DD->sample = 0;
    int f = DD->sample - (*DD->time * DD->tmax);
    if (f<0) f += DD->length;
    float eax = o.process(input);
    eax = hpf.process(input);
    
    float out = DD->data[DD->sample] = eax + (DD->data[f] * DD->feedback);
    DD->sample++;
    DD->eax = eax;
    return xfade(out, input, DD->amount);
}

// STK allpass interpolating delay line.
// This method implements a fractional-length digital delay-line using
// a first-order allpass filter.
// An allpass filter has unity magnitude gain but variable phase
// delay properties, making it useful in achieving fractional delays
// without affecting a signal's frequency magnitude response.  In
// order to achieve a maximally flat phase delay response, the
// minimum delay possible in this implementation is limited to a
// value of 0.5.
// Based on code by Perry R. Cook and Gary P. Scavone, 1995--2021.

float fADelay(delay* DD, const float& input)
{
    if(DD->eax != *DD->time)
    {
        DD->eax = *DD->time;
        float outPointer = DD->iPtr - DD->eax;              // read chases write
        while (outPointer < 0) outPointer += DD->length;    // modulo maximum length
        DD->oPtr = (int) outPointer;                        // integer part
        if (DD->oPtr == DD->length) DD->oPtr = 0;
        DD->alpha = outPointer - DD->oPtr;                  // fractional part
        DD->beta = (float) 1.0 - DD->alpha;
    }

    DD->data[DD->iPtr++] = input * DD->feedback;
    if (DD->iPtr == DD->length) DD->iPtr = 0;
    // First 1/2 of interpolation
    float out = DD->data[DD->oPtr] * DD->beta;
    // Second 1/2 of interpolation
    if (DD->oPtr + 1 < DD->length) out += DD->data[DD->oPtr+1] * DD->alpha;
    else out += DD->data[0] * DD->alpha;
    if (++DD->oPtr == DD->length) DD->oPtr = 0;
    return xfade(out, input, DD->amount);
}


// STK linear interpolating delay line
// This method implements a fractional-length digital delay-line using
// first-order linear interpolation.
// Linear interpolation is an efficient technique for achieving
// fractional delay lengths, though it does introduce high-frequency
// signal attenuation to varying degrees depending on the fractional
// delay setting.  The use of higher order Lagrange interpolators can
// typically improve (minimize) this attenuation characteristic.
// Based on code by Perry R. Cook and Gary P. Scavone, 1995--2021.

float fLDelay(delay* DD, const float& input)
{
    if(DD->ebx != *DD->time)
    {
        DD->ebx = *DD->time;
        float outPointer = DD->iPtr - DD->ebx*2 + 1.0;        // outPoint chases inpoint

        while (outPointer < 0) outPointer += DD->length;    // modulo maximum length
        DD->oPtr = (int) outPointer;                        // integer part
        if (DD->oPtr == DD->length) DD->oPtr = 0;
        DD->alpha = 1.0 + DD->oPtr - outPointer;            // fractional part
        if (DD->alpha < 0.5) 
        {
            // The optimal range for alpha is about 0.5 - 1.5 in order to
            // achieve the flattest phase delay response.
            DD->oPtr += 1;
            if (DD->oPtr >= DD->length) DD->oPtr -= DD->length;
            DD->alpha += 1.0f;
        }
        DD->beta = (1.0 - DD->alpha) / (1.0 + DD->alpha);   // coefficient for allpass
    }

    DD->data[DD->iPtr++] = input * DD->feedback;
    if (DD->iPtr == DD->length) DD->iPtr = 0;
    // Do allpass interpolation delay.
    DD->feed *= -DD->beta;
    DD->feed += DD->eax + (DD->beta * DD->data[DD->oPtr]);
    // Save the allpass input and increment modulo length.
    DD->eax = DD->data[DD->oPtr++];
    if (DD->oPtr == DD->length) DD->oPtr = 0;
    return xfade(DD->feed, input, DD->amount);
}




// STK non-interpolating delay line.
// This method implements a non-interpolating digital delay-line.
// A non-interpolating delay line is typically used in fixed
// delay-length applications, such as for reverberation.
// Based on code by Perry R. Cook and Gary P. Scavone, 1995--2021.

float fNDelay(delay* DD, const float& input)
{
    if(DD->eax != *DD->time)
    {
        DD->eax = *DD->time;
        if (DD->iPtr >= *DD->time) DD->oPtr = DD->iPtr - *DD->time;
        else DD->oPtr = DD->length + DD->iPtr - *DD->time;
    }

    DD->data[DD->iPtr++] = input * DD->feedback;
    if (DD->iPtr == DD->length) DD->iPtr = 0;
    DD->feed = DD->data[DD->oPtr++];
    if (DD->oPtr == DD->length) DD->oPtr = 0;
    return xfade(DD->feed, input, DD->amount);
}


float (*fDD[])(delay*, const float&) = {
                                        fDelay,  // 0
                                        fFDelay, // 1
                                        fSDelay, // 2
                                        fADelay, // 3
                                        fLDelay, // 4
                                        fNDelay  // 5   
                                        };







#endif //////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

