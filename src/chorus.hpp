#include "delay.hpp"
#include "lfo.hpp"

struct chorus
{
    delay dd;
    LFO   lfo;
    float rate;
    float depth = 1;
    float level;
    float morph = 0.5;
    float eq;       
    int   form = 2;     // LFO form
    float out;
    float process(const float&);
    chorus();
};

float chorus::process(const float& in)
{
    lfo.delta = rate*4;
    
    form_lfo[form](&lfo);
    out = fDD[0](&dd, in)*level;
    return out;
}

chorus::chorus()
{
    dd.feedback = 0.2;
    dd.amount = 0.4;
    
    lfo.morph = &morph;
    lfo.amplitude = &depth;
    dd.time = &lfo.out;
}