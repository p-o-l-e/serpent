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
        int         type;

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

delay::~delay()
{
    delete data;
}









float fDelay(delay* DD, const float& input)
{
    if (DD->sample >= DD->length) DD->sample = 0;
    int f = DD->sample - (*DD->time * DD->tmax);
    if (f<0) f += DD->length;
    float out = DD->data[DD->sample] = input + (DD->data[f] * DD->feedback * DD->amount);
    out+=(-input)*DD->amount;
    DD->sample++;
    return out;
}

float fFDelay(delay* DD, const float& input)
{
    static onepole o; 
    if (DD->sample >= DD->length) DD->sample = 0;
    int f = DD->sample - (*DD->time * DD->tmax);
    if (f<0) f += DD->length;
    float eax = o.process(input);
    float out = DD->data[DD->sample] = eax + (DD->data[f] * DD->feedback * DD->amount);
    DD->sample++;
    DD->eax = eax;
    return out;
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
    
    float out = DD->data[DD->sample] = eax + (DD->data[f] * DD->feedback * DD->amount);
    DD->sample++;
    DD->eax = eax;
    return out;
}


float fADelay(delay* DD, const float& input)
{

}


float (*fDD[])(delay*, const float&) = {fDelay, fFDelay, fSDelay, fADelay};

//////////////////////////////////////////////////////////////////////////////////////////////////////

#endif