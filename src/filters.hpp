////////////////////////////////////////////////////////////////////////////////////////
// Filters
// V.0.1.1 2022-01-30
////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#ifndef __FILTERS
#define __FILTERS
#include "iospecs.hpp"
#include "constants.hpp"
#include <cmath>
#define VT 0.312
//iospecs settings;

class VCF
{
    private:
        uint    sample_rate;
        // Lp
        float   V [4];
        float   dV[4];
        float   tV[4];
        
        float   x;
        float   g;
        float   drive = 1.1f;
        // Peak
        float a  [3];
        float out[3];

    
    public:

        float*  cutoff;
        float   Q;
        float   lp24(const float&);
        float   peak(const float&);
        VCF();
        ~VCF();
};




VCF::VCF(): Q(0.0f), sample_rate(settings.sample_rate)                      
{
        out[0]=0.0f;
        out[1]=0.0f;
        out[2]=0.0f;
}

VCF::~VCF()
{
}



float VCF::lp24(const float& input) 
{
        float dV0, dV1, dV2, dV3;

        float f = *cutoff * 9928.56f + 27.5f;
        x = (M_PI * f) / sample_rate;
        g = 4.0 * M_PI * VT * f * (1.0 - x) / (1.0 + x);

        dV0 = -g * (tanh((drive * input + Q * 4.0f * V[3]) / (2.0 * VT)) + tV[0]);
        V[0] += (dV0 + dV[0]) / (2.0 * sample_rate);
        dV[0] = dV0;
        tV[0] = tanh(V[0] / (2.0 * VT));
            
        dV1 = g * (tV[0] - tV[1]);
        V[1] += (dV1 + dV[1]) / (2.0 * sample_rate);
        dV[1] = dV1;
        tV[1] = tanh(V[1] / (2.0 * VT));
            
        dV2 = g * (tV[1] - tV[2]);
        V[2] += (dV2 + dV[2]) / (2.0 * sample_rate);
        dV[2] = dV2;
        tV[2] = tanh(V[2] / (2.0 * VT));
            
        dV3 = g * (tV[2] - tV[3]);
        V[3] += (dV3 + dV[3]) / (2.0 * sample_rate);
        dV[3] = dV3;
        tV[3] = tanh(V[3] / (2.0 * VT));
            
        return V[3];      
}


float VCF::peak(const float& input) 
{
        float r = Q*0.99609375;
        float f = (1-*cutoff);
        a[0] = (1-r)*sqrt(r*(r-4*(f*f)+2)+1);
        a[1] = 2*f*r;
        a[2] = -(r*r);
        out[0] = a[0] *input+a[1]*out[1]+a[2]*out[2]+1.0e-24;
        out[2] = out[1];
        out[1] = out[0];
        return out[0]; 
}


// Q: 1.4142 to ~ 0.1  Cutoff: 0.0 - 1.0 
// Base on code by Patrice Tarrabia https://www.musicdsp.org/en/latest/Filters/38-lp-and-hp-filter.html
struct hp 
{
    private:
        float eax = 0.0f;
        float ebx = 0.0f;
    public:
        float cutoff = 0.7f;
        float Q      = 1.4f;


    float process(float in)
    {
        float c = (cutoff + Q) * cutoff;
        float out = ((1.0/(1.0+c))*eax+in-ebx)*(1.0-c);
        eax = out;
        ebx = in;
        return out;
    }
};

inline float fLowPass(VCF *o, const float& input)
{
        return o->lp24(input);
}

inline float fHighPass(VCF *o, const float& input)
{
        return (input - o->lp24(input))*0.5f;
}

inline float fBandPass(VCF *o, const float& input)
{
        return o->peak(input);
}

inline float fNotch(VCF *o, const float& input)
{       
        return input-o->peak(input);
}

inline float fBypass(VCF *o, const float& input)
{
        return input;
}

float (*form_filter[])(VCF*, const float&) = { fBypass, fLowPass, fHighPass, fBandPass, fNotch };


//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////

#endif

