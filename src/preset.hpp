////////////////////////////////////////////////////////////////////////////////////////
// PRESET
// V.0.0.1 2022-01-2
////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include <fstream>
#include <iostream>
#include <filesystem>
#include <sstream>
#include "spawner.hpp"

#define envn 6
#define lfos 4
#define nn	 12

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

        int     oct [nn]; // Octave values
        bool    note[nn]; // Note On & Off

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

        int     env_slider_type[envn];
        int     env_form[envn];

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

        int     radio_form_env[envn];  // Implement!
        float   scale_env[envn];

        bool    trigger_sequence    = true;

        int     radio_env_amp       = true, 
                radio_env_mod       = true;


        int     lfo_imprint_type[lfos];
        int     lfo_mod_type[lfos];
        float   lfo_mod_amount[lfos];


        LFO     lfo[lfos];
        float   lfo_amp[lfos];
        float   lfo_morph[lfos];
        
        envelope_adsr env[envn];

        vector<float> env_imprint[envn]; // Implement!
        vector<float> lfo_imprint[lfos];


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


	for(int i=0; i<nn; i++)
	{
		ff<<oct[i]<<"\n"; 
    	ff<<note[i]<<"\n";
	}

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

    ff<<trigger_sequence<<"\n";

    ff<<radio_env_amp<<"\n";
    ff<<radio_env_mod<<"\n";

    ff<<lfo_imprint_type[0]<<"\n";
    ff<<lfo_imprint_type[1]<<"\n";
    ff<<lfo_mod_type[0]<<"\n";
    ff<<lfo_mod_type[1]<<"\n";
    ff<<lfo_mod_amount[0]<<"\n";
    ff<<lfo_mod_amount[1]<<"\n";

    ff<<lfo[0].frequency<<"\n";
    ff<<lfo[1].frequency<<"\n";
    /// Amp Envelopes //////////////
	for(int i=0; i<envn; i++)
	{
		ff<<env[i].adsr.A.time<<"\n";
		ff<<env[i].adsr.D.time<<"\n";
		ff<<env[i].adsr.S.time<<"\n";
		ff<<env[i].adsr.R.time<<"\n";

		ff<<env[i].adsr.A.value<<"\n";
		ff<<env[i].adsr.D.value<<"\n";
		ff<<env[i].adsr.S.value<<"\n";
		ff<<env[i].adsr.R.value<<"\n";

		ff<<radio_form_env[i]<<"\n"; // Implement!
		ff<<scale_env[i]<<"\n"; 
	}


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

	for(int i=0; i<nn; i++)
	{
		ff>>f;
        note_set[i] = f;
        f = 0;

		ff>>oct_set[i];
		ff>>oct[i];
    	ff>>note[i];
	}


    for(int i=0; i<oscn; i++) 
	{
		ff>>cuttoff_slider_type[i];
		ff>>amp_slider_type[i];
		ff>>phase_slider_type[i];
		ff>>cutoff[i];

		ff>>amp_mod_type[i];
		ff>>amp_mod_amount[i];
		ff>>pwm_mod_type[i];
		ff>>pwm_mod_amount[i];
		ff>>phase_mod_type[i];
		ff>>phase_mod_amount[i];
		ff>>cutoff_mod_type[i];
		ff>>cutoff_mod_amount[i];
	}
	

    ff>>trigger_sequence;

    ff>>radio_env_amp;
    ff>>radio_env_mod;

    ff>>lfo_imprint_type[0];
    ff>>lfo_imprint_type[1];
    ff>>lfo_mod_type[0];
    ff>>lfo_mod_type[1];
    ff>>lfo_mod_amount[0];
    ff>>lfo_mod_amount[1];


    ff>>lfo[0].frequency;
    ff>>lfo[1].frequency;

    /// Amp Envelopes ////////
	for(int i=0; i<envn; i++)
	{
		ff>>env[i].adsr.A.time;
		ff>>env[i].adsr.D.time;
		ff>>env[i].adsr.S.time;
		ff>>env[i].adsr.R.time;

		ff>>env[i].adsr.A.value;
		ff>>env[i].adsr.D.value;
		ff>>env[i].adsr.S.value;
		ff>>env[i].adsr.R.value;

		ff>>radio_form_env[i]; // Implement!
		ff>>scale_env[i]; 
	}

    ff>>if_square_a;
    ff>>if_square_b;


    ff.close();
}

preset::preset()
{
        lfo_amp[0] = 0.5f;
        lfo_amp[1] = 0.5f;

        lfo[0].amplitude = &lfo_amp[0];
        lfo[1].amplitude = &lfo_amp[1];
        lfo[0].morph = &lfo_morph[0];
        lfo[1].morph = &lfo_morph[1];
        lfo[0].init(true);
        lfo[1].init(true);  
        lfo_imprint[0] = imprint(&lfo[0], 120, 1);
        lfo_imprint[1] = imprint(&lfo[1], 120, 1); 
		for(int i=0; i<nn; i++)
		{
			note[i]=0;
		}
		note[0] = 1;
        for(int osc=0; osc<oscn; osc++)
        {
            octave[osc] = 1;
        } 
		for(int i=0; i<envn; i++)
		{
			scale_env[i]=1.0f; 
		}
}

preset::~preset()
{
}

extern preset o;


