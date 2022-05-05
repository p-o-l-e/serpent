////////////////////////////////////////////////////////////////////////////////////////
// VCO - Oscillator
// V.0.3.5 2022-02-17
////////////////////////////////////////////////////////////////////////////////////////
#ifndef __VCO
#define __VCO

#include <cmath>
#include "serpent.hpp"
#include "constants.hpp"
#include "iospecs.hpp"
#include "wavering.hpp"
#include "envelopes.hpp"
#include "filters.hpp"
#include "chaos.hpp"
#include "fastmath.hpp"
#include "utility.hpp"
iospecs settings;
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////


class VCO
{
    public:
        envelope_adsr env;
        int     nharm;                // Number of harmonics
        float   sample_rate;          // System sample rate
        float   phase;                // Current phase
        float   delta;                // Phase increment
        float   frequency;            // Frequency in Hz
        float*  amplitude;            // Overall amplitude
        float   feedback;             // Feedback
        float   eax, ebx;             // Feedback memory
        int     cax, cbx;             // Noise memory
        float*  phasecv;              // Contol value for phase
        float*  pwmcv;                // Control value for pwm
        float*  fcv;                  // Control value for frequency
        float   pshift;               // Pitch shift range
        float   out;
        bool    on;
        inline void init();
        VCO();
       ~VCO() {};
}; 

inline void VCO::init()
{ 
        delta = frequency * TAO / sample_rate; 
        pshift = (frequency*CHROMATIC_RATIO-frequency/CHROMATIC_RATIO)*TAO/sample_rate;
}

VCO::VCO()
{
        sample_rate = settings.sample_rate;
        phase       = 0.0f;
        delta       = 0.0f;  
        nharm       = 11;          
        on          = false;
        cax         = 0xFFFFFFFF;
        cbx         = 0xAAAAAAAA;
}

inline void fSine(VCO* o)
{       
        o->out = sinf(o->phase+(*o->phasecv-0.5f)*TAO)**o->amplitude;
        o->phase += o->delta + (*o->fcv-0.5f)*o->pshift*2.0f;
        if(o->phase >= TAO) o->phase -= TAO;
}

inline void fSquare(VCO* o)
{
        float pw = (*o->pwmcv*1.9f+0.05f)*M_PI;
        float ax = 0, bx = 0;
        for(int i = 1; i<= o->nharm; i++)
        {
                ax+=sinf((o->phase+(*o->phasecv-0.5f)*TAO)*i)/i;
                bx+=sinf((o->phase+(*o->phasecv-0.5f)*TAO+pw)*i)/i;
        }
        o->out = (ax-bx)**o->amplitude;
        o->phase += o->delta + (*o->fcv-0.5f)*o->pshift*2.0f;
        if(o->phase >= TAO) o->phase -= TAO;
}

inline void fSawtooth(VCO* o) 
{
        o->out = 0;
        for(int i = 1; i<= o->nharm; i++)
        {
                o->out+=sinf((o->phase+(*o->phasecv-0.5f)*TAO)*i)/i;
        }
        o->out*=*o->amplitude;
        o->phase += o->delta + (*o->fcv-0.5f)*o->pshift*2.0f;
        if(o->phase >= TAO) o->phase -= TAO;
}

inline void fRamp(VCO* o) 
{
        o->out = 0;
        for(int i = 1; i<= o->nharm; i++)
        {
                o->out+=sinf((o->phase+(*o->phasecv-0.5f)*TAO)*i+M_PI)/i;
        }
        o->out*=*o->amplitude;
        o->phase += o->delta + (*o->fcv-0.5f)*o->pshift*2.0f;
        if(o->phase >= TAO) o->phase -= TAO;
}

inline void fTriangle(VCO* o)
{
        float rise = *o->pwmcv * TAO;
        float fall = TAO - rise;
        float rise_delta = (rise != 0) ? (2.0 * *o->amplitude / rise) : 0;
        float fall_delta = (fall != 0) ? (2.0 * *o->amplitude / fall) : 0;

        if (o->phase > TAO) o->phase -= TAO;
        if (o->phase < rise) o->out = -*o->amplitude + (o->phase)* rise_delta;
        else o->out = *o->amplitude - (o->phase - rise) * fall_delta;
        o->phase += o->delta + (*o->fcv-0.5f)*o->pshift*2.0f;
}

