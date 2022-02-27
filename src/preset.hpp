////////////////////////////////////////////////////////////////////////////////////////
// PRESET
// V.0.0.1 2022-01-26
////////////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef __PRESET
#define __PRESET
#include <fstream>
#include <iostream>
#include <filesystem>
#include <sstream>
#include "spawner.hpp"

class preset
{
    public:
        std::fstream ff;
        std::stringstream ss;
        const std::filesystem::path presets{"presets"};
            
        

        float   tempo   = 0.1f, 
                seed    = 0.001;
        uint    beat    = 0x80808080;
        int     rhythm[4] = {128, 128, 128, 128};
        int     octaves = 1;

        int octave[oscn];
            
        vector<bool> note_set = {1,0,0,0,0,0,0,0,0,0,0,0};
        vector<int>  oct_set  = {4,4,4,4,4,4,4,4,4,4,4,4};

        // Octave values: o = octave; a,b,c,etc. = note; s = sharp, n = natural
        int     oan = 3, oas = 3, obn = 3, ocn = 3,
                ocs = 3, odn = 3, ods = 3, oen = 3,
                ofn = 3, ofs = 3, ogn = 3, ogs = 3;
        // Note On & Off; n = note
        bool    nan = 1, nas = 0, nbn = 0, ncn = 0,
                ncs = 0, ndn = 0, nds = 0, nen = 0,
                nfn = 0, nfs = 0, ngn = 0, ngs = 0;

        // Center values for modulations
        float   center_amp[oscn],
                center_pwm[oscn],
                center_phase[oscn],
                center_cutoff[oscn];

        int     cuttoff_slider_type[oscn];
        int     amp_slider_type[oscn];
        int     detune_slider_type[oscn];
        int     delay_slider_type[oscn];
        int     pan_slider_type[oscn];
        int     phase_slider_type[oscn];
        int     pwm_slider_type[oscn];
        float   cutoff[oscn];

        int     env_slider_type[4];
        int     env_form[4];

        float   volume              = 0.05;
        int     form_vco[oscn];

        int     amp_mod_type[oscn];
        int     detune_mod_type[oscn];
        int     pan_mod_type[oscn];
        float   amp_mod_amount[oscn];
        float   detune_mod_amount[oscn];
        float   ddtime_mod_amount[oscn];
        int     ddtime_mod_type[oscn];
        float   pan_mod_amount[oscn];
        int     pwm_mod_type[oscn];
        float   pwm_mod_amount[oscn];
        int     phase_mod_type[oscn];
        float   phase_mod_amount[oscn];
        int     cutoff_mod_type[oscn];
        float   cutoff_mod_amount[oscn];

        int     radio_form_env_amp_a = 1,  
                radio_form_env_amp_b = 1;
        int     radio_form_env_mod_a = 1,  
                radio_form_env_mod_b = 1;
        float   scale_env_amp_a     = 1.0f, 
                scale_env_amp_b     = 1.0f;
        float   scale_env_mod_a     = 1.0f, 
                scale_env_mod_b     = 1.0f;

        bool    trigger_sequence    = true;

        int     radio_env_amp       = true, 
                radio_env_mod       = true;


        int     lfo_imprint_a_type = 0, 
                lfo_imprint_b_type = 0;
        int     lfo_a_mod_type,
                lfo_b_mod_type;
        float   lfo_a_mod_amount,
                lfo_b_mod_amount;

        LFO     lfo_a, lfo_b;
        float   lfo_a_amp = 0.5, lfo_b_amp = 0.5;
        float   lfo_morph[lfos];
        
        envelope_adsr env_amp_a;
        envelope_adsr env_amp_b; 
        envelope_adsr env_mod_a;
        envelope_adsr env_mod_b;

        vector<float> env_imprint_amp_a;
        vector<float> env_imprint_amp_b;
        vector<float> env_imprint_mod_a;
        vector<float> env_imprint_mod_b;
            
