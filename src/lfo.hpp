////////////////////////////////////////////////////////////////////////////////////////
// LFO
// V.0.3.8 2022-02-21
////////////////////////////////////////////////////////////////////////////////////////
#pragma once
#include <cmath>
#include "constants.hpp"
#include "iospecs.hpp"
#include "wavering.hpp"
#include "envelopes.hpp"
#include "filters.hpp"
#include "utility.hpp"
#include "chaos.hpp"

////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////

class LFO
{
    public:
        float   sample_rate;        // System sample rate
        float*  amplitude;          // Amplitude control
        float   phase     = 0.0f;   // Current phase
        float   delta     = 0.0f;   // Phase increment speed
        float   frequency = 2.0f;   // Frequency in Hz
        float   phasecv   = 0.0f;   // Phase control value
        float   ampcv     = 1.0f;   // Control value for amplitude
        float   out;                // Output
        float*  morph;
        int     form      = 0;      // Lfo form id
        bool    on        = false;  // On/Off Switch
        inline void init(bool);
        LFO();
       ~LFO(){};
}; 

inline void LFO::init(bool retrigger)
{ 
        if(retrigger)
        {
            phase = 0.0f;
        }
        delta = frequency * TAO / sample_rate; 
}

inline void lfoSine(LFO* o)
{
        o->phase += o->phasecv;
        o->out = sinf(tanf(sinf(o->phase) * (*o->morph-0.5f)*3.0f))**o->amplitude*o->ampcv;
        o->phase += o->delta;
        if(o->phase >= TAO)
        o->phase -= TAO;
}

inline void lfoMorphSine(LFO* o)
{
        o->phase += o->phasecv;
        o->out = sinf(sinf(o->phase) * (*o->morph-0.5f)*TAO)**o->amplitude*o->ampcv;
        o->phase += o->delta;
        if(o->phase >= TAO)
        o->phase -= TAO;
}

inline void lfoSaw(LFO* o) 
{
        o->phase += o->phasecv;
        o->out = ( 1.0-2.0*o->phase*(ITAO))**o->amplitude*o->ampcv;
        o->phase += o->delta;
        if(o->phase >= TAO)
        o->phase -= TAO;   
        if(o->phase < 0.0f)
        o->phase += TAO;
}

inline void lfoTriangle(LFO* o)
{
        o->phase += o->phasecv;
        o->out = tan(sin(o->phase))**o->amplitude*o->ampcv*0.65f;
        o->phase += o->delta;
        if(o->phase >= TAO)
        o->phase -= TAO;
}

inline void lfoMorphTri(LFO* o)
{
        float rise = *o->morph * TAO;
        float fall = TAO - rise;
        float rise_delta = (rise != 0) ? (2.0 * *o->amplitude / rise) : 0;
        float fall_delta = (fall != 0) ? (2.0 * *o->amplitude / fall) : 0;

        if (o->phase > TAO) o->phase -= TAO;
        if (o->phase < rise) o->out = -*o->amplitude + o->phase * rise_delta;
        else o->out = *o->amplitude - (o->phase - rise) * fall_delta;
        o->phase += o->delta;
}


inline void lfoLorenzX(LFO* o)
{
        //o->laf.a = o->delta*32 + 4.3;
        static dcb d; d.f = 0.82;
        static lorenz l;
        l.a = o->delta*16 + 8;
        //l.delta*= o->delta;
        l.t = *o->morph * *o->morph * *o->morph/200.0f;
        l.iterate();
        o->out = d.process(l.x * 0.2f);
}



inline void lfoIkeda(LFO* o)
{
        static ikeda i;
        static dcb d; d.f = 0.82;
        i.x =  o->phase;
        i.u = (0.979999f +*o->morph/50.0f);
        
        i.iterate();
        o->out = d.process((i.y + i.x * *o->morph))**o->amplitude*0.2;

        o->phase += o->delta;
        if(o->phase >= M_PI) o->phase -= TAO;
}



inline void lfoRoesslerX(LFO* o)
{
        static roessler rsl;
        //o->laf.a = o->delta*32 + 4.3;
        rsl.delta = o->delta/32;
        rsl.c = *o->morph * 7.7f + 1.0;
        rsl.iterate();

        o->out = rsl.x * 0.05;
}


LFO::LFO(): sample_rate(settings.sample_rate) {};

void (*form_lfo[])(LFO*) = {    lfoSine ,        // 0
                                lfoSaw,          // 1
                                lfoMorphTri,
                                lfoTriangle,      // 2
                                lfoLorenzX,
                                lfoIkeda,
                                lfoRoesslerX
                            };


vector<float> imprint(LFO* l, int width, int step)
{
    l->sample_rate= width/2;
    l->init(true);
    vector<float> r(width,0);
        int i = 0;
        while (i<width)
        {
            form_lfo[l->form](l);
            r[i] = l->out;
            i++; 
        }               
    return r;
}

////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////