inline void fTomisawa(VCO* o)
{
        o->feedback = 1.0f;                  
        o->feedback *= 1.0f * (1 - 0.0001f*o->frequency); 
        if(o->feedback<0) o->feedback = 0;

        o->phase += o->delta + (*o->fcv-0.5f)*o->pshift*2.0f;                
        if(o->phase>=M_PI) o->phase-=TAO;             

        float oa = cosf(o->phase+(*o->phasecv-0.5f)*TAO+o->feedback*o->eax); 
        o->eax = 0.5f*(oa+o->eax);        

        float ob = cosf(o->phase+(*o->phasecv-0.5f)*TAO+o->feedback*o->ebx+(*o->pwmcv * 1.9f + 0.05f)*M_PI); 
        o->ebx = 0.5f*(ob+o->ebx);            
        o->out = (oa-ob) * *o->amplitude;
}

inline void fGinger(VCO* o)     // Add warp
{
        static gingerbreadman g;
        g.x =  o->phase;
        g.y*= (*o->pwmcv-0.5)*2;
        g.iterate();
        o->out = (g.x - 2.5f)**o->amplitude;

        o->phase += o->delta + (*o->fcv-0.5f)*o->pshift*2.0f;
        if(o->phase >= M_PI) o->phase -= TAO;
}

inline void fIkeda(VCO* o)     // *
{
        static ikeda i;
        i.x =  o->phase;
        i.u = (0.979999f +*o->pwmcv/50.0f);
        
        i.iterate();
        o->out = (i.y + i.x * *o->phasecv)**o->amplitude;

        o->phase += o->delta + (*o->fcv-0.5f)*o->pshift*2.0f;
        if(o->phase >= M_PI) o->phase -= TAO;
}


inline void fDuffing(VCO* o)     // *
{
        static duffing d;
        d.y =  o->phase + (*o->phasecv-0.5f)*2;
        //d.t = o->delta/10;
        d.a = *o->pwmcv*16.0f;
        d.iterate();
        o->out = (d.y*0.1f)**o->amplitude;

        o->phase += o->delta + (*o->fcv-0.5f)*o->pshift*2.0f;
        if(o->phase >= M_PI) o->phase -= TAO;
}



// inline void fFabrikant(VCO* o)     
// {
//         static rabinovich_fabrikant d;
//         static dcb f;
//         d.x =  o->phase;
//         d.gamma = (0.979999f +*o->phasecv/50.0f);
//         d.x *= (1+*o->pwmcv);
//         //d.gamma = 0.999999;
//         //d.alpha = (*o->pwmcv-0.5)*8;
//         d.iterate();
//         o->out = f.process((d.x+d.y+d.z)*0.002f**o->amplitude*(1.01-*o->pwmcv)*(1.2-*o->phasecv));

//         o->phase += o->delta + (*o->fcv-0.5f)*o->pshift*2.0f;
//         if(o->phase >= M_PI) o->phase -= TAO;
// }

inline void fFabrikant(VCO* o)     
{
        static rabinovich_fabrikant d;
        
        d.t = o->delta/10.0f;
        //d.f = *o->phasecv*2;
        //d.a = 10.0f + *o->phasecv*2;
        d.iterate();
        
        o->out = ( xfade(d.y, d.x, *o->pwmcv ))*2.0f**o->amplitude;
        o->phase += o->delta + (*o->fcv-0.5f)*o->pshift*2.0f;
        if(o->phase >= M_PI) o->phase -= TAO;
}


// inline void fVanDerPol(VCO* o)//? Uncertainity
// {
//         static vanderpol d;
//         d.y =  o->phase;
//         //d.x = o->phase;

//         d.mu = (*o->pwmcv-0.5f)*4.0f;
//         //d.x  *= *o->phasecv*2;
//         d.f  = (*o->phasecv-0.5f)*2.0f; 
//         d.delta = o->delta;
//         d.iterate();
//         o->out = (d.y)*0.2f**o->amplitude;