        vector<float> lfo_imprint_a;
        vector<float> lfo_imprint_b;

        bool    if_square_a = false, 
                if_square_b = false;
            

        const string    path_prefix = "presets/";
        vector<string>  preset_list;
        string name;
        size_t preset_count;
        void save(const string&);
        void save();
        void load();
        void get_list();
        preset();
       ~preset();
};

void preset::get_list()
{
    int st = path_prefix.length()+1;
    preset_count = 0;
    for (auto const& entry : std::filesystem::directory_iterator{presets}) 
    {
        ss<<entry;
        name = ss.str().substr(st, ss.str().length() - st-1);
        preset_list.push_back(name);
        ss.str("");
        ss.clear();
        preset_count++;
    }
}

void preset::save()
{
    ff.open(path_prefix+name, std::ios_base::out);
    
    ff<<volume<<"\n";
    for(int i=0; i<oscn; i++) ff<<form_vco[i]<<"\n";

    ff<<tempo<<"\n";
    ff<<beat<<"\n";
    ff<<seed<<"\n";
    ff<<octaves<<"\n";

    for(auto i: note_set) ff<<i<<"\n";
    for(auto i: oct_set ) ff<<i<<"\n";

    ff<<oan<<"\n";  ff<<oas<<"\n";  ff<<obn<<"\n";
    ff<<ocn<<"\n";  ff<<ocs<<"\n";  ff<<odn<<"\n";
    ff<<ods<<"\n";  ff<<oen<<"\n";  ff<<ofn<<"\n";
    ff<<ofs<<"\n";  ff<<ogn<<"\n";  ff<<ogs<<"\n";

    ff<<nan<<"\n";  ff<<nas<<"\n";  ff<<nbn<<"\n";
    ff<<ncs<<"\n";  ff<<ncs<<"\n";  ff<<ndn<<"\n";
    ff<<nds<<"\n";  ff<<nen<<"\n";  ff<<nfn<<"\n";
    ff<<nfs<<"\n";  ff<<ngn<<"\n";  ff<<ngs<<"\n";

    for(int i=0; i<oscn; i++) ff<<cuttoff_slider_type[i]<<"\n";
    for(int i=0; i<oscn; i++) ff<<amp_slider_type[i] <<"\n";
    for(int i=0; i<oscn; i++) ff<<phase_slider_type[i]<<"\n";
    for(int i=0; i<oscn; i++) ff<<cutoff[i]<<"\n";


    for(int i=0; i<oscn; i++) ff<<amp_mod_type[i]<<"\n";
    for(int i=0; i<oscn; i++) ff<<amp_mod_amount[i]<<"\n";
    for(int i=0; i<oscn; i++) ff<<pwm_mod_type[i]<<"\n";
    for(int i=0; i<oscn; i++) ff<<pwm_mod_amount[i]<<"\n";
    for(int i=0; i<oscn; i++) ff<<phase_mod_type[i]<<"\n";
    for(int i=0; i<oscn; i++) ff<<phase_mod_amount[i]<<"\n";
    for(int i=0; i<oscn; i++) ff<<cutoff_mod_type[i]<<"\n";
    for(int i=0; i<oscn; i++) ff<<cutoff_mod_amount[i]<<"\n";


    ff<<radio_form_env_amp_a<<"\n";
    ff<<radio_form_env_amp_b<<"\n";
    ff<<radio_form_env_mod_a<<"\n";
    ff<<radio_form_env_mod_b<<"\n";
    ff<<scale_env_amp_a<<"\n"; 
    ff<<scale_env_amp_b<<"\n";
    ff<<scale_env_mod_a<<"\n"; 
    ff<<scale_env_mod_b<<"\n";

    ff<<trigger_sequence<<"\n";

    ff<<radio_env_amp<<"\n";
    ff<<radio_env_mod<<"\n";

    ff<<lfo_imprint_a_type<<"\n";
    ff<<lfo_imprint_b_type<<"\n";
    ff<<lfo_a_mod_type<<"\n";
    ff<<lfo_b_mod_type<<"\n";
    ff<<lfo_a_mod_amount<<"\n";
    ff<<lfo_b_mod_amount<<"\n";

    ff<<lfo_a.frequency<<"\n";
    ff<<lfo_b.frequency<<"\n";
    /// Amp Envelopes //////////////
    ff<<env_amp_a.adsr.A.time<<"\n";
    ff<<env_amp_a.adsr.D.time<<"\n";
    ff<<env_amp_a.adsr.S.time<<"\n";
    ff<<env_amp_a.adsr.R.time<<"\n";

    ff<<env_amp_a.adsr.A.value<<"\n";
    ff<<env_amp_a.adsr.D.value<<"\n";
    ff<<env_amp_a.adsr.S.value<<"\n";
    ff<<env_amp_a.adsr.R.value<<"\n";

    ff<<env_amp_b.adsr.A.time<<"\n";
    ff<<env_amp_b.adsr.D.time<<"\n";
    ff<<env_amp_b.adsr.S.time<<"\n";
    ff<<env_amp_b.adsr.R.time<<"\n";

    ff<<env_amp_b.adsr.A.value<<"\n";
    ff<<env_amp_b.adsr.D.value<<"\n";
    ff<<env_amp_b.adsr.S.value<<"\n";
    ff<<env_amp_b.adsr.R.value<<"\n";
    /// Mod Envelopes //////////////
    ff<<env_mod_a.adsr.A.time<<"\n";
    ff<<env_mod_a.adsr.D.time<<"\n";
    ff<<env_mod_a.adsr.S.time<<"\n";
    ff<<env_mod_a.adsr.R.time<<"\n";

    ff<<env_mod_a.adsr.A.value<<"\n";
    ff<<env_mod_a.adsr.D.value<<"\n";
    ff<<env_mod_a.adsr.S.value<<"\n";
    ff<<env_mod_a.adsr.R.value<<"\n";

    ff<<env_mod_b.adsr.A.time<<"\n";
    ff<<env_mod_b.adsr.D.time<<"\n";
    ff<<env_mod_b.adsr.S.time<<"\n";
    ff<<env_mod_b.adsr.R.time<<"\n";

    ff<<env_mod_b.adsr.A.value<<"\n";
    ff<<env_mod_b.adsr.D.value<<"\n";
    ff<<env_mod_b.adsr.S.value<<"\n";
    ff<<env_mod_b.adsr.R.value<<"\n";

    ff<<if_square_a<<"\n";
    ff<<if_square_b<<"\n";
               

    ff.close();
}

