////////////////////////////////////////////////////////////////////////////////////////
// SPAWNER 
// V.0.2.1 2022-01-24
////////////////////////////////////////////////////////////////////////////////////////
#ifndef __SPAWNER
#define __SPAWNER

#include <iostream>
#include <cstdint>
#include <cstdlib>
#include <chrono>
#include <string>
#include <vector>
#include "wavering.hpp"
#include "wform_processor.hpp"
#include "iospecs.hpp"
#include "feeder.hpp"
#include "delay.hpp"
#include "vco.hpp"
#include "scales.hpp"
#include "serpent.hpp"
#include "lfo.hpp"
using std::vector;
#define poly    8       // Polyphony
#define oscn    3       // Number of oscillators
#define lfos    2       // Number of LFOs
#define xm      9       // Modulation matrix X
#define ym      23      // Modulation matrix Y
#define zm      2       // Modulation matrix Z
////////////////////////////////////////////////////////////////////////////////////////
// Feed the destination audio buffer ///////////////////////////////////////////////////
class spawner
{  
    public:
        circular* data;
        waveform_processor wpr;
        serpent   esq;
        delay  dd[3];
        VCO    vco[oscn][poly];
        LFO    lfo[lfos];  
        VCF    vcf[oscn];                           // One filter per VCO
        ADSR   env[oscn*2];                         // Envelope imprints
        float  modmatrix[xm][ym][zm];               // Modulation matrix
        float  cvin[ym];                            // Control values input
        int    form_vco[oscn];                      // Oscillator waveform
        int    form_vcf[oscn];                      // Filter type
        int    octave[oscn];                        // Octave multiplier
        uint   departed;                            // Samples left off
        float  volume   = 0.05f;                    // Overall amplitude
        float  out[oscn];                           // Current sample to process
        float  pan[oscn];                           // 0.0: 100% Left - 1.0: 100
        bool   freerun[oscn];                       // Internal envelopes are off
        bool   trigger_sequence = true;             // If off - triggering individual notes
        int    free[oscn];                          // Currently free voice for every VCO
        int    regenerate[2];                       // If [0] is on - regenerates sequence, [1] - rhythm
        int    algorhithm[2];                       // by given algorhithm[0] - algorithm[1] - forms beat
        float  seed_step[2];                        // with given seed
        int    flags[2];                            // Change flags: [0]: Change speed
        uint   bpm;
        uint   pattern;
        friend class feeder;
        inline void spawn(void);
        inline void init(void);
        inline void trigger(void);
        inline void iterate(const int&, const int&);
        inline void set_modulation();
        inline void start_envelopes();
        inline void clear_matrix();
        inline int  get_free(const int&);
        spawner(circular*);
       ~spawner();
};

////////////////////////////////////////////////////////////////////////////////////////
// Destination buffer setter ///////////////////////////////////////////////////////////
void spawner::spawn(void)
{
    for(size_t i=0; i<settings.buffer_size/2; i++)
    {
        if(departed-esq.offset >esq.step)
        {
            trigger();
        }
        form_lfo[lfo[0].form](&lfo[0]);
        form_lfo[lfo[1].form](&lfo[1]);
        out[0] = 0.0f;
        out[1] = 0.0f;
        out[2] = 0.0f;

        for(int osc=0; osc<oscn; osc++)
        {
            if(freerun[osc])
            {
                if(vco[osc][0].env_mod.state == envelope_adsr::STAGE::OFF)
                {
                    vco[osc][0].env_mod.start();
                }
                vco[osc][0].env_mod.iterate();
                set_modulation();
                form[form_vco[osc]](&vco[osc][0]);
                out[osc] += vco[osc][0].out;
                out[osc] = form_filter[form_vcf[osc]](&vcf[osc], out[osc]);
            }
            else
            {
                for(int voice=0; voice<poly; voice++)
                {
                    if(vco[osc][voice].on)
                    {
                        set_modulation();
                        iterate(osc, voice);
                        out[osc] += vco[osc][voice].out * vco[osc][voice].env_amp.iterate();
                        out[osc] = form_filter[form_vcf[osc]](&vcf[osc], out[osc]);
                        if(vco[osc][voice].env_amp.state==envelope_adsr::STAGE::OFF)
                        {
                            vco[osc][voice].on = false;
                        }
                    }
                }
            }
        }
        out[0] = fDD[0](&dd[0], out[0]);
        out[1] = fDD[0](&dd[1], out[1]);
        out[2] = fDD[0](&dd[2], out[2]);

        data->set(fProcess[wpr.mode](&wpr, waveform_processor::CHANNEL::LEFT));
        data->set(fProcess[wpr.mode](&wpr, waveform_processor::CHANNEL::RIGHT));
    }
}

