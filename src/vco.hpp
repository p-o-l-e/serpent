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
#include "utility.hpp"
iospecs settings;
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////


class VCO
{
    public:
        envelope_adsr env_amp;
        envelope_adsr env_mod;
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
        cax = 0xFFFFFFFF;
        cbx = 0xAAAAAAAA;
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
        d.a = *o->pwmcv*16.0f;
        d.iterate();
        o->out = (d.y*0.1f)**o->amplitude;

        o->phase += o->delta + (*o->fcv-0.5f)*o->pshift*2.0f;
        if(o->phase >= M_PI) o->phase -= TAO;
}



inline void fFabrikant(VCO* o)     
{
        static rabinovich_fabrikant d;
        static dcb f;
        d.x =  o->phase;
        d.gamma = (0.979999f +*o->phasecv/50.0f);
        d.x *= (1+*o->pwmcv);
        //d.gamma = 0.999999;
        //d.alpha = (*o->pwmcv-0.5)*8;
        d.iterate();
        o->out = f.process((d.x+d.y+d.z)*0.002f**o->amplitude*(1.01-*o->pwmcv)*(1.2-*o->phasecv));

        o->phase += o->delta + (*o->fcv-0.5f)*o->pshift*2.0f;
        if(o->phase >= M_PI) o->phase -= TAO;
}

inline void fVanDerPol(VCO* o)//? Uncertainity
{
        static vanderpol d;
        d.y =  o->phase;
        //d.x = o->phase;

        d.mu = (*o->pwmcv-0.5f)*4.0f;
        //d.x  *= *o->phasecv*2;
        d.f  = (*o->phasecv-0.5f)*2.0f; 
        d.delta = o->delta;
        d.iterate();
        o->out = (d.y)*0.2f**o->amplitude;

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

// inline void fSquare(VCO* o) // Odd harmonics pulse
// {
//         float pw = (*o->pwmcv*1.9f+0.05f)*M_PI;
//         float ax = 0, bx = 0;
//         for(int i = 1; i<= o->nharm*2+1; i+=2)
//         {
//                 ax+=sinf(o->phase*i)/i;
//                 bx+=sinf((o->phase+pw)*i)/i;
//         }
//         o->out = (ax-bx)**o->amplitude*o->ampcv;
//         o->phase += o->delta;
//         if(o->phase >= TAO) o->phase -= TAO;
// }

// inline void fVanDerPol(VCO* o)     
// {
//         o->out = (*o->pwmcv-0.55f)*2.0f *(1.0f - o->phase*o->phase)*o->eax - o->phase;
//         o->out *= *o->amplitude;
//         o->out /= TAO;
//         o->eax = o->out;
//         o->phase += o->delta + (*o->fcv-0.5f)*o->pshift*2.0f;
//         if(o->phase >= M_PI) o->phase -= TAO;
// }


// roessler rs;
// inline void fVanDerPol(VCO* o)     
// {
//         rs.a = *o->pwmcv + 0.2; // +0.17
//         rs.b = *o->phasecv + 0.2; // + 10.2 (increasing upper bound of mA)
//         rs.delta = o->delta;
//         rs.iterate();
//         o->out = rs.x*0.1;
// }

// vanderpol vdp;

// inline void fVanDerPol(VCO* o)     
// {
//         vdp.delta = o->delta;
//         vdp.x = o->phase;
        
//         vdp.mu = *o->pwmcv*4;
//         vdp();
//         o->out = (vdp.y) *0.001;
//         //o->out = vdp.x;

//         o->phase += o->delta + (*o->fcv-0.5f)*o->pshift*2.0f;
//         if(o->phase >= M_PI) o->phase -= TAO;
// }
