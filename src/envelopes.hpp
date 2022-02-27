////////////////////////////////////////////////////////////////////////////////////////
// ENVELOPES
// V.0.3.6 2022-02-19
////////////////////////////////////////////////////////////////////////////////////////
#ifndef __ENVELOPES
#define __ENVELOPES
#include <cstdlib>
#include <cmath>
#include <vector>
#include <iostream>

using std::vector;

typedef struct{
    float  time;
    float value;
} BREAKPOINT;

typedef struct{
    BREAKPOINT A;
    BREAKPOINT D;
    BREAKPOINT S;
    BREAKPOINT R;
} ADSR;

class envelope_adsr
{
    private:
    public:
        enum   STAGE {OFF, A, D, S, R};
        enum   TYPE  {LIN, LOG, CUB, SFT};
        STAGE  state = OFF;
        ADSR   adsr;
        TYPE   curve = CUB;
        uint   departed;      // Current sample
        uint   breakpoint;    // Samples before next stage
        int    form[4];       // Function for given segment
        float  sample_rate;   // System sample rate
        float  e;             // Multiplier
        float  level;         // Previous - Next
        float  scale;         // Overall time multiplier
        float  min;           // Floor
        float  out;           // Current output
        inline float coefficient(float, float, uint);
        inline float process();
        inline void  start();          
        inline void  next_stage();
        inline void  init();
        inline float iterate();
        envelope_adsr();
       ~envelope_adsr();
};


// t = Time - Amount of time that has passed since the beginning.
// b = Beginning value - The starting point of the transition.
// c = Change in value - The amount of change needed to go from starting point to end point.
// d = Duration - Amount of time the transition will take.

inline float fLinear(float t, float b, float c, float d)
{
        return c * t / d + b;
}

inline float fCubicIn(float t, float b, float c, float d)
{
        t /= d;
	    return c*t*t*t + b;
}

inline float fCubicOut(float t, float b, float c, float d)
{
        t /= d;
        t--;
        return c*(t*t*t + 1) + b;
};

inline float fCubicIO(float t, float b, float c, float d)
{
        t /= d/2;
        if (t < 1) return c/2*t*t*t + b;
        t -= 2;
        return c/2*(t*t*t + 2) + b;
}

inline float (*fCUBe[])(float, float, float, float) = { fLinear,
                                                        fCubicOut,
                                                        fCubicIn,
                                                        fCubicIO
                                                    
                                                    };


inline float fO(envelope_adsr* o)
{
        return 0.00001;
}

inline float fAttack(envelope_adsr* o)
{
        return fCUBe[o->form[0]](float(o->departed), o->adsr.A.value, o->level, o->breakpoint);
}

inline float fDecay(envelope_adsr* o)
{
        return fCUBe[o->form[1]](float(o->departed), o->adsr.D.value, o->level, o->breakpoint);
}

inline float fSustain(envelope_adsr* o)
{
        return fCUBe[o->form[2]](float(o->departed), o->adsr.S.value, o->level, o->breakpoint);
}

inline float fRelCUBe(envelope_adsr* o)
{
        return fCUBe[o->form[3]](float(o->departed), o->adsr.R.value, o->level, o->breakpoint);
}

inline float (*fEnv[])(envelope_adsr* o) = {fO, fAttack, fDecay, fSustain, fRelCUBe};

inline float envelope_adsr::process()
{
        return fEnv[state](this);
}

inline void envelope_adsr::start()
{
    if(curve==CUB)
    {
        form[0] = 1;
        form[1] = 1;
        form[2] = 2;
        form[3] = 1;
    }
    else if(curve==SFT)
    {
        form[0] = 1;
        form[1] = 3;
        form[2] = 3;
        form[3] = 3;
    }
    state = A;
    init();
}

inline void envelope_adsr::init()
{
    departed = 0;
    switch (state)
    {
        case OFF: 
            level = min;
            breakpoint = 0;
            break;
        case A:   
            breakpoint = adsr.A.time; 
            if(curve>1) level = adsr.D.value - adsr.A.value;
            else 
            {
                level = adsr.A.value;
                e = coefficient(level, adsr.D.value, breakpoint); 
            }
            break;
        case D:   
            breakpoint = adsr.D.time; 
            if(curve>1) level = adsr.S.value - adsr.D.value;
            else
            {
                level = adsr.D.value;
                e = coefficient(level, adsr.S.value, breakpoint); 
            }
            break;
        case S:   
            breakpoint = adsr.S.time;
            if(curve>1) level = adsr.R.value - adsr.S.value;
            else
            {
                level = adsr.S.value;
                e = coefficient(level, adsr.R.value, breakpoint); 
            }
            break;
        case R:   
            breakpoint = adsr.R.time;
            if(curve>1) level = min - adsr.R.value;
            else
            {
                level = adsr.R.value;
                e = coefficient(level, min, breakpoint); 
            }
            break;
        default:  
            break;
    }
}

inline void envelope_adsr::next_stage()
{
    switch (state)
    {
        case A:   state = D;    init(); break;
        case D:   state = S;    init(); break;
        case S:   state = R;    init(); break;
        case R:   state = OFF;  init(); break;
        default:                        break;
    }
}

inline float envelope_adsr::iterate()
{
        departed++;
        if(departed==breakpoint) { next_stage(); }
        if(curve==LIN) out = level+=e;
        if(curve==LOG) out = level+=(level*e);
        if(curve==CUB) out = process();
        if(curve==SFT) out = process();
        return out;
}

inline float envelope_adsr::coefficient(float start_value, float end_value, uint length) 
{
        if(curve==LIN) return (end_value-start_value)/float(length);
        if(curve==LOG) return (log(end_value)-log(start_value))/float(length);
        return 1;
}


envelope_adsr::envelope_adsr()
{
        adsr.A.value = 0.01f;
        adsr.A.time  = 100.0f;

        adsr.D.value = 1.0f;
        adsr.D.time  = 5000.0f;

        adsr.S.value = 0.3f;
        adsr.S.time  = 5000.0f;
        
        adsr.R.value = 0.3f;
        adsr.R.time  = 5000.0f;

        departed     = 0;
        sample_rate  = 48000;
        e            = 1.0f;
        scale        = 1.0f;
        min          = 0.000001f;

        form[0] = 1;
        form[1] = 1;
        form[2] = 2;
        form[3] = 1;
        
}

envelope_adsr::~envelope_adsr()
{
    
}

vector<float> imprint(envelope_adsr *adsr, int offset_left, int offset_right)
{
    float w =   adsr->adsr.A.time + 
                adsr->adsr.D.time + 
                adsr->adsr.S.time + 
                adsr->adsr.R.time ;
    vector<float> r(offset_left,0);
                adsr->start();
                int i = 0;
            
                while (i<int(w))
                {
                    r.push_back(adsr->iterate()); 
                    i++; 
                }
                i=0;
                while (i<offset_right)
                {
                    r.push_back(0); i++;
                }
                
    return r;
}

#endif