//         o->phase += o->delta + (*o->fcv-0.5f)*o->pshift*2.0f;
//         if(o->phase >= M_PI) o->phase -= TAO;
// }

inline void fSprottST(VCO* o)//? Uncertainity
{
        static sprott_st d;

        d.t = o->delta/2.0f;
        d.iterate();
        
        o->out = ( xfade(d.z, d.y, *o->pwmcv ))*1.0f**o->amplitude;

        o->phase += o->delta + (*o->fcv-0.5f)*o->pshift*2.0f;
        if(o->phase >= M_PI) o->phase -= TAO;
}

inline void fHelmholz(VCO* o)//? Uncertainity
{
        static helmholz d;

        d.t = o->delta/2.0f;
        d.iterate();
        
        o->out = ( xfade(d.y, d.x, *o->pwmcv ))*1.5f**o->amplitude;

        o->phase += o->delta + (*o->fcv-0.5f)*o->pshift*2.0f;
        if(o->phase >= M_PI) o->phase -= TAO;
}

inline void fHalvorsen(VCO* o)//? Uncertainity
{
        static halvorsen d;

        d.t = o->delta/30.0f;
        d.iterate();
        
        o->out = ( xfade(d.y, d.z, *o->pwmcv ))*1.2f**o->amplitude;

        o->phase += o->delta + (*o->fcv-0.5f)*o->pshift*2.0f;
        if(o->phase >= M_PI) o->phase -= TAO;
}

inline void fTSUCS(VCO* o)//? Uncertainity
{
        static tsucs d;

        d.t = o->delta/40.0f;
        d.iterate();
        
        o->out = ( xfade(d.y, d.z, *o->pwmcv ))*1.8f**o->amplitude;

        o->phase += o->delta + (*o->fcv-0.5f)*o->pshift*2.0f;
        if(o->phase >= M_PI) o->phase -= TAO;
}



inline void fLinz(VCO* o)// OK
{
        static linz d;

        d.t = o->delta;
        d.a = 0.01f + *o->phasecv/2.0f;
        d.iterate();
        
        o->out = ( xfade(d.y, d.z, *o->pwmcv ))*2.0f**o->amplitude;
        o->phase += o->delta + (*o->fcv-0.5f)*o->pshift*2.0f;
        if(o->phase >= M_PI) o->phase -= TAO;

}

inline void fYuWang(VCO* o)// OK
{
        static yu_wang d;

        d.t = o->delta/40;
        d.a = 10.0f + *o->phasecv*2;
        d.iterate();
        
        o->out = ( xfade(d.y, d.x, *o->pwmcv ))*2.0f**o->amplitude;
        o->phase += o->delta + (*o->fcv-0.5f)*o->pshift*2.0f;
        if(o->phase >= M_PI) o->phase -= TAO;
}


inline void fVanDerPol(VCO* o)// OK
{
        static vanderpol d;

        d.t = o->delta/3.0f;
        d.f = *o->phasecv*2;
        //d.a = 10.0f + *o->phasecv*2;
        d.iterate();
        
        o->out = ( xfade(d.y, d.x, *o->pwmcv ))*2.0f**o->amplitude;
        o->phase += o->delta + (*o->fcv-0.5f)*o->pshift*2.0f;
        if(o->phase >= M_PI) o->phase -= TAO;
}



inline void fNoise(VCO* o)     
{
        o->cax ^= o->cbx;
        o->out  = o->cbx * *o->amplitude * 4.6566e-10;
        o->cbx += o->cax;
}


void (*form[])(VCO*) = {    fSine ,             // 0
                            fRamp,              // 1
                            fSawtooth,          // 2
                            fSquare,            // 3
                            fTomisawa,          // 4
                            fTriangle,          // 5
                            fGinger,            // 6
                            fIkeda,             // 7
                            fDuffing,           // 8
                            fFabrikant,         // 9
                            fVanDerPol,         // 10
                            fNoise              // 11
                        };

#endif /////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