void preset::load()
{
    ff.open(path_prefix+name, std::ios_base::in);

    ff>>volume;
    for(int i=0; i<oscn; i++) ff>>form_vco[i];

    ff>>tempo;
    ff>>beat;
    ff>>seed;
    ff>>octaves;
    int f=0;
    for(size_t i=0;i<12;i++)
    {
        ff>>f;
        note_set[i] = f;
        f = 0;
    }
    for(size_t i=0;i<12;i++) ff>>oct_set[i];

    ff>>oan; ff>>oas; ff>>obn; ff>>ocn;
    ff>>ocs; ff>>odn; ff>>ods; ff>>oen;
    ff>>ofn; ff>>ofs; ff>>ogn; ff>>ogs;

    ff>>nan; ff>>nas; ff>>nbn; ff>>ncn;
    ff>>ncs; ff>>ndn; ff>>nds; ff>>nen;
    ff>>nfn; ff>>nfs; ff>>ngn; ff>>ngs;

    for(int i=0; i<oscn; i++) ff>>cuttoff_slider_type[i];
    for(int i=0; i<oscn; i++) ff>>amp_slider_type[i];
    for(int i=0; i<oscn; i++) ff>>phase_slider_type[i];
    for(int i=0; i<oscn; i++) ff>>cutoff[i];

    for(int i=0; i<oscn; i++) ff>>amp_mod_type[i];
    for(int i=0; i<oscn; i++) ff>>amp_mod_amount[i];
    for(int i=0; i<oscn; i++) ff>>pwm_mod_type[i];
    for(int i=0; i<oscn; i++) ff>>pwm_mod_amount[i];
    for(int i=0; i<oscn; i++) ff>>phase_mod_type[i];
    for(int i=0; i<oscn; i++) ff>>phase_mod_amount[i];
    for(int i=0; i<oscn; i++) ff>>cutoff_mod_type[i];
    for(int i=0; i<oscn; i++) ff>>cutoff_mod_amount[i];


    ff>>radio_form_env_amp_a;
    ff>>radio_form_env_amp_b;
    ff>>radio_form_env_mod_a;
    ff>>radio_form_env_mod_b;
    ff>>scale_env_amp_a; 
    ff>>scale_env_amp_b;
    ff>>scale_env_mod_a; 
    ff>>scale_env_mod_b;

    ff>>trigger_sequence;

    ff>>radio_env_amp;
    ff>>radio_env_mod;

    ff>>lfo_imprint_a_type;
    ff>>lfo_imprint_b_type;
    ff>>lfo_a_mod_type;
    ff>>lfo_b_mod_type;
    ff>>lfo_a_mod_amount;
    ff>>lfo_b_mod_amount;


    ff>>lfo_a.frequency;
    ff>>lfo_b.frequency;

    /// Amp Envelopes ////////
    ff>>env_amp_a.adsr.A.time;
    ff>>env_amp_a.adsr.D.time;
    ff>>env_amp_a.adsr.S.time;
    ff>>env_amp_a.adsr.R.time;

    ff>>env_amp_a.adsr.A.value;
    ff>>env_amp_a.adsr.D.value;
    ff>>env_amp_a.adsr.S.value;
    ff>>env_amp_a.adsr.R.value;

    ff>>env_amp_b.adsr.A.time;
    ff>>env_amp_b.adsr.D.time;
    ff>>env_amp_b.adsr.S.time;
    ff>>env_amp_b.adsr.R.time;

    ff>>env_amp_b.adsr.A.value;
    ff>>env_amp_b.adsr.D.value;
    ff>>env_amp_b.adsr.S.value;
    ff>>env_amp_b.adsr.R.value;

    /// Mod Envelopes ////////
    ff>>env_mod_a.adsr.A.time;
    ff>>env_mod_a.adsr.D.time;
    ff>>env_mod_a.adsr.S.time;
    ff>>env_mod_a.adsr.R.time;

    ff>>env_mod_a.adsr.A.value;
    ff>>env_mod_a.adsr.D.value;
    ff>>env_mod_a.adsr.S.value;
    ff>>env_mod_a.adsr.R.value;

    ff>>env_mod_b.adsr.A.time;
    ff>>env_mod_b.adsr.D.time;
    ff>>env_mod_b.adsr.S.time;
    ff>>env_mod_b.adsr.R.time;

    ff>>env_mod_b.adsr.A.value;
    ff>>env_mod_b.adsr.D.value;
    ff>>env_mod_b.adsr.S.value;
    ff>>env_mod_b.adsr.R.value;

    ff>>if_square_a;
    ff>>if_square_b;


    ff.close();
}

preset::preset()
{
        lfo_a.amplitude = &lfo_a_amp;
        lfo_b.amplitude = &lfo_b_amp;
        lfo_a.morph = &lfo_morph[0];
        lfo_b.morph = &lfo_morph[1];
        lfo_a.init(true);
        lfo_b.init(true);  
        lfo_imprint_a = imprint(&lfo_a, 120, 1);
        lfo_imprint_b = imprint(&lfo_b, 120, 1); 
        for(int osc=0; osc<oscn; osc++)
        {
            octave[osc] = 1;
        } 
}

preset::~preset()
{
}

extern preset o;



#endif