void spawner::iterate(const int& osc, const int& voice)
{
        form[form_vco[osc]](&vco[osc][voice]);
        vco[osc][voice].env_amp.iterate();
        vco[osc][voice].env_mod.iterate();
}


void spawner::trigger()
{
        esq.next();   
        init();
        if(trigger_sequence)
        {
            if(esq.is==0)
            {
                for(int osc=0; osc<oscn; osc++) free[osc] = get_free(osc);
                start_envelopes();
            }
        }
        else
        {
            for(int osc=0; osc<oscn; osc++) free[osc] = get_free(osc);
            start_envelopes();
        }

        if(esq.is==0)
        {
            if(regenerate[0])
            {
                esq.seed[0] += (seed_step[0]*regenerate[0]);
                esq.regenerate(algorhithm[0]);
            }
            if(regenerate[1])
            {
                esq.seed[1] += (seed_step[1]*regenerate[1]);
                esq.pattern = beat_divide(fR[algorhithm[1]](esq.seed[1]), esq.divisor);
                esq.timeset(bpm);
            }
            if(flags[0])
            {
                esq.pattern = pattern;
                esq.timeset(bpm);
                flags[0] = 0;
            }
        }
}

void spawner::start_envelopes()
{
    for(int osc=0; osc<oscn; osc++)
    {
        if(!freerun[osc])
        {
            vco[osc][free[osc]].on = true;
            vco[osc][free[osc]].env_amp.start();
        }
        if(vco[osc][free[osc]].env_mod.state == envelope_adsr::STAGE::OFF)
        vco[osc][free[osc]].env_mod.start();
    }
}

void spawner::init(void)
{
    for(int osc=0; osc<oscn; osc++)
    {
        vco[osc][free[osc]].env_amp.adsr = env[osc];  
        vco[osc][free[osc]].env_mod.adsr = env[osc+2];   
        vco[osc][free[osc]].frequency= esq.note*octave[osc];
        vco[osc][free[osc]].init();
    }
}


int spawner::get_free(const int& osc)
{   
    int i = 0;
    while (i<(poly-1))
    {
       if(!vco[osc][i].on) break;
       i++;
    }
    return i;
}


