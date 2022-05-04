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
    lfo.delta = rate / 20.0f + 0.001f;
    form_lfo[form](&lfo);
    out = fDD[3](&dd, in) *level;
    return out;
}


chorus::chorus()
{
    lfo.morph       = &morph;
    lfo.amplitude   = &depth;
    dd.time         = &lfo.out;
}