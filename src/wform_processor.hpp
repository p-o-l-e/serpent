////////////////////////////////////////////////////////////////////////////////////////
// Waveform processor
// V.0.0.1 2022-02-04
////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#ifndef __WFORM_PROCESSOR
#define __WFORM_PROCESSOR
#include <cmath>
#include "delay.hpp"
#include "utility.hpp"
#include "reverb.hpp"
#include "chorus.hpp"

struct waveform_processor
{
    enum    CHANNEL {LEFT, RIGHT};
    limiter limit;
    reverb  rv;
    chorus  ch;
    float*  source[3];
    float*  amount;
    float*  pan[3];
    float   symmetry    = 1.0f;
    float   drive[3];
    float   gain[3];
    float   mix[3];
    int     mode        = 0;
    float   feed;

    waveform_processor();
    ~waveform_processor() {};
};

float fDistort(waveform_processor& o, int idx)
{
    return sinf(atanf(*o.source[idx] * (o.gain[idx]+0.02f)*50.0f)*(o.drive[idx]*0.5f+1.0f)) ;
}

float fSaturate(waveform_processor& o, int idx)
{
    return sinf(tanhf(*o.source[idx] * (o.gain[idx]+0.02f)*50.0f)*(o.drive[idx]*1.5f+1.0f)) ;
}

float fReverb(waveform_processor& o, int idx)
{
    o.rv.init();
    return o.rv.process(*o.source[idx]) ;
}

float fChorus(waveform_processor& o, int idx)
{
    o.ch.rate   = o.gain[idx];
    o.ch.level  = 1.0f;
    o.ch.morph  = o.drive[idx];
    return  xfade(o.ch.process(*o.source[idx]), *o.source[idx], o.mix[idx]);
}

float (*fFx[])(waveform_processor&, int) = { fSaturate, fDistort, fReverb, fChorus };
 
waveform_processor::waveform_processor()
{
    amount         = nullptr;
    source[0]      = nullptr;
    source[1]      = nullptr;
    source[2]      = nullptr;
}


float fMix(waveform_processor* o, const waveform_processor::CHANNEL& c)
{
    //*o->source[0] = fFx[0](*o, 0)*o->mix[0] + *o->source[0]*(1.0f - o->mix[0]);
    *o->source[0] = fFx[2](*o, 0);
    *o->source[0] = fFx[3](*o, 0);

    if(c == waveform_processor::CHANNEL::LEFT)
    {
        o->feed = (*o->source[0]*(1.0f-*o->pan[0]) + *o->source[1]*(1.0f-*o->pan[1]) + *o->source[2]*(1.0f-*o->pan[2]))**o->amount;
    }
    if(c == waveform_processor::CHANNEL::RIGHT)
    {
        o->feed = (*o->source[0]**o->pan[0] + *o->source[1]**o->pan[1] + *o->source[2]**o->pan[2])**o->amount;
    }
    return o->limit.process(o->feed);
}

// Symmetry in range: -1.0:1.0
// Input range      :  0.0:1.0
// Correction       :  Input*2-1
float fWProcess(waveform_processor* o, const waveform_processor::CHANNEL& c)
{
    float symmetry = o->symmetry*2.0f-1.0f;
    o->feed = ((*o->source[0] * (symmetry + *o->source[1])) + (*o->source[2] * (symmetry-*o->source[1]))) ** o->amount;
    return o->feed;
}


float (*fProcess[])(waveform_processor*, const waveform_processor::CHANNEL&) = {fMix, fWProcess};



#endif