spawner::spawner(circular* buffer): data(buffer)
{
    esq = serpent("A4", 10000, 0b1111);
    esq.departed = &departed;
    esq.octaves = 1;    
    lfo[0].init(true);
    lfo[1].init(true);

    cvin[0]  = 1.0f;
    cvin[1]  = 1.0f;
    cvin[2]  = 1.0f;

    cvin[3]  = 1.0f;
    cvin[4]  = 1.0f;
    cvin[5]  = 1.0f;

    cvin[6]  = 0.0f;
    cvin[7]  = 0.0f;
    cvin[8]  = 0.0f;

    cvin[9]  = 1.0f;
    cvin[10] = 1.0f;
    cvin[11] = 1.0f;

    cvin[12] = 0.5f;
    cvin[13] = 0.5f;

    freerun[0] = false;
    freerun[1] = false;
    freerun[2] = true;

    for(int osc=0; osc<oscn; osc++)
    {
        free[osc]   = 0;
        octave[osc] = 1;
    } 
    for(int v=0; v<poly; v++)
    {
        vco[0][v].amplitude  = &cvin[0];
        vco[1][v].amplitude  = &cvin[1];
        vco[2][v].amplitude  = &cvin[2];
            
        vco[0][v].pwmcv      = &cvin[3];
        vco[1][v].pwmcv      = &cvin[4];
        vco[2][v].pwmcv      = &cvin[5];

        vco[0][v].phasecv    = &cvin[6];
        vco[1][v].phasecv    = &cvin[7];
        vco[2][v].phasecv    = &cvin[8];

        vco[0][v].fcv        = &cvin[17];
        vco[1][v].fcv        = &cvin[18];
        vco[2][v].fcv        = &cvin[19];

    }
        vcf[0].cutoff        = &cvin[9];
        vcf[1].cutoff        = &cvin[10];
        vcf[2].cutoff        = &cvin[11];

        // lfo[0].amplitude     = &cvin[12];
        // lfo[1].amplitude     = &cvin[13];

        lfo[0].morph         = &cvin[12];
        lfo[1].morph         = &cvin[13];

        wpr.pan[0]           = &cvin[14];
        wpr.pan[1]           = &cvin[15];
        wpr.pan[2]           = &cvin[16];

        dd[0].time           = &cvin[20];
        dd[1].time           = &cvin[21];
        dd[2].time           = &cvin[22];


        wpr.amount = &volume;
        wpr.source[0]  = &out[0];
        wpr.source[1]  = &out[1];
        wpr.source[2]  = &out[2]; 

}


spawner::~spawner()
{

}


void mfEnvA(spawner* o, int target, float amount, float center)
{
    o->cvin[target] = o->vco[0]->env_mod.out * amount * 2.0f + center;
}

void mfEnvB(spawner* o, int target, float amount, float center)
{
    o->cvin[target] = o->vco[1]->env_mod.out * amount * 2.0f + center;
}

void mfEnvC(spawner* o, int target, float amount, float center)
{
    o->cvin[target] = o->vco[0]->env_mod.out * amount * 2.0f + center;
}

void mfEnvD(spawner* o, int target, float amount, float center)
{
    o->cvin[target] = o->vco[1]->env_mod.out * amount * 2.0f + center;
}

void mfLfoA(spawner* o, int target, float amount, float center) 
{
    o->cvin[target] = o->lfo[0].out * amount * 0.5 + 0.5f + center;
}

void mfLfoB(spawner* o, int target, float amount, float center) 
{
    o->cvin[target] = o->lfo[1].out * amount * 0.5 + 0.5f + center;
}

void mfVcoA(spawner* o, int target, float amount, float center)
{
    o->cvin[target] = o->vco[0]->out * amount * 0.5 + 0.5f + center;
}

void mfVcoB(spawner* o, int target, float amount, float center)
{
    o->cvin[target] = o->vco[1]->out * amount * 0.5 + 0.5f + center;
}

void mfVcoC(spawner* o, int target, float amount, float center)
{
    o->cvin[target] = o->vco[2]->out * amount * 0.5 + 0.5f + center;
}

void (*setmod[])(spawner*, int, float, float) = {   mfEnvA, 
                                                    mfEnvB, 
                                                    mfEnvC, 
                                                    mfEnvD, 
                                                    mfLfoA, 
                                                    mfLfoB, 
                                                    mfVcoA, 
                                                    mfVcoB,
                                                    mfVcoC 
                                                };

void spawner::set_modulation()
{
    for(int j=0; j<xm; j++)
    {
        for(int i=0; i<ym; i++)
        {
            if(modmatrix[j][i][0])
            {
                setmod[j](this, i, modmatrix[j][i][0], modmatrix[j][i][1]);
            }
        }
    }
}

void spawner::clear_matrix()
{
    for(int j=0; j<xm; j++)
    {
        for(int i=0; i<ym; i++)
        {
            modmatrix[j][i][0] = 0.0f;
            modmatrix[j][i][1] = 0.0f;
        }
    }
}

#endif /////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////


