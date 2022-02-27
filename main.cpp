////////////////////////////////////////////////////////////////////////////////////////
// SERPENT - Algorhithmic sequencer/ synth
// V.0.3.7 2022-02-19
////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <bitset>
#include <stdio.h>
#include <utility>

#include "interface.hpp"
#include "iospecs.hpp"
#include "feeder.hpp"
#include "preset.hpp"



int main(int, char**)
{
    // Init //////////////////////////////////////////////////////////////////////////////
    if (SDL_Init(SDL_INIT_AUDIO))
    std::cerr<<"SDL could not initialize audio : "<<SDL_GetError()<<"\n";
    static feeder mute;

    ///////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0)
    {
        printf("Error: %s\n", SDL_GetError());
        return -1;
    }
    const char* glsl_version = "#version 130";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    // Create window with graphics context
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI| SDL_WINDOW_BORDERLESS);
    SDL_Window* window = SDL_CreateWindow("fFFF", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 788, 490, window_flags);

    if (SDL_SetWindowHitTest(window, hitTest, NULL) == -1) {
        SDL_Log("Enabling hit-testing failed!\n");
        SDL_Quit();
        return 1;
    }


    SDL_GLContext gl_context = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, gl_context);
    SDL_GL_SetSwapInterval(1); // Enable vsync

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls

    set_style();

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Setup Platform/Renderer backends /////////////////////////////////////////////////////////////////////////////////////////////////
    ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
    ImGui_ImplOpenGL3_Init(glsl_version);



            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            // SETUP ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            static std::bitset<32> B;
            static float* scope = mute.data->get();

            const int n_mods  = 10;
            const int n_waves = 12;
            const int n_vcfs  = 5;
            const int n_lfos  = 7;
            const int n_algs  = 3;
            //const int n_proc  = 2;
            const int n_dd    = 3;
            const int n_envs  = 4;


            const char* mod_types[n_mods] = {   "Modulator:   Off",
                                                "Modulator: ADSR1", 
                                                "Modulator: ADSR2", 
                                                "Modulator: ADSR3",
                                                "Modulator: ADSR4",
                                                "Modulator:  LFO1", 
                                                "Modulator:  LFO2",
                                                "Modulator:  VCO1",
                                                "Modulator:  VCO2",
                                                "Modulator:  VCO3" 
                        
                                                };

            const char* wave_names[n_waves] = { "Wave:       Sine   ", //0
                                                "Wave:       Ramp   ", //1
                                                "Wave:        Saw   ", //2
                                                "Wave:      Pulse   ", //3
                                                "Wave:   Tomisawa   ", //4
                                                "Wave:   Triangle   ", //5
                                                "Wave:     Ginger   ", //6
                                                "Wave:      Ikeda   ", //7
                                                "Wave:    Duffing   ", //8
                                                "Wave:  Fabrikant   ", //9
                                                "Wave:  VanDerPol   ", //10
                                                "Wave:      Noise   "};//11

            const char* vcf_names[n_vcfs] = {   "Filter:      OFF   ",
                                                "Filter:  LowPass   ", 
                                                "Filter: HighPass   ", 
                                                "Filter: BandPass   ", 
                                                "Filter:    Notch   "};

            const char* lfo_names[n_lfos] =   { "LFO:        Sine", 
                                                "LFO:         Saw", 
                                                "LFO:    Triangle",
                                                "LFO:    Triangle",
                                                "LFO:      Lorenz",
                                                "LFO:       Ikeda",
                                                "LFO:    Roessler"
                                                };


            const char* algorhithms[n_algs] = { "3n+1  Conjecture   ", 
                                                "Lorenz Attractor   ", 
                                                "Hofstaedter    Q   "};

            // const char* proc_names[n_proc]  = { "Mixer              ",
            //                                     "Waveform Processor "};

            const char* dd_names_l[n_dd]    = { "Left:      Delay",
                                                "Left:     Reverb",
                                                "Left:     Chorus"
                                            };

            const char* dd_names_c[n_dd]    = { "Centre:    Delay",
                                                "Centre:   Reverb",
                                                "Centre:   Chorus"
                                            };

            const char* dd_names_r[n_dd]    = { "Right:     Delay",
                                                "Right:    Reverb",
                                                "Right:    Chorus"
                                            };

            const char* env_names[n_envs]   = { "Form:             Cubic",
                                                "Form:      Linear (eco)",
                                                "Form: Logarithmic (eco)",
                                                "Form:      Cubic (soft)"
                                                
                                                };
    

            static int current_src = 0;
            static vector<bool> rset(32,0);
            bool if_square[oscn];
            bool if_morph[oscn];
            bool if_phase[oscn];

            static char pset_name[32] = "";       
            static int  switch_lfo_a,
                        switch_lfo_b;
            static bool f[32];
            static bool seed_up = false, seed_down = false;

            static int steps = 32;

            static preset o;
           
            // mute.renderer->lfo[0].morph = &o.lfo_morph[0];
            // mute.renderer->lfo[1].morph = &o.lfo_morph[1];

            o.lfo_a.morph = mute.renderer->lfo[0].morph;
            o.lfo_b.morph = mute.renderer->lfo[1].morph;

            

            mute.renderer->lfo[0].amplitude = o.lfo_a.amplitude;
            mute.renderer->lfo[1].amplitude = o.lfo_b.amplitude;

            o.lfo_a.amplitude = mute.renderer->lfo[0].amplitude;
            o.lfo_b.amplitude = mute.renderer->lfo[1].amplitude;
        

            mute.renderer->pattern = o.beat;
            mute.renderer->esq.pattern = o.beat;
            mute.renderer->esq.timeset(int(o.tempo*10000));
            
            struct plot_f
            {
                static float plot_adsr1(void*, int i)  { return o.env_imprint_amp_a.at(i); }
                static float plot_adsr2(void*, int i)  { return o.env_imprint_amp_b.at(i); }
                static float plot_adsr3(void*, int i)  { return o.env_imprint_mod_a.at(i); }
                static float plot_adsr4(void*, int i)  { return o.env_imprint_mod_b.at(i); }
                static float plot_lfo_imprint_a(void*, int i)   { return o.lfo_imprint_a.at(i);  }
                static float plot_lfo_imprint_b(void*, int i)   { return o.lfo_imprint_b.at(i);  }
                static float plot_output(void*, int i) { return scope[i]+scope[i-1]; } 
            };
            // mute.renderer->lfo[0].out * mute.renderer->cvin[12];
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Main loop ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    bool done = false;
    while (!done)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT)
                done = true;
            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(window))
                done = true;
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) done = true;
        }

        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Start the Dear ImGui frame /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();
        {
            
            const char* wave_name_a = (mute.renderer->form_vco[0] >= 0 && mute.renderer->form_vco[0] < n_waves) ? wave_names[mute.renderer->form_vco[0]] : "Unknown";
            const char* wave_name_b = (mute.renderer->form_vco[1] >= 0 && mute.renderer->form_vco[1] < n_waves) ? wave_names[mute.renderer->form_vco[1]] : "Unknown";
            const char* wave_name_c = (mute.renderer->form_vco[2] >= 0 && mute.renderer->form_vco[2] < n_waves) ? wave_names[mute.renderer->form_vco[2]] : "Unknown";
            
            const char* mod_amp1 = (o.amp_mod_type[0] >= 0 && o.amp_mod_type[0] < n_mods) ? mod_types[o.amp_mod_type[0]] : "Unknown";
            const char* mod_amp2 = (o.amp_mod_type[1] >= 0 && o.amp_mod_type[1] < n_mods) ? mod_types[o.amp_mod_type[1]] : "Unknown";
            const char* mod_amp3 = (o.amp_mod_type[2] >= 0 && o.amp_mod_type[2] < n_mods) ? mod_types[o.amp_mod_type[2]] : "Unknown";

            const char* mod_detune1 = (o.detune_mod_type[0] >= 0 && o.detune_mod_type[0] < n_mods) ? mod_types[o.detune_mod_type[0]] : "Unknown";
            const char* mod_detune2 = (o.detune_mod_type[1] >= 0 && o.detune_mod_type[1] < n_mods) ? mod_types[o.detune_mod_type[1]] : "Unknown";
            const char* mod_detune3 = (o.detune_mod_type[2] >= 0 && o.detune_mod_type[2] < n_mods) ? mod_types[o.detune_mod_type[2]] : "Unknown";

            const char* mod_ddtime1 = (o.ddtime_mod_type[0] >= 0 && o.ddtime_mod_type[0] < n_mods) ? mod_types[o.ddtime_mod_type[0]] : "Unknown";
            const char* mod_ddtime2 = (o.ddtime_mod_type[1] >= 0 && o.ddtime_mod_type[1] < n_mods) ? mod_types[o.ddtime_mod_type[1]] : "Unknown";
            const char* mod_ddtime3 = (o.ddtime_mod_type[2] >= 0 && o.ddtime_mod_type[2] < n_mods) ? mod_types[o.ddtime_mod_type[2]] : "Unknown";


            const char* mod_pan1 = (o.pan_mod_type[0] >= 0 && o.pan_mod_type[0] < n_mods) ? mod_types[o.pan_mod_type[0]] : "Unknown";
            const char* mod_pan2 = (o.pan_mod_type[1] >= 0 && o.pan_mod_type[1] < n_mods) ? mod_types[o.pan_mod_type[1]] : "Unknown";
            const char* mod_pan3 = (o.pan_mod_type[2] >= 0 && o.pan_mod_type[2] < n_mods) ? mod_types[o.pan_mod_type[2]] : "Unknown";
    

            (mute.renderer->form_vco[0]>2)? if_square[0]=true: if_square[0]=false;
            (mute.renderer->form_vco[1]>2)? if_square[1]=true: if_square[1]=false;
            (mute.renderer->form_vco[2]>2)? if_square[2]=true: if_square[2]=false;


            (mute.renderer->form_vco[0]>4)? if_morph[0]=true: if_morph[0]=false;
            (mute.renderer->form_vco[1]>4)? if_morph[1]=true: if_morph[1]=false;
            (mute.renderer->form_vco[2]>4)? if_morph[2]=true: if_morph[2]=false;

            (mute.renderer->form_vco[0]>4)? if_phase[0]=false: if_phase[0]=true;
            (mute.renderer->form_vco[1]>4)? if_phase[1]=false: if_phase[1]=true;
            (mute.renderer->form_vco[2]>4)? if_phase[2]=false: if_phase[2]=true;


            const char* mod_pwm1 =   (o.pwm_mod_type[0] >= 0 && o.pwm_mod_type[0] < n_mods) ? mod_types[o.pwm_mod_type[0]] : "Unknown";
            const char* mod_pwm2 =   (o.pwm_mod_type[1] >= 0 && o.pwm_mod_type[1] < n_mods) ? mod_types[o.pwm_mod_type[1]] : "Unknown";
            const char* mod_pwm3 =   (o.pwm_mod_type[2] >= 0 && o.pwm_mod_type[2] < n_mods) ? mod_types[o.pwm_mod_type[2]] : "Unknown";

            const char* mod_phase1 = (o.phase_mod_type[0] >= 0 && o.phase_mod_type[0] < n_mods) ? mod_types[o.phase_mod_type[0]] : "Unknown";
            const char* mod_phase2 = (o.phase_mod_type[1] >= 0 && o.phase_mod_type[1] < n_mods) ? mod_types[o.phase_mod_type[1]] : "Unknown";
            const char* mod_phase3 = (o.phase_mod_type[2] >= 0 && o.phase_mod_type[2] < n_mods) ? mod_types[o.phase_mod_type[2]] : "Unknown";

            const char* vcf_name1 = (mute.renderer->form_vcf[0] >= 0 && mute.renderer->form_vcf[0] < n_vcfs) ? vcf_names[mute.renderer->form_vcf[0]] : "Unknown";
            const char* vcf_name2 = (mute.renderer->form_vcf[1] >= 0 && mute.renderer->form_vcf[1] < n_vcfs) ? vcf_names[mute.renderer->form_vcf[1]] : "Unknown";
            const char* vcf_name3 = (mute.renderer->form_vcf[2] >= 0 && mute.renderer->form_vcf[2] < n_vcfs) ? vcf_names[mute.renderer->form_vcf[2]] : "Unknown";
            
            const char* mod_name1 = (o.cutoff_mod_type[0] >= 0 && o.cutoff_mod_type[0] < n_mods) ? mod_types[o.cutoff_mod_type[0]] : "Unknown";
            const char* mod_name2 = (o.cutoff_mod_type[1] >= 0 && o.cutoff_mod_type[1] < n_mods) ? mod_types[o.cutoff_mod_type[1]] : "Unknown";
            const char* mod_name3 = (o.cutoff_mod_type[2] >= 0 && o.cutoff_mod_type[2] < n_mods) ? mod_types[o.cutoff_mod_type[2]] : "Unknown";

            //const char* proc_type = (mute.renderer->wpr.mode >= 0 && mute.renderer->wpr.mode < n_proc) ? proc_names[mute.renderer->wpr.mode] : "Unknown";
         
            const char* src_l_type = (mute.renderer->dd[0].type >= 0 && mute.renderer->dd[0].type < n_dd) ? dd_names_l[mute.renderer->dd[0].type] : "Unknown";
            const char* src_c_type = (mute.renderer->dd[1].type >= 0 && mute.renderer->dd[1].type < n_dd) ? dd_names_c[mute.renderer->dd[1].type] : "Unknown";
            const char* src_r_type = (mute.renderer->dd[2].type >= 0 && mute.renderer->dd[2].type < n_dd) ? dd_names_r[mute.renderer->dd[2].type] : "Unknown";

            const char* note_alg = (mute.renderer->algorhithm[0] >= 0 && mute.renderer->algorhithm[0] < n_algs) ? algorhithms[mute.renderer->algorhithm[0]] : "Unknown";
            const char* rhtm_alg = (mute.renderer->algorhithm[1] >= 0 && mute.renderer->algorhithm[1] < n_algs) ? algorhithms[mute.renderer->algorhithm[1]] : "Unknown";

            const char* env_name1 = (o.env_form[0] >= 0 && o.env_form[0] < n_envs) ? env_names[o.env_form[0]] : "Unknown";
            const char* env_name2 = (o.env_form[1] >= 0 && o.env_form[1] < n_envs) ? env_names[o.env_form[1]] : "Unknown";
            const char* env_name3 = (o.env_form[2] >= 0 && o.env_form[2] < n_envs) ? env_names[o.env_form[2]] : "Unknown";
            const char* env_name4 = (o.env_form[3] >= 0 && o.env_form[3] < n_envs) ? env_names[o.env_form[3]] : "Unknown";

            ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            ImGui::SetNextWindowSize(ImVec2(788.0f,490.0f),0);
            ImGui::SetNextWindowPos(ImVec2(0.0f,0.0f));
            ImGui::Begin("##FF", NULL, ImGuiWindowFlags_NoResize|ImGuiWindowFlags_NoBackground|ImGuiWindowFlags_NoScrollbar|ImGuiWindowFlags_NoCollapse);    
            ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            // Child #1 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            ImGui::BeginChild(1, ImVec2(470,460), false);
            ImGui::PushItemWidth(-1);
                    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                    // VOLUME /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                    ImGui::PushStyleColor(ImGuiCol_FrameBg,ImVec4(0.129, 0.215, 0.349,1));
                    if (ImGui::SliderFloat("##Volume", &mute.renderer->volume, 0.0f, 1.0f,"Volume:   %.4f",ImGuiSliderFlags_Logarithmic)) o.volume = mute.renderer->volume;
                    ImGui::PopStyleColor();

                    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                    /// OSCILLATOR A /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                    /// WAVEFORM /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                    ImGui::BeginChild(11, ImVec2(154, 206), false);
                    ImGui::PushItemWidth(-1);
                    ImGui::PushStyleColor(ImGuiCol_SliderGrab,ImVec4(0.14,0.2,0.28,1));
                    if (ImGui::SliderInt  ("##WaveformA", &mute.renderer->form_vco[0], 0, n_waves-1, wave_name_a)) o.form_vco[0] = mute.renderer->form_vco[0];
                    ImGui::PopStyleColor();
                    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                    // AMPLITUDE A ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                    ImGui::PushItemWidth(132);
                    if(o.amp_slider_type[0]==0)
                    {
                        ImGui::SliderFloat("##AmpA", &mute.renderer->cvin[0], 0.0f, 1.00f, "Amp:      %.4f");
                    }
                    if(o.amp_slider_type[0]==1)
                    {
                        ImGui::PushStyleColor(ImGuiCol_SliderGrab, ImVec4(0.14,0.2,0.28,1));
                        ImGui::SliderInt("##ModTypeAmpA", &o.amp_mod_type[0], 0, n_mods-1, mod_amp1); 
                        ImGui::PopStyleColor();
                    }
                    if(o.amp_slider_type[0]==2)
                    {
                        ImGui::SliderFloat("##ModAmountAmpA", &o.amp_mod_amount[0], 0.0f, 1.00f,"Amount:   %.4f", ImGuiSliderFlags_Logarithmic);
                    }
                    ImGui::SameLine();
                    if( ImGui::Button("###SwitchAmpA", ImVec2(17,17)) ) { o.amp_slider_type[0]++; o.amp_slider_type[0]>2? o.amp_slider_type[0]=0 : o.amp_slider_type[0]; }
                    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                    // Detune A ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                    if(o.detune_slider_type[0]==0)
                    {
                        ImGui::SliderFloat("##DetuneA", &mute.renderer->cvin[17], 0.0f, 1.0f, "Detune:   %.4f");
                    }
                    if(o.detune_slider_type[0]==1)
                    {
                        ImGui::PushStyleColor(ImGuiCol_SliderGrab, ImVec4(0.14,0.2,0.28,1));
                        ImGui::SliderInt("##ModTypeDetuneA", &o.detune_mod_type[0], 0, n_mods-1, mod_detune1); 
                        ImGui::PopStyleColor();
                    }
                    if(o.detune_slider_type[0]==2)
                    {
                        ImGui::SliderFloat("##ModAmountDetuneA", &o.detune_mod_amount[0], 0.0f, 1.00f,"Amount:   %.4f", ImGuiSliderFlags_Logarithmic);
                    }
                    ImGui::SameLine();
                    if( ImGui::Button("###SwitchDetuneA", ImVec2(17,17)) ) { o.detune_slider_type[0]++; o.detune_slider_type[0]>2? o.detune_slider_type[0]=0 : o.detune_slider_type[0]; }
                    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                    // PAN A //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                    ImGui::PushItemWidth(132);
                    if(o.pan_slider_type[0]==0)
                    {
                        ImGui::SliderFloat("##PanA", &mute.renderer->cvin[14], 0.0f, 1.00f, "Pan:      %.4f");
                    }
                    if(o.pan_slider_type[0]==1)
                    {
                        ImGui::PushStyleColor(ImGuiCol_SliderGrab, ImVec4(0.14,0.2,0.28,1));
                        ImGui::SliderInt("##ModTypePanA", &o.pan_mod_type[0], 0, n_mods-1, mod_pan1); 
                        ImGui::PopStyleColor();
                    }
                    if(o.pan_slider_type[0]==2)
                    {
                        ImGui::SliderFloat("##ModAmountPanA", &o.pan_mod_amount[0], 0.0f, 1.00f,"Amount:   %.4f", ImGuiSliderFlags_Logarithmic);
                    }
                    ImGui::SameLine();
                    if( ImGui::Button("###SwitchPanA", ImVec2(17,17)) ) { o.pan_slider_type[0]++; o.pan_slider_type[0]>2? o.pan_slider_type[0]=0 : o.pan_slider_type[0]; }


                    ImGui::PushItemWidth(-1);
                    ImGui::PushStyleColor(ImGuiCol_SliderGrab,ImVec4(0.14,0.2,0.28,1));
                    ImGui::SliderInt("##OctaveA",&mute.renderer->octave[0] ,1,9,"Octave:        %d   ");
                    ImGui::PopStyleColor();
                    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                    // Phase A ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                    ImGui::PushItemWidth(132);
                    if(o.phase_slider_type[0]==0)
                    {
                        if(if_phase[0])
                        ImGui::SliderFloat("##PhaseA", &mute.renderer->cvin[6], 0.0f, 1.0f, "Phase:    %.4f");
                        else
                        ImGui::SliderFloat("##PhaseA", &mute.renderer->cvin[6], 0.0f, 1.0f, "Warp:     %.4f");
                        ImGui::SameLine();
                    }
                    if(o.phase_slider_type[0]==1)
                    {
                        ImGui::PushStyleColor(ImGuiCol_SliderGrab,ImVec4(0.14,0.2,0.28,1));
                        ImGui::SliderInt("##ModTypePhaseA", &o.phase_mod_type[0], 0, n_mods-1, mod_phase1); 
                        ImGui::PopStyleColor();
                        ImGui::SameLine();
                    }
                    if(o.phase_slider_type[0]==2)
                    {
                        ImGui::SliderFloat("##ModAmountPhaseA", &o.phase_mod_amount[0], 0.0f, 1.00f,"Amount:   %.4f",ImGuiSliderFlags_Logarithmic);    
                        ImGui::SameLine();
                    }
                    if( ImGui::Button("###SwitchPhaseA", ImVec2(17,17)) ) { o.phase_slider_type[0]++; o.phase_slider_type[0]>2? o.phase_slider_type[0]=0 : o.phase_slider_type[0]; }


                    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                    // PWM A ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                    ImGui::PushItemWidth(132);
                    ImGui::BeginDisabled(!if_square[0]);
                    if(o.pwm_slider_type[0]==0)
                    {
                        if (if_morph[0]) 
                        ImGui::SliderFloat("##PwmA", &mute.renderer->cvin[3], 0.00f, 1.0f,"Morph:    %.4f");
                        else
                        ImGui::SliderFloat("##PwmA", &mute.renderer->cvin[3], 0.00f, 1.0f,"PWM:      %.4f");
                        ImGui::SameLine();
                    }
                    if(o.pwm_slider_type[0]==1)
                    {
                        ImGui::PushStyleColor(ImGuiCol_SliderGrab,ImVec4(0.14,0.2,0.28,1));
                        ImGui::SliderInt("##ModTypePwmA", &o.pwm_mod_type[0], 0, n_mods-1, mod_pwm1); 
                        ImGui::PopStyleColor();
                        ImGui::SameLine();
                    }
                    if(o.pwm_slider_type[0]==2)
                    {
                        ImGui::SliderFloat("##ModAmountPwmA", &o.pwm_mod_amount[0], 0.0f, 1.00f,"Amount:   %.4f",ImGuiSliderFlags_Logarithmic);
                        ImGui::SameLine();
                    }
                    if( ImGui::Button("###SwitchPwmA", ImVec2(17,17)) ) { o.pwm_slider_type[0]++; o.pwm_slider_type[0]>2? o.pwm_slider_type[0]=0 : o.pwm_slider_type[0]; }
                    ImGui::EndDisabled();
                    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                    /// CUTOFF A WAVEFORM ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                    ImGui::PushItemWidth(-1);
                    ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(38,64,90,255));
                    ImGui::PushStyleColor(ImGuiCol_SliderGrab, IM_COL32(38,64,90,255));
                    ImGui::SliderInt  ("##VcfA", &mute.renderer->form_vcf[0], 0, n_vcfs-1, vcf_name1); 
                    ImGui::PopStyleColor();
                    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                    /// CUTOFF A ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
                    /// Cutoff range in Hz: 27.5 - 9956.06 //////////////////////////////////////////////////////////////////////////////////////
                    ImGui::PushItemWidth(132);
                    switch(o.cuttoff_slider_type[0]){
                        case 0:
                            if(ImGui::SliderFloat("##CutoffA", &o.cutoff[0], 0.0f, 1.0f, "Cutoff:   %.4f"))
                            {
                                mute.renderer->cvin[9] = o.cutoff[0];
                            }
                            break;
                        case 1:
                            ImGui::PushStyleColor(ImGuiCol_SliderGrab, ImVec4(0.14,0.2,0.28,1));
                            ImGui::SliderInt("##CutoffModTypeA", &o.cutoff_mod_type[0], 0, n_mods-1, mod_name1); 
                            ImGui::PopStyleColor(); break;
                        case 2:
                            ImGui::SliderFloat("##CutoffModAmountA", &o.cutoff_mod_amount[0], 0.0f, 1.00f, "Amount:   %.4f", ImGuiSliderFlags_Logarithmic); break;
                        default: break;
                    }
                    ImGui::SameLine();
                    if(ImGui::Button("###CutoffSwitchA", ImVec2(17,17))) { o.cuttoff_slider_type[0]++; o.cuttoff_slider_type[0]>2? o.cuttoff_slider_type[0]=0: o.cuttoff_slider_type[0]; }
                    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                    /// RESONANCE A ////////////////////////////////////////////////////////////////////////////////////////////////////////////
                    ImGui::PushItemWidth(-1);
                    ImGui::SliderFloat("##ResonanceA", &mute.renderer->vcf[0].Q, 0.0f, 1.00f, "Q:        %.4f   ");
                    ImGui::PopStyleColor();
                    ImGui::SameLine();
                    ImGui::EndChild();
                    // End of Oscillator A ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


                    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                    /// OSCILLATOR B /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                    /// WAVEFORM /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                    ImGui::SameLine();
                    ImGui::BeginChild(12, ImVec2(154, 206), false);
                    ImGui::PushItemWidth(-1);
                    ImGui::PushStyleColor(ImGuiCol_SliderGrab,ImVec4(0.14,0.2,0.28,1));
                    if (ImGui::SliderInt  ("##WaveformB", &mute.renderer->form_vco[1], 0, n_waves-1, wave_name_b)) o.form_vco[1] = mute.renderer->form_vco[1];
                    ImGui::PopStyleColor();
                    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                    /// AMPLITUDE B ///////////////////////////////////////////////////////////////////////////////////////////////////////////
                    ImGui::PushItemWidth(132);
                    if(o.amp_slider_type[1]==0)
                    {
                        ImGui::SliderFloat("##AmpB", &mute.renderer->cvin[1], 0.0f, 1.00f, "Amp:      %.4f");
                    }
                    if(o.amp_slider_type[1]==1)
                    {
                        ImGui::PushStyleColor(ImGuiCol_SliderGrab, ImVec4(0.14,0.2,0.28,1));
                        ImGui::SliderInt("##ModTypeAmpB", &o.amp_mod_type[1], 0, n_mods-1, mod_amp2); 
                        ImGui::PopStyleColor();
                    }
                    if(o.amp_slider_type[1]==2)
                    {
                        ImGui::SliderFloat("##ModAmountAmpB", &o.amp_mod_amount[1], 0.0f, 1.00f,"Amount:   %.4f",ImGuiSliderFlags_Logarithmic);
                    }
                    ImGui::SameLine();
                    if( ImGui::Button("###AmpSwitchB",ImVec2(17,17)) ) { o.amp_slider_type[1]++; o.amp_slider_type[1]>2? o.amp_slider_type[1]=0 : o.amp_slider_type[1]; }
                    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                    // Detune B ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                    if(o.detune_slider_type[1]==0)
                    {
                        ImGui::SliderFloat("##DetuneB", &mute.renderer->cvin[18], 0.0f, 1.0f, "Detune:   %.4f");
                    }
                    if(o.detune_slider_type[1]==1)
                    {
                        ImGui::PushStyleColor(ImGuiCol_SliderGrab, ImVec4(0.14,0.2,0.28,1));
                        ImGui::SliderInt("##ModTypeDetuneB", &o.detune_mod_type[1], 0, n_mods-1, mod_detune2); 
                        ImGui::PopStyleColor();
                    }
                    if(o.detune_slider_type[1]==2)
                    {
                        ImGui::SliderFloat("##ModAmountDetuneB", &o.detune_mod_amount[1], 0.0f, 1.00f,"Amount:   %.4f", ImGuiSliderFlags_Logarithmic);
                    }
                    ImGui::SameLine();
                    if( ImGui::Button("###SwitchDetuneB", ImVec2(17,17)) ) { o.detune_slider_type[1]++; o.detune_slider_type[1]>2? o.detune_slider_type[1]=0 : o.detune_slider_type[1]; }
                  
                    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                    // PAN B //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                    ImGui::PushItemWidth(132);
                    if(o.pan_slider_type[1]==0)
                    {
                        ImGui::SliderFloat("##PanB", &mute.renderer->cvin[15], 0.0f, 1.00f, "Pan:      %.4f");
                    }
                    if(o.pan_slider_type[1]==1)
                    {
                        ImGui::PushStyleColor(ImGuiCol_SliderGrab, ImVec4(0.14,0.2,0.28,1));
                        ImGui::SliderInt("##ModTypePanB", &o.pan_mod_type[1], 0, n_mods-1, mod_pan2); 
                        ImGui::PopStyleColor();
                    }
                    if(o.pan_slider_type[1]==2)
                    {
                        ImGui::SliderFloat("##ModAmountPanB", &o.pan_mod_amount[1], 0.0f, 1.00f,"Amount:   %.4f", ImGuiSliderFlags_Logarithmic);
                    }
                    ImGui::SameLine();
                    if( ImGui::Button("###SwitchPanB", ImVec2(17,17)) ) { o.pan_slider_type[1]++; o.pan_slider_type[1]>2? o.pan_slider_type[1]=0 : o.pan_slider_type[1]; }


                    ImGui::PushItemWidth(-1);
                    ImGui::PushStyleColor(ImGuiCol_SliderGrab,ImVec4(0.14,0.2,0.28,1));
                    ImGui::SliderInt("##OctaveB",&mute.renderer->octave[1],0,9,"Octave:        %d   ");
                    ImGui::PopStyleColor();
                    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                    /// Phase B /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                    ImGui::PushItemWidth(132);
                    if(o.phase_slider_type[1]==0)
                    {
                        if(if_phase[1])
                        ImGui::SliderFloat("##PhaseB",&mute.renderer->cvin[7], 0.0f, 1.00f, "Phase:    %.4f");
                        else
                        ImGui::SliderFloat("##PhaseB",&mute.renderer->cvin[7], 0.0f, 1.00f, "Warp:     %.4f");
                        ImGui::SameLine();
                    }
                    if(o.phase_slider_type[1]==1)
                    {
                        ImGui::PushStyleColor(ImGuiCol_SliderGrab, ImVec4(0.14,0.2,0.28,1));
                        ImGui::SliderInt("##ModTypePhaseB", &o.phase_mod_type[1], 0, n_mods-1, mod_phase2); 
                        ImGui::PopStyleColor();
                        ImGui::SameLine();
                    }
                    if(o.phase_slider_type[1]==2)
                    {
                        ImGui::SliderFloat("##ModAmountPhaseB", &o.phase_mod_amount[1], 0.0f, 1.00f, "Amount:   %.4f", ImGuiSliderFlags_Logarithmic);
                        ImGui::SameLine();
                    }
                    if( ImGui::Button("###SwitchPhaseB", ImVec2(17,17)) ) { o.phase_slider_type[1]++; o.phase_slider_type[1]>2? o.phase_slider_type[1]=0 : o.phase_slider_type[1]; }                 
                    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                    /// PWM B /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                    ImGui::PushItemWidth(132);
                    ImGui::BeginDisabled(!if_square[1]);
                    if(o.pwm_slider_type[1]==0)
                    {
                        if (if_morph[1])
                        ImGui::SliderFloat("##PwmB", &mute.renderer->cvin[4], 0.0f, 1.0f, "Morph:    %.4f");                 
                        else
                        ImGui::SliderFloat("##PwmB", &mute.renderer->cvin[4], 0.0f, 1.0f, "PWM:      %.4f");                 
                        ImGui::SameLine();
                    }
                    if(o.pwm_slider_type[1]==1)
                    {
                        ImGui::PushStyleColor(ImGuiCol_SliderGrab, ImVec4(0.14,0.2,0.28,1));
                        ImGui::SliderInt("##ModTypePwmB", &o.pwm_mod_type[1], 0, n_mods-1, mod_pwm2); 
                        ImGui::PopStyleColor();
                        ImGui::SameLine();
                    }
                    if(o.pwm_slider_type[1]==2)
                    {
                        ImGui::SliderFloat("##ModAmountPwmB", &o.pwm_mod_amount[1], 0.0f, 1.00f, "Amount:   %.4f", ImGuiSliderFlags_Logarithmic);
                        ImGui::SameLine();
                    }
                    if( ImGui::Button("###SwitchPwmB", ImVec2(17,17)) ) { o.pwm_slider_type[1]++; o.pwm_slider_type[1]>2? o.pwm_slider_type[1]=0 : o.pwm_slider_type[1]; }                 
                    ImGui::EndDisabled();
                    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                    /// CUTOFF B WAVEFORM ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                    ImGui::PushItemWidth(-1);
                    ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(38,64,90,255));
                    ImGui::PushStyleColor(ImGuiCol_SliderGrab, IM_COL32(38,64,90,255));
                    ImGui::SliderInt  ("##VcfB", &mute.renderer->form_vcf[1], 0, n_vcfs-1, vcf_name2); 
                    ImGui::PopStyleColor();
                    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                    /// CUTOFF B ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
                    ImGui::PushItemWidth(132);
                    switch(o.cuttoff_slider_type[1]){
                        case 0:
                            if(ImGui::SliderFloat("##CutoffB", &o.cutoff[1], 0.0f, 1.0f, "Cutoff:   %.4f"))
                            {
                                mute.renderer->cvin[10] = o.cutoff[1];
                            }
                            break;
                        case 1:
                            ImGui::PushStyleColor(ImGuiCol_SliderGrab, ImVec4(0.14,0.2,0.28,1));
                            ImGui::SliderInt("##CutoffModTypeB", &o.cutoff_mod_type[1], 0, n_mods-1, mod_name2); 
                            ImGui::PopStyleColor(); break;
                        case 2:
                            ImGui::SliderFloat("##CutoffModAmountB", &o.cutoff_mod_amount[1], 0.0f, 1.00f,"Amount:   %.4f",ImGuiSliderFlags_Logarithmic); break;
                        default: break;
                    }
                    ImGui::SameLine();
                    if(ImGui::Button("###CuttoffSwitchB", ImVec2(17,17))) { o.cuttoff_slider_type[1]++; o.cuttoff_slider_type[1]>2? o.cuttoff_slider_type[1]=0: o.cuttoff_slider_type[1]; }
                    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                    /// RESONANCE B ////////////////////////////////////////////////////////////////////////////////////////////////////////////
                    ImGui::PushItemWidth(-1);
                    ImGui::SliderFloat("##ResonanceB", &mute.renderer->vcf[1].Q, 0.0f, 1.00f, "Q:        %.4f   ");
                    ImGui::PopStyleColor();
                    ImGui::PopItemWidth();
                    ImGui::EndChild();
                    // End of Oscillator B ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 


                    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                    /// OSCILLATOR C /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                    /// WAVEFORM /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                    ImGui::SameLine();
                    ImGui::BeginChild(13, ImVec2(154, 206), false);
                    ImGui::PushItemWidth(-1);
                    ImGui::PushStyleColor(ImGuiCol_SliderGrab,ImVec4(0.14,0.2,0.28,1));
                    if (ImGui::SliderInt  ("##WaveformC", &mute.renderer->form_vco[2], 0, n_waves-1, wave_name_c)) o.form_vco[2] = mute.renderer->form_vco[2];
                    ImGui::PopStyleColor();
                    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                    // AMPLITUDE C ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                    ImGui::PushItemWidth(132);
                    if(o.amp_slider_type[2]==0)
                    {
                        ImGui::SliderFloat("##AmpC", &mute.renderer->cvin[2], 0.0f, 1.00f, "Amp:      %.4f");
                    }
                    if(o.amp_slider_type[2]==1)
                    {
                        ImGui::PushStyleColor(ImGuiCol_SliderGrab, ImVec4(0.14,0.2,0.28,1));
                        ImGui::SliderInt("##ModTypeAmpC", &o.amp_mod_type[2], 0, n_mods-1, mod_amp3); 
                        ImGui::PopStyleColor();
                    }
                    if(o.amp_slider_type[2]==2)
                    {
                        ImGui::SliderFloat("##ModAmountAmpC", &o.amp_mod_amount[2], 0.0f, 1.00f,"Amount:   %.4f", ImGuiSliderFlags_Logarithmic);
                    }
                    ImGui::SameLine();
                    if( ImGui::Button("###SwitchAmpC", ImVec2(17,17)) ) { o.amp_slider_type[2]++; o.amp_slider_type[2]>2? o.amp_slider_type[2]=0 : o.amp_slider_type[2]; }
                    
                    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                    // Detune C ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                    if(o.detune_slider_type[2]==0)
                    {
                        ImGui::SliderFloat("##DetuneC", &mute.renderer->cvin[19], 0.0f, 1.0f, "Detune:   %.4f");
                    }
                    if(o.detune_slider_type[2]==1)
                    {
                        ImGui::PushStyleColor(ImGuiCol_SliderGrab, ImVec4(0.14,0.2,0.28,1));
                        ImGui::SliderInt("##ModTypeDetuneC", &o.detune_mod_type[2], 0, n_mods-1, mod_detune3); 
                        ImGui::PopStyleColor();
                    }
                    if(o.detune_slider_type[2]==2)
                    {
                        ImGui::SliderFloat("##ModAmountDetuneC", &o.detune_mod_amount[2], 0.0f, 1.00f,"Amount:   %.4f", ImGuiSliderFlags_Logarithmic);
                    }
                    ImGui::SameLine();
                    if( ImGui::Button("###SwitchDetuneC", ImVec2(17,17)) ) { o.detune_slider_type[2]++; o.detune_slider_type[2]>2? o.detune_slider_type[2]=0 : o.detune_slider_type[2]; }
                  
                    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                    // PAN C //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                    ImGui::PushItemWidth(132);
                    if(o.pan_slider_type[2]==0)
                    {
                        ImGui::SliderFloat("##PanC", &mute.renderer->cvin[16], 0.0f, 1.00f, "Pan:      %.4f");
                    }
                    if(o.pan_slider_type[2]==1)
                    {
                        ImGui::PushStyleColor(ImGuiCol_SliderGrab, ImVec4(0.14,0.2,0.28,1));
                        ImGui::SliderInt("##ModTypePanC", &o.pan_mod_type[2], 0, n_mods-1, mod_pan3); 
                        ImGui::PopStyleColor();
                    }
                    if(o.pan_slider_type[2]==2)
                    {
                        ImGui::SliderFloat("##ModAmountPanC", &o.pan_mod_amount[2], 0.0f, 1.00f,"Amount:   %.4f", ImGuiSliderFlags_Logarithmic);
                    }
                    ImGui::SameLine();
                    if( ImGui::Button("###SwitchPanC", ImVec2(17,17)) ) { o.pan_slider_type[2]++; o.pan_slider_type[2]>2? o.pan_slider_type[2]=0 : o.pan_slider_type[2]; }

             
                    ImGui::PushItemWidth(-1);
                    ImGui::PushStyleColor(ImGuiCol_SliderGrab,ImVec4(0.14,0.2,0.28,1));
                    ImGui::SliderInt("##OctaveC",&mute.renderer->octave[2],0,9,"Octave:        %d   ");
                    ImGui::PopStyleColor();
                    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                    // Phase C ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                    ImGui::PushItemWidth(132);
                    if(o.phase_slider_type[2]==0)
                    {
                        if(if_phase[2])
                        ImGui::SliderFloat("##PhaseC", &mute.renderer->cvin[8], 0.0f, 1.0f, "Phase:    %.4f");
                        else
                        ImGui::SliderFloat("##PhaseC", &mute.renderer->cvin[8], 0.0f, 1.0f, "Warp:     %.4f");
                        ImGui::SameLine();
                    }
                    if(o.phase_slider_type[2]==1)
                    {
                        ImGui::PushStyleColor(ImGuiCol_SliderGrab,ImVec4(0.14,0.2,0.28,1));
                        ImGui::SliderInt("##ModTypePhaseC", &o.phase_mod_type[2], 0, n_mods-1, mod_phase3); 
                        ImGui::PopStyleColor();
                        ImGui::SameLine();
                    }
                    if(o.phase_slider_type[2]==2)
                    {
                        ImGui::SliderFloat("##ModAmountPhaseC", &o.phase_mod_amount[2], 0.0f, 1.00f,"Amount:   %.4f",ImGuiSliderFlags_Logarithmic);
                        ImGui::SameLine();
                    }
                    if( ImGui::Button("###SwitchPhaseC", ImVec2(17,17)) ) { o.phase_slider_type[2]++; o.phase_slider_type[2]>2? o.phase_slider_type[2]=0 : o.phase_slider_type[2]; }
                    
                    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                    // PWM C ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                    ImGui::PushItemWidth(132);
                    ImGui::BeginDisabled(!if_square[2]);
                    if(o.pwm_slider_type[2]==0)
                    {
                        if (if_morph[2])
                        ImGui::SliderFloat("##PwmC", &mute.renderer->cvin[5], 0.0f, 1.0f,"Morph:    %.4f");
                        else
                        ImGui::SliderFloat("##PwmC", &mute.renderer->cvin[5], 0.0f, 1.0f,"PWM:      %.4f");
                        ImGui::SameLine();
                    }
                    if(o.pwm_slider_type[2]==1)
                    {
                        ImGui::PushStyleColor(ImGuiCol_SliderGrab,ImVec4(0.14,0.2,0.28,1));
                        ImGui::SliderInt("##ModTypePwmC", &o.pwm_mod_type[2], 0, n_mods-1, mod_pwm3); 
                        ImGui::PopStyleColor();
                        ImGui::SameLine();
                    }
                    if(o.pwm_slider_type[2]==2)
                    {
                        ImGui::SliderFloat("##ModAmountPwmC", &o.pwm_mod_amount[2], 0.0f, 1.00f,"Amount:   %.4f", ImGuiSliderFlags_Logarithmic);
                        ImGui::SameLine();
                    }
                    if( ImGui::Button("###SwitchPwmC", ImVec2(17,17)) ) { o.pwm_slider_type[2]++; o.pwm_slider_type[2]>2? o.pwm_slider_type[2]=0 : o.pwm_slider_type[2]; }
                    ImGui::EndDisabled();
                    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                    /// CUTOFF C WAVEFORM ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                    ImGui::PushItemWidth(-1);
                    ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(38,64,90,255));
                    ImGui::PushStyleColor(ImGuiCol_SliderGrab, IM_COL32(38,64,90,255));
                    ImGui::SliderInt  ("##VcfC", &mute.renderer->form_vcf[2], 0, n_vcfs-1, vcf_name3); 
                    ImGui::PopStyleColor();
                    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                    /// CUTOFF C ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
                    /// Cutoff range in Hz: 27.5 - 9956.06 //////////////////////////////////////////////////////////////////////////////////////
                    ImGui::PushItemWidth(132);
                    switch(o.cuttoff_slider_type[2]){
                        case 0:
                            if(ImGui::SliderFloat("##CutoffC", &o.cutoff[2], 0.0f, 1.0f, "Cutoff:   %.4f"))
                            {
                                mute.renderer->cvin[11] = o.cutoff[2];
                            }
                            break;
                        case 1:
                            ImGui::PushStyleColor(ImGuiCol_SliderGrab, ImVec4(0.14,0.2,0.28,1));
                            ImGui::SliderInt("##CutoffModTypeC", &o.cutoff_mod_type[2], 0, n_mods-1, mod_name3); 
                            ImGui::PopStyleColor(); break;
                        case 2:
                            ImGui::SliderFloat("##CutoffModAmountC", &o.cutoff_mod_amount[2], 0.0f, 1.00f, "Amount:   %.4f", ImGuiSliderFlags_Logarithmic); break;
                        default: break;
                    }
                    ImGui::SameLine();
                    if(ImGui::Button("###CutoffSwitchC", ImVec2(17,17))) { o.cuttoff_slider_type[2]++; o.cuttoff_slider_type[2]>2? o.cuttoff_slider_type[2]=0: o.cuttoff_slider_type[2]; }
                    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                    /// RESONANCE C ////////////////////////////////////////////////////////////////////////////////////////////////////////////
                    ImGui::PushItemWidth(-1);
                    ImGui::SliderFloat("##ResonanceC", &mute.renderer->vcf[2].Q, 0.0f, 1.00f, "Q:        %.4f   ");
                    ImGui::PopStyleColor();
                    ImGui::SameLine();
                    ImGui::EndChild();
                    // End of Oscillator C ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

                    o.cutoff[0] = mute.renderer->cvin[9];
                    o.cutoff[1] = mute.renderer->cvin[10];
                    o.cutoff[2] = mute.renderer->cvin[11];
                    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                    /// Octave & Seed ///////////////////////////////////////////////////////////////////////////////////////////////////////////
                    
                    ImGui::BeginChild(21, ImVec2(312, 228), false);
                    ImGui::PushItemWidth(-1);
                    ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.129, 0.215, 0.349,1));
                    ImGui::PushStyleColor(ImGuiCol_SliderGrab, IM_COL32(33, 55, 89, 255));
                    ImGui::PushItemWidth(154);
                    if (ImGui::SliderInt  ("##Octave", &o.octaves, 1, 9, "Range:         %d   ")) mute.renderer->esq.octaves=o.octaves;
                    ImGui::SameLine();
                    ImGui::SliderFloat("##SeedStep", &mute.renderer->seed_step[0], 0.0f, 0.1f, "Step:     %.4f   ", ImGuiSliderFlags_Logarithmic);
                    ImGui::PushItemWidth(112);
                    ImGui::PopStyleColor();
                    
                    if (ImGui::SliderFloat("##Seed", &o.seed, 0.0001f, 1.0f, "Seed:  %.4f"))
                    {                 
                        mute.renderer->esq.seed[0] = int(o.seed*10000.0f);         
                        mute.renderer->esq.regenerate(mute.renderer->algorhithm[0]);
                    }
                    //std::cout<<mute.renderer->esq.seed<<"\n";
                    if (mute.renderer->esq.seed[0] > 10000)
                    {
                        seed_up = false;
                        seed_down = true;
                        mute.renderer->esq.seed[0] = 10000;
                    }
                    if (mute.renderer->esq.seed[0] < 1)
                    {
                        seed_up = true;
                        seed_down = false;
                        mute.renderer->esq.seed[0] = 1;
                    }
                    
                    o.seed = float(mute.renderer->esq.seed[0])/10000.0f;
                    ImGui::PushStyleColor(ImGuiCol_SliderGrab, IM_COL32(33, 55, 89, 255));
                    ImGui::SameLine();
                    if(seed_up)
                    {
                        ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(61, 133, 224, 255));
                        if (ImGui::ArrowButton("##SeedUp", ImGuiDir_Up))
                        {
                            seed_up = false;
                        }
                        ImGui::PopStyleColor();

                        ImGui::SameLine();
                        if (ImGui::ArrowButton("##SeedDown", ImGuiDir_Down))
                        {
                            seed_up = false;
                            seed_down = true;
                        }

                        mute.renderer->regenerate[0] = 1;

                    }
                    else if(seed_down)
                    {
                        if (ImGui::ArrowButton("##SeedUp", ImGuiDir_Up))
                        {
                            seed_up = true;
                            seed_down = false;
                        }

                        ImGui::SameLine();
                        ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(61, 133, 224, 255));
                        if (ImGui::ArrowButton("##SeedDown", ImGuiDir_Down))
                        {
                            seed_down = false;
                        }
                        ImGui::PopStyleColor();
                        mute.renderer->regenerate[0] = -1;

                    }
                    else
                    {
                        if (ImGui::ArrowButton("##SeedUp", ImGuiDir_Up))
                        {
                            seed_up = true;
                        }

                        ImGui::SameLine();
                        if (ImGui::ArrowButton("##SeedDown", ImGuiDir_Down))
                        {
                            seed_down = true;
                        }
                        mute.renderer->regenerate[0] = 0;
                    }

                    ImGui::SameLine();

                    ImGui::PushItemWidth(154);
                    
                    ImGui::SliderInt("##NoteAlgorhithm", &mute.renderer->algorhithm[0], 0, n_algs-1, note_alg);
                    ImGui::PopStyleColor();
                    ImGui::PopStyleColor();
                    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                        
                        if(ImGui::VSliderInt("##oan", ImVec2(17, 80), &o.oan, 1, 8, ""))  {o.oct_set[0]  = o.oan; }  ImGui::SameLine();
                        if(ImGui::VSliderInt("##oas", ImVec2(17, 80), &o.oas, 1, 8, ""))  {o.oct_set[1]  = o.oas; }  ImGui::SameLine();
                        if(ImGui::VSliderInt("##obn", ImVec2(17, 80), &o.obn, 1, 8, ""))  {o.oct_set[2]  = o.obn; }  ImGui::SameLine();
                        if(ImGui::VSliderInt("##ocn", ImVec2(17, 80), &o.ocn, 1, 8, ""))  {o.oct_set[3]  = o.ocn; }  ImGui::SameLine();
                        if(ImGui::VSliderInt("##ocs", ImVec2(17, 80), &o.ocs, 1, 8, ""))  {o.oct_set[4]  = o.ocs; }  ImGui::SameLine();
                        if(ImGui::VSliderInt("##odn", ImVec2(17, 80), &o.odn, 1, 8, ""))  {o.oct_set[5]  = o.odn; }  ImGui::SameLine();
                        if(ImGui::VSliderInt("##ods", ImVec2(17, 80), &o.ods, 1, 8, ""))  {o.oct_set[6]  = o.ods; }  ImGui::SameLine();
                        if(ImGui::VSliderInt("##oen", ImVec2(17, 80), &o.oen, 1, 8, ""))  {o.oct_set[7]  = o.oen; }  ImGui::SameLine();
                        if(ImGui::VSliderInt("##ofn", ImVec2(17, 80), &o.ofn, 1, 8, ""))  {o.oct_set[8]  = o.ofn; }  ImGui::SameLine();
                        if(ImGui::VSliderInt("##ofd", ImVec2(17, 80), &o.ofs, 1, 8, ""))  {o.oct_set[9]  = o.ofs; }  ImGui::SameLine();
                        if(ImGui::VSliderInt("##ogn", ImVec2(17, 80), &o.ogn, 1, 8, ""))  {o.oct_set[10] = o.ogn; }  ImGui::SameLine();
                        if(ImGui::VSliderInt("##ogs", ImVec2(17, 80), &o.ogs, 1, 8, ""))  {o.oct_set[11] = o.ogs; }
                        ImGui::SameLine();
                        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                        ImGui::BeginChild(3, ImVec2(60, 80), false);
   
                        if (ImGui::Button("SET", ImVec2(60,38))) mute.renderer->esq.set((get_scale(o.note_set, o.oct_set)));
                        ImGui::Button("##SET", ImVec2(60,17));
                        if(ImGui::Button("##BITSET", ImVec2(60,17)))
                        {
                            std::cout<<B<<"\n";
                        }
                        
                        //ImGui::Button("##SET", ImVec2(60,17));
                        ImGui::EndChild();


                        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                        ImGui::PushItemWidth(10);

                        if(ImGui::Checkbox("##nan", &o.nan))  {if((o.nan+o.nas+o.nbn+o.ncn+o.ncs+o.ndn+o.nds+o.nen+o.nfn+o.nfs+o.ngn+o.ngs)==0) o.nan = true;  o.note_set[0]  = o.nan; }   ImGui::SameLine();
                        if(ImGui::Checkbox("##nas", &o.nas))  {if((o.nan+o.nas+o.nbn+o.ncn+o.ncs+o.ndn+o.nds+o.nen+o.nfn+o.nfs+o.ngn+o.ngs)==0) o.nas = true;  o.note_set[1]  = o.nas; }   ImGui::SameLine();
                        if(ImGui::Checkbox("##nbn", &o.nbn))  {if((o.nan+o.nas+o.nbn+o.ncn+o.ncs+o.ndn+o.nds+o.nen+o.nfn+o.nfs+o.ngn+o.ngs)==0) o.nbn = true;  o.note_set[2]  = o.nbn; }   ImGui::SameLine();
                        if(ImGui::Checkbox("##ncn", &o.ncn))  {if((o.nan+o.nas+o.nbn+o.ncn+o.ncs+o.ndn+o.nds+o.nen+o.nfn+o.nfs+o.ngn+o.ngs)==0) o.ncn = true;  o.note_set[3]  = o.ncn; }   ImGui::SameLine();
                        if(ImGui::Checkbox("##ncs", &o.ncs))  {if((o.nan+o.nas+o.nbn+o.ncn+o.ncs+o.ndn+o.nds+o.nen+o.nfn+o.nfs+o.ngn+o.ngs)==0) o.ncs = true;  o.note_set[4]  = o.ncs; }   ImGui::SameLine();
                        if(ImGui::Checkbox("##ndn", &o.ndn))  {if((o.nan+o.nas+o.nbn+o.ncn+o.ncs+o.ndn+o.nds+o.nen+o.nfn+o.nfs+o.ngn+o.ngs)==0) o.ndn = true;  o.note_set[5]  = o.ndn; }   ImGui::SameLine();
                        if(ImGui::Checkbox("##nds", &o.nds))  {if((o.nan+o.nas+o.nbn+o.ncn+o.ncs+o.ndn+o.nds+o.nen+o.nfn+o.nfs+o.ngn+o.ngs)==0) o.nds = true;  o.note_set[6]  = o.nds; }   ImGui::SameLine();
                        if(ImGui::Checkbox("##nen", &o.nen))  {if((o.nan+o.nas+o.nbn+o.ncn+o.ncs+o.ndn+o.nds+o.nen+o.nfn+o.nfs+o.ngn+o.ngs)==0) o.nen = true;  o.note_set[7]  = o.nen; }   ImGui::SameLine();
                        if(ImGui::Checkbox("##nfn", &o.nfn))  {if((o.nan+o.nas+o.nbn+o.ncn+o.ncs+o.ndn+o.nds+o.nen+o.nfn+o.nfs+o.ngn+o.ngs)==0) o.nfn = true;  o.note_set[8]  = o.nfn; }   ImGui::SameLine();
                        if(ImGui::Checkbox("##nfs", &o.nfs))  {if((o.nan+o.nas+o.nbn+o.ncn+o.ncs+o.ndn+o.nds+o.nen+o.nfn+o.nfs+o.ngn+o.ngs)==0) o.nfs = true;  o.note_set[9]  = o.nfs; }   ImGui::SameLine();
                        if(ImGui::Checkbox("##ngn", &o.ngn))  {if((o.nan+o.nas+o.nbn+o.ncn+o.ncs+o.ndn+o.nds+o.nen+o.nfn+o.nfs+o.ngn+o.ngs)==0) o.ngn = true;  o.note_set[10] = o.ngn; }   ImGui::SameLine();
                        if(ImGui::Checkbox("##ngs", &o.ngs))  {if((o.nan+o.nas+o.nbn+o.ncn+o.ncs+o.ndn+o.nds+o.nen+o.nfn+o.nfs+o.ngn+o.ngs)==0) o.ngs = true;  o.note_set[11] = o.ngs; }

            
                        ImGui::SameLine();
                        ImGui::ArrowButton("##LL", ImGuiDir_Left);
                        ImGui::SameLine();

                        if(mute.renderer->trigger_sequence==true)
                        {
                            ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(61, 133, 224, 255));
                            if(ImGui::Button("##TriggerSequence", ImVec2(17,17)))
                            {
                                mute.renderer->trigger_sequence = false;
                            }
                            ImGui::PopStyleColor();
                        }
                        else
                        {
                            if(ImGui::Button("##TriggerSequence", ImVec2(17,17)))
                            {
                                mute.renderer->trigger_sequence = true;
                            }
                        }
                 
                        ImGui::SameLine();
                        ImGui::ArrowButton("##LL", ImGuiDir_Right);

                        ImGui::PushItemWidth(154);
                        ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.129, 0.215, 0.349,1));
      
                        if (ImGui::SliderFloat("##Tempo", &o.tempo, 0.0, 1.0, "Tempo:    %.4f")) 
                        {
                            mute.renderer->bpm = int((1.1f-o.tempo)*100000);
                            mute.renderer->flags[0] = 1;
                        }
              
                        ImGui::PopStyleColor();
                        

                        ImGui::SameLine();
                        ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.129, 0.215, 0.349,1));
                        ImGui::PushStyleColor(ImGuiCol_SliderGrab, IM_COL32(33, 55, 89, 255));
                        ImGui::PushItemWidth(153);
                        ImGui::SliderInt("##RhytmAlgorithm", &mute.renderer->algorhithm[1], 0, n_algs-1, rhtm_alg);
                        ImGui::PopStyleColor();
                        ImGui::PopStyleColor();
                        ImGui::PopItemWidth();

                        bool rhythm_change = false;
                        if(ImGui::VSliderInt("##Rhythm1", ImVec2(12,60), &o.rhythm[0], 0, 255, "")) rhythm_change = true;
                        ImGui::SameLine();
                        if(ImGui::VSliderInt("##Rhythm2", ImVec2(12,60), &o.rhythm[1], 0, 255, "")) rhythm_change = true;
                        ImGui::SameLine();
                        if(ImGui::VSliderInt("##Rhythm3", ImVec2(12,60), &o.rhythm[2], 0, 255, "")) rhythm_change = true;
                        ImGui::SameLine();
                        if(ImGui::VSliderInt("##Rhythm4", ImVec2(12,60), &o.rhythm[3], 0, 255, "")) rhythm_change = true;
                        ImGui::SameLine();

                        if(mute.renderer->esq.pattern!=o.beat)
                        {
                            mute.renderer->pattern = mute.renderer->esq.pattern;
                            o.beat = mute.renderer->esq.pattern;
                        }
                        if(rhythm_change)
                        {
                            o.beat =o.rhythm[0]; o.beat<<=8;
                            o.beat+=o.rhythm[1]; o.beat<<=8;
                            o.beat+=o.rhythm[2]; o.beat<<=8;
                            o.beat+=o.rhythm[3]; 
                            //if(o.beat<3) o.beat=3;
                            mute.renderer->pattern = o.beat;
                            mute.renderer->flags[0] = true;
                        }
                        B = o.beat;
                ////////////////////////////////////////////////////////////////////////////////////////
                ////////////////////////////////////////////////////////////////////////////////////////
                ////////////////////////////////////////////////////////////////////////////////////////
                ////////////////////////////////////////////////////////////////////////////////////////
                ////////////////////////////////////////////////////////////////////////////////////////
                ////////////////////////////////////////////////////////////////////////////////////////
            
                rset = int_to_vector(o.beat);
                draw_sequencer(rset,12,12,0xA86551FF);


                //--------------------------------------------------------------------------

        
                uint newset = vector_to_int(rset);
                if(o.beat != newset)
                {
                    //if(newset<3) newset=3;
                    mute.renderer->pattern = newset;
                    mute.renderer->flags[0] = 1;

                    o.beat=newset;

                    o.rhythm[0]= (o.beat&0xFF000000)>>24;
                    o.rhythm[1]= (o.beat&0x00FF0000)>>16;
                    o.rhythm[2]= (o.beat&0x0000FF00)>>8;
                    o.rhythm[3]= (o.beat&0x000000FF);
                }        
                ImGui::EndChild();

                ImGui::SameLine();

                ImGui::BeginChild(606,ImVec2(120,60), false);
                ImGui::PushItemWidth(119);
                ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.129, 0.215, 0.349,1));
                ImGui::PushStyleColor(ImGuiCol_SliderGrab, IM_COL32(33, 55, 89, 255));

                ImGui::SliderFloat("##BeatSeed", &mute.renderer->esq.seed[1], 0.0f, 1.0f, "Seed: %.4f" );
                ImGui::SliderFloat("##BeatStep", &mute.renderer->seed_step[1], 0.0f, 1.0f, "Step: %.4f" );

                ImGui::BeginChild(909, ImVec2(38,18));
                if(mute.renderer->regenerate[1] == 1)
                {
                    ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(61, 133, 224, 255));
                    if(ImGui::ArrowButton("###RhythmUp", ImGuiDir_Up))
                    {
                        mute.renderer->regenerate[1] = 0;
                    }
                    ImGui::PopStyleColor();
                    ImGui::SameLine();
                    if(ImGui::ArrowButton("###RhythmDown", ImGuiDir_Down))
                    {
                        mute.renderer->regenerate[1] = -1;
                    }
                }
                else if(mute.renderer->regenerate[1] == -1)
                {
                    if(ImGui::ArrowButton("###RhythmUp", ImGuiDir_Up)) 
                    {
                        mute.renderer->regenerate[1] = 1;
                    }
                    ImGui::SameLine();
                    ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(61, 133, 224, 255));
                    if(ImGui::ArrowButton("###RhythmDown", ImGuiDir_Down))
                    {
                        mute.renderer->regenerate[1] = 0;
                    }
                    ImGui::PopStyleColor();

                }
                else
                {
                    if(ImGui::ArrowButton("###RhythmUp", ImGuiDir_Up)) 
                    {
                        mute.renderer->regenerate[1] = 1;
                    }
                    ImGui::SameLine();
                    if(ImGui::ArrowButton("###RhythmDown", ImGuiDir_Down)) 
                    {
                        mute.renderer->regenerate[1] = -1;
                    }               
                }
                
                ImGui::EndChild();
                ImGui::SameLine();
                
    
 
                if(steps == 32)
                {
                    mute.renderer->esq.divisor = 1;
                    if(ImGui::Button("8", ImVec2(23,17))) steps = 8;
                    ImGui::SameLine();
                    if(ImGui::Button("16", ImVec2(23,17))) steps = 16;
                    ImGui::SameLine();
                    ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(61, 133, 224, 255));
                    ImGui::Button("32", ImVec2(23,17));
                    ImGui::PopStyleColor();
                }

                else if(steps == 16)
                {
                    mute.renderer->esq.divisor = 2;
                    if(ImGui::Button("8", ImVec2(23,17))) steps = 8;
                    ImGui::SameLine();
                    ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(61, 133, 224, 255));
                    ImGui::Button("16", ImVec2(23,17));
                    ImGui::PopStyleColor();
                    ImGui::SameLine();
                    if(ImGui::Button("32", ImVec2(23,17))) steps = 32;
                }

                else if(steps == 8)
                {
                    mute.renderer->esq.divisor = 4;
                    ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(61, 133, 224, 255));
                    ImGui::Button("8", ImVec2(23,17));
                    ImGui::PopStyleColor();
                    ImGui::SameLine();
                    if(ImGui::Button("16", ImVec2(23,17))) steps = 16;
                    ImGui::SameLine();
                    if(ImGui::Button("32", ImVec2(23,17))) steps = 32;
                }


                ImGui::PopStyleColor();
                ImGui::PopStyleColor();

                ImGui::EndChild();
                ImGui::EndChild();


                ////////////////////////////////////////////////////////////////////////////////////////////////////////////
                /// Waveform Processor
                        
                        ImGui::SameLine();
                        ImGui::BeginChild(808, ImVec2(154,228),false);
                        ImGui::PushItemWidth(-1);
    

                        ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.129, 0.215, 0.349,1));
                        ImGui::PushStyleColor(ImGuiCol_SliderGrab, IM_COL32(33, 55, 89, 255));


                        // ImGui::SliderInt("##ProcType", &mute.renderer->wpr.mode, 0, n_proc-1, proc_type);
                        // ImGui::SliderFloat("###Symmetry", &mute.renderer->wpr.symmetry, 0.0f, 1.0f, "Symmetry: %.4f   ");

                        // ImGui::PushItemWidth(132);
                        // ImGui::SliderInt("##SourceLeft", &mute.renderer->wpr.source[0], 0, n_dd-1, src_l_type);
                        // ImGui::SameLine();
                        // if(ImGui::Button("###Source", ImVec2(17,17))) ++current_src>2? current_src=0: current_src;
                


                     
                        ImGui::PopStyleColor();
                        ImGui::PopStyleColor();

                        ImGui::PushItemWidth(-1);
                    if(current_src==0)
                    {
                        ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.129, 0.215, 0.349,1));
                        ImGui::PushStyleColor(ImGuiCol_SliderGrab, IM_COL32(33, 55, 89, 255));
                            ImGui::PushItemWidth(132);
                            ImGui::SliderInt("##SourceLeft", &mute.renderer->dd[0].type, 0, n_dd-1, src_l_type);                           
                        ImGui::PopStyleColor();
                        ImGui::PopStyleColor();
                    }
                    if(current_src==1)
                    {
                        ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.129, 0.215, 0.349,1));
                        ImGui::PushStyleColor(ImGuiCol_SliderGrab, IM_COL32(33, 55, 89, 255));
                            ImGui::PushItemWidth(132);
                            ImGui::SliderInt("##SourceCentre", &mute.renderer->dd[1].type, 0, n_dd-1, src_c_type);
                        ImGui::PopStyleColor();
                        ImGui::PopStyleColor();
                    }
                    if(current_src==2)
                    {
                        ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.129, 0.215, 0.349,1));
                        ImGui::PushStyleColor(ImGuiCol_SliderGrab, IM_COL32(33, 55, 89, 255));
                            ImGui::PushItemWidth(132);
                            ImGui::SliderInt("##SourceRight", &mute.renderer->dd[2].type, 0, n_dd-1, src_r_type);
                        ImGui::PopStyleColor();
                        ImGui::PopStyleColor();
                    }
                    ImGui::SameLine();
                    if(ImGui::Button("###Source", ImVec2(17,17))) ++current_src>2? current_src=0: current_src;

                    if(current_src==0)
                    {
                        ImGui::PushItemWidth(-1);
                        ImGui::SliderFloat("##DDAmountA", &mute.renderer->dd[0].amount, 0.0f, 1.0f, "Mix:      %.4f   ");
                        ImGui::SliderFloat("##DDFeedbackA", &mute.renderer->dd[0].feedback, 0.0001f, 1.0f, "Feedback: %.4f   ");
                        ImGui::PushItemWidth(132);
                        if(o.delay_slider_type[0]==0)
                        {
                            ImGui::SliderFloat("##DDTimeA", &mute.renderer->cvin[20], 0.0001f, 1.0f, "Time:     %.4f", ImGuiSliderFlags_Logarithmic);
                        }
                        if(o.delay_slider_type[0]==1)
                        {
                            ImGui::PushStyleColor(ImGuiCol_SliderGrab,ImVec4(0.14,0.2,0.28,1));
                            ImGui::SliderInt("##DDTimeModTypeA", &o.ddtime_mod_type[0], 0, n_mods-1 , mod_ddtime1);
                            ImGui::PopStyleColor();
                        }
                        if(o.delay_slider_type[0]==2)
                        {
                            ImGui::SliderFloat("##DDTimeModAmountA", &o.ddtime_mod_amount[0], 0.0001f, 1.0f, "Amount:   %.4f", ImGuiSliderFlags_Logarithmic);
                        }
                        ImGui::SameLine();
                        if (ImGui::Button("##DDSliderTypeA", ImVec2(17,17)))
                        {
                            o.delay_slider_type[0]++;
                            if (o.delay_slider_type[0]>2) o.delay_slider_type[0] = 0;
                        }
                    }
                    
                    if(current_src==1)
                    {
                        ImGui::PushItemWidth(-1);
                        ImGui::SliderFloat("##DDAmountB", &mute.renderer->dd[1].amount, 0.0f, 1.0f, "Mix:      %.4f   ");
                        ImGui::SliderFloat("##DDFeedbackB", &mute.renderer->dd[1].feedback, 0.0001f, 1.0f, "Feedback: %.4f   ");
                        ImGui::PushItemWidth(132);
                        if(o.delay_slider_type[1]==0)
                        {
                            ImGui::SliderFloat("##DDTimeB", &mute.renderer->cvin[21], 0.0001f, 1.0f, "Time:     %.4f", ImGuiSliderFlags_Logarithmic);
                        }
                        if(o.delay_slider_type[1]==1)
                        {
                            ImGui::PushStyleColor(ImGuiCol_SliderGrab,ImVec4(0.14,0.2,0.28,1));
                            ImGui::SliderInt("##DDTimeModTypeB", &o.ddtime_mod_type[1], 0, n_mods-1 , mod_ddtime2);
                            ImGui::PopStyleColor();
                        }
                        if(o.delay_slider_type[1]==2)
                        {
                            ImGui::SliderFloat("##DDTimeModAmountB", &o.ddtime_mod_amount[1], 0.0001f, 1.0f, "Amount:   %.4f", ImGuiSliderFlags_Logarithmic);
                        }
                        ImGui::SameLine();
                        if (ImGui::Button("##DDSliderTypeB", ImVec2(17,17)))
                        {
                            o.delay_slider_type[1]++;
                            if (o.delay_slider_type[1]>2) o.delay_slider_type[1] = 0;
                        }                    
                    }
            
                    if(current_src==2)
                    {
                        ImGui::PushItemWidth(-1);
                        ImGui::SliderFloat("##DDAmountC", &mute.renderer->dd[2].amount, 0.0f, 1.0f, "Mix:      %.4f   ");
                        ImGui::SliderFloat("##DDFeedbackC", &mute.renderer->dd[2].feedback, 0.0001f, 1.0f, "Feedback: %.4f   ");
                        ImGui::PushItemWidth(132);
                        if(o.delay_slider_type[2]==0)
                        {
                            ImGui::SliderFloat("##DDTimeC", &mute.renderer->cvin[22], 0.0001f, 1.0f, "Time:     %.4f", ImGuiSliderFlags_Logarithmic);
                        }
                        if(o.delay_slider_type[2]==1)
                        {
                            ImGui::PushStyleColor(ImGuiCol_SliderGrab,ImVec4(0.14,0.2,0.28,1));
                            ImGui::SliderInt("##DDTimeModTypeC", &o.ddtime_mod_type[2], 0, n_mods-1 , mod_ddtime3);
                            ImGui::PopStyleColor();
                        }
                        if(o.delay_slider_type[2]==2)
                        {
                            ImGui::SliderFloat("##DDTimeModAmountC", &o.ddtime_mod_amount[2], 0.0001f, 1.0f, "Amount:   %.4f", ImGuiSliderFlags_Logarithmic);
                        }
                        ImGui::SameLine();
                        if (ImGui::Button("##DDSliderTypeC", ImVec2(17,17)))
                        {
                            o.delay_slider_type[2]++;
                            if (o.delay_slider_type[2]>2) o.delay_slider_type[2] = 0;
                        }                   
                    }



                    ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(38,64,90,255));
                    ImGui::PushItemWidth(-1);
                    ImGui::SliderFloat("##SaturateA", &mute.renderer->wpr.drive[0], 0.0f,1.0f, "Saturate: %.4f   ");
                    ImGui::SliderFloat("##GainA", &mute.renderer->wpr.gain[0], 0.0f, 1.0f,     "Gain:     %.4f   ");
                    ImGui::SliderFloat("##MixA", &mute.renderer->wpr.mix[0], 0.0f, 1.0f,     "Mix:      %.4f   ");

                    ImGui::PopStyleColor();

                    ImGui::SliderFloat("##RevTime", &mute.renderer->wpr.rv.scale , 0.0f,1.0f,    "Time:     %.4f   ");
                    ImGui::SliderFloat("##RevTone", &mute.renderer->wpr.rv.tone , 0.0f,1.0f,    "Tone:     %.4f   ");
                    ImGui::SliderFloat("##RevAmount", &mute.renderer->wpr.rv.amount, 0.0f, 1.0f,  "Amount:   %.4f   ");
                    ImGui::SliderFloat("##RevFeedA", &mute.renderer->wpr.rv.feedback, 0.0f, 1.0f,     "Mix:      %.4f   ");

                ImGui::EndChild();
                ImGui::EndChild();


                    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                    // Child #2 ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                    ImGui::SameLine();
                    ImGui::BeginChild(2, ImVec2(304, 462), false);

                    
                
                    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                    // ENVELOPES  A & B/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                    
                    if(o.radio_env_amp)
                    {

                        ImGui::VSliderFloat("##aAT1", ImVec2(12, 80), &o.env_amp_a.adsr.A.time, 100.0f,  10000.0f*o.scale_env_amp_a, "", ImGuiSliderFlags_Logarithmic); ImGui::SameLine();
                        ImGui::VSliderFloat("##aDT1", ImVec2(12, 80), &o.env_amp_a.adsr.D.time, 1000.0f, 10000.0f*o.scale_env_amp_a, "", ImGuiSliderFlags_Logarithmic); ImGui::SameLine();
                        ImGui::VSliderFloat("##aST1", ImVec2(12, 80), &o.env_amp_a.adsr.S.time, 1000.0f, 10000.0f*o.scale_env_amp_a, "", ImGuiSliderFlags_Logarithmic); ImGui::SameLine();
                        ImGui::VSliderFloat("##aRT1", ImVec2(12, 80), &o.env_amp_a.adsr.R.time, 1000.0f, 10000.0f*o.scale_env_amp_a, "", ImGuiSliderFlags_Logarithmic); ImGui::SameLine();
                    ImGui::PushStyleColor(ImGuiCol_SliderGrab, 0xB45050FF);
                        ImGui::VSliderFloat("##aAV1", ImVec2(12, 80), &o.env_amp_a.adsr.A.value, 0.00001f, 1.0f, ""); ImGui::SameLine(); 
                        ImGui::VSliderFloat("##aDV1", ImVec2(12, 80), &o.env_amp_a.adsr.D.value, 0.00001f, 1.0f, ""); ImGui::SameLine();
                        ImGui::VSliderFloat("##aSV1", ImVec2(12, 80), &o.env_amp_a.adsr.S.value, 0.00001f, 1.0f, ""); ImGui::SameLine();
                        ImGui::VSliderFloat("##aRV1", ImVec2(12, 80), &o.env_amp_a.adsr.R.value, 0.00001f, 1.0f, ""); ImGui::SameLine();
                    ImGui::PopStyleColor();

                    o.env_imprint_amp_a = imprint(&o.env_amp_a, 5, 100);
                    float (*func_adsr1)(void*, int) = plot_f::plot_adsr1;
                    ImGui::PlotLines("##plot_adsr1", func_adsr1, NULL, o.env_imprint_amp_a.size(), 0, NULL, 0.0f, 1.0f, ImVec2(172, 80));
                    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                    ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.129, 0.215, 0.349, 1));


                    ImGui::RadioButton("##aDENV1",&o.radio_env_amp,1); ImGui::SameLine();
                    ImGui::RadioButton("##aDENV2",&o.radio_env_amp,0);

                    ImGui::SameLine();
                    ImGui::PushItemWidth(237);
                    
                    if(o.env_slider_type[0]==0)
                    {
                        ImGui::DragFloat("##aENVSCALE1", &o.scale_env_amp_a, 0.1, 0.1, 100.0, "ADSR 1 Scale %.3f", 1);
                    }
                    else if(o.env_slider_type[0]==1)
                    {
                        ImGui::PushStyleColor(ImGuiCol_SliderGrab, IM_COL32(33, 55, 89, 255));
                        ImGui::SliderInt  ("##EnvformA", &o.env_form[0], 0, n_envs-1, env_name1);
                        ImGui::PopStyleColor();
                        
                        if (o.env_form[0]==0) o.env_amp_a.curve = envelope_adsr::CUB;
                        if (o.env_form[0]==1) o.env_amp_a.curve = envelope_adsr::LIN;
                        if (o.env_form[0]==2) o.env_amp_a.curve = envelope_adsr::LOG;
                        if (o.env_form[0]==3) o.env_amp_a.curve = envelope_adsr::SFT;

                    }                    
                    ImGui::SameLine();

                    if(ImGui::Button("##EnvSliderTypeA", ImVec2(17,17)))
                    {
                        o.env_slider_type[0]++;
                        if(o.env_slider_type[0]>1) o.env_slider_type[0]=0;
                    }


                    ImGui::PopStyleColor();
                    }
                    else
                    {
                        ImGui::VSliderFloat("##aAT2", ImVec2(12, 80), &o.env_amp_b.adsr.A.time, 100.0f,  10000.0f*o.scale_env_amp_b, "",ImGuiSliderFlags_Logarithmic); ImGui::SameLine();
                        ImGui::VSliderFloat("##aDT2", ImVec2(12, 80), &o.env_amp_b.adsr.D.time, 1000.0f, 10000.0f*o.scale_env_amp_b, "",ImGuiSliderFlags_Logarithmic); ImGui::SameLine();
                        ImGui::VSliderFloat("##aST2", ImVec2(12, 80), &o.env_amp_b.adsr.S.time, 1000.0f, 10000.0f*o.scale_env_amp_b, "",ImGuiSliderFlags_Logarithmic); ImGui::SameLine();
                        ImGui::VSliderFloat("##aRT2", ImVec2(12, 80), &o.env_amp_b.adsr.R.time, 1000.0f, 10000.0f*o.scale_env_amp_b, "",ImGuiSliderFlags_Logarithmic); ImGui::SameLine();
                    ImGui::PushStyleColor(ImGuiCol_SliderGrab, 0xB45050FF);
                        ImGui::VSliderFloat("##aAV2", ImVec2(12, 80), &o.env_amp_b.adsr.A.value, 0.00001f, 1.0f, "");ImGui::SameLine(); 
                        ImGui::VSliderFloat("##aDV2", ImVec2(12, 80), &o.env_amp_b.adsr.D.value, 0.00001f, 1.0f, "");ImGui::SameLine();
                        ImGui::VSliderFloat("##aSV2", ImVec2(12, 80), &o.env_amp_b.adsr.S.value, 0.00001f, 1.0f, "");ImGui::SameLine();
                        ImGui::VSliderFloat("##aRV2", ImVec2(12, 80), &o.env_amp_b.adsr.R.value, 0.00001f, 1.0f, "");ImGui::SameLine();
                    ImGui::PopStyleColor();
                    

                    o.env_imprint_amp_b = imprint(&o.env_amp_b,5,100);
                    float (*func_adsr2)(void*, int) = plot_f::plot_adsr2;
                    ImGui::PlotLines("##plot_adsr2", func_adsr2, NULL, o.env_imprint_amp_b.size(), 0, NULL, 0.0f, 1.0f, ImVec2(172, 80));
                    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                    ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.129, 0.215, 0.349, 1));

                    ImGui::RadioButton("##aDENV1",&o.radio_env_amp,1); ImGui::SameLine();
                    ImGui::RadioButton("##aDENV2",&o.radio_env_amp,0);
                    
                    ImGui::SameLine();
                    ImGui::PushItemWidth(237);
                    if(o.env_slider_type[1]==0)
                    {
                        ImGui::DragFloat("##aENVSCALE2", &o.scale_env_amp_b, 0.1, 0.1, 100.0, "ADSR 2 Scale %.3f", 1);
                    }
                    else if(o.env_slider_type[1]==1)
                    {
                        ImGui::PushStyleColor(ImGuiCol_SliderGrab, IM_COL32(33, 55, 89, 255));
                        ImGui::SliderInt  ("##EnvformB", &o.env_form[1], 0, n_envs-1, env_name2);
                        ImGui::PopStyleColor();
                        
                        if (o.env_form[1]==0) o.env_amp_b.curve = envelope_adsr::CUB;
                        if (o.env_form[1]==1) o.env_amp_b.curve = envelope_adsr::LIN;
                        if (o.env_form[1]==2) o.env_amp_b.curve = envelope_adsr::LOG;
                        if (o.env_form[1]==3) o.env_amp_b.curve = envelope_adsr::SFT;

                    }
                    
                    ImGui::SameLine();
                    if(ImGui::Button("##EnvSliderTypeB", ImVec2(17,17)))
                    {
                        o.env_slider_type[1]++;
                        if(o.env_slider_type[1]>1) o.env_slider_type[1]=0;
                    }
                    ImGui::PopStyleColor();
                    }


                    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                    // ENVELOPES C & D////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                    if(o.radio_env_mod)
                    {
                        ImGui::VSliderFloat("##mAT1", ImVec2(12, 80), &o.env_mod_a.adsr.A.time, 100.0f,  10000.0f*o.scale_env_mod_a, "", ImGuiSliderFlags_Logarithmic); ImGui::SameLine();
                        ImGui::VSliderFloat("##mDT1", ImVec2(12, 80), &o.env_mod_a.adsr.D.time, 1000.0f, 10000.0f*o.scale_env_mod_a, "", ImGuiSliderFlags_Logarithmic); ImGui::SameLine();
                        ImGui::VSliderFloat("##mST1", ImVec2(12, 80), &o.env_mod_a.adsr.S.time, 1000.0f, 10000.0f*o.scale_env_mod_a, "", ImGuiSliderFlags_Logarithmic); ImGui::SameLine();
                        ImGui::VSliderFloat("##mRT1", ImVec2(12, 80), &o.env_mod_a.adsr.R.time, 1000.0f, 10000.0f*o.scale_env_mod_a, "", ImGuiSliderFlags_Logarithmic); ImGui::SameLine();
                    ImGui::PushStyleColor(ImGuiCol_SliderGrab, 0xB45050FF);
                        ImGui::VSliderFloat("##mAV1", ImVec2(12, 80), &o.env_mod_a.adsr.A.value, 0.00001f, 1.0f, ""); ImGui::SameLine(); 
                        ImGui::VSliderFloat("##mDV1", ImVec2(12, 80), &o.env_mod_a.adsr.D.value, 0.00001f, 1.0f, ""); ImGui::SameLine();
                        ImGui::VSliderFloat("##mSV1", ImVec2(12, 80), &o.env_mod_a.adsr.S.value, 0.00001f, 1.0f, ""); ImGui::SameLine();
                        ImGui::VSliderFloat("##mRV1", ImVec2(12, 80), &o.env_mod_a.adsr.R.value, 0.00001f, 1.0f, ""); ImGui::SameLine();
                    ImGui::PopStyleColor();

                    o.env_imprint_mod_a = imprint(&o.env_mod_a, 5, 100);
                    float (*func_adsr3)(void*, int) = plot_f::plot_adsr3;
                    ImGui::PlotLines("##plot_adsr3", func_adsr3, NULL, o.env_imprint_mod_a.size(), 0, NULL, 0.0f, 1.0f, ImVec2(172, 80));
                    
                    ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.129, 0.215, 0.349, 1));
                    ImGui::RadioButton("##mdLL11", &o.radio_env_mod, 1); 
                    ImGui::SameLine();
                    ImGui::RadioButton("##mdLL12", &o.radio_env_mod, 0); 
                    ImGui::SameLine();
                    ImGui::PushItemWidth(237);
                    if(o.env_slider_type[2]==0)
                    {
                        ImGui::DragFloat("##aENVSCALE3", &o.scale_env_mod_a, 0.1, 0.1, 100.0, "ADSR 3 Scale %.3f", 1);
                    }
                    else if(o.env_slider_type[2]==1)
                    {
                        ImGui::PushStyleColor(ImGuiCol_SliderGrab, IM_COL32(33, 55, 89, 255));
                        ImGui::SliderInt  ("##EnvformC", &o.env_form[2], 0, n_envs-1, env_name3);
                        ImGui::PopStyleColor();
                        
                        if (o.env_form[2]==0) o.env_mod_a.curve = envelope_adsr::CUB;
                        if (o.env_form[2]==1) o.env_mod_a.curve = envelope_adsr::LIN;
                        if (o.env_form[2]==2) o.env_mod_a.curve = envelope_adsr::LOG;
                        if (o.env_form[2]==3) o.env_mod_a.curve = envelope_adsr::SFT;
                    }
                    
                    ImGui::SameLine();
                    if(ImGui::Button("##EnvSliderTypeC", ImVec2(17,17)))
                    {
                        o.env_slider_type[2]++;
                        if(o.env_slider_type[2]>1) o.env_slider_type[2]=0;
                    }
                    }
                    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                    else
                    {
                        ImGui::VSliderFloat("##mAT2", ImVec2(12, 80), &o.env_mod_b.adsr.A.time, 100.0f,  10000.0f*o.scale_env_mod_b, "", ImGuiSliderFlags_Logarithmic); ImGui::SameLine();
                        ImGui::VSliderFloat("##mDT2", ImVec2(12, 80), &o.env_mod_b.adsr.D.time, 1000.0f, 10000.0f*o.scale_env_mod_b, "", ImGuiSliderFlags_Logarithmic); ImGui::SameLine();
                        ImGui::VSliderFloat("##mST2", ImVec2(12, 80), &o.env_mod_b.adsr.S.time, 1000.0f, 10000.0f*o.scale_env_mod_b, "", ImGuiSliderFlags_Logarithmic); ImGui::SameLine();
                        ImGui::VSliderFloat("##mRT2", ImVec2(12, 80), &o.env_mod_b.adsr.R.time, 1000.0f, 10000.0f*o.scale_env_mod_b, "", ImGuiSliderFlags_Logarithmic); ImGui::SameLine();
                    ImGui::PushStyleColor(ImGuiCol_SliderGrab, IM_COL32(180, 80, 80, 255));
                        ImGui::VSliderFloat("##mAV2", ImVec2(12, 80), &o.env_mod_b.adsr.A.value, 0.00001f, 1.0f, ""); ImGui::SameLine(); 
                        ImGui::VSliderFloat("##mDV2", ImVec2(12, 80), &o.env_mod_b.adsr.D.value, 0.00001f, 1.0f, ""); ImGui::SameLine();
                        ImGui::VSliderFloat("##mSV2", ImVec2(12, 80), &o.env_mod_b.adsr.S.value, 0.00001f, 1.0f, ""); ImGui::SameLine();
                        ImGui::VSliderFloat("##mRV2", ImVec2(12, 80), &o.env_mod_b.adsr.R.value, 0.00001f, 1.0f, ""); ImGui::SameLine();
                    ImGui::PopStyleColor();

                    o.env_imprint_mod_b = imprint(&o.env_mod_b, 5, 100);
                    float (*func_adsr4)(void*, int) = plot_f::plot_adsr4;
                    ImGui::PlotLines("##plot_adsr4", func_adsr4, NULL, o.env_imprint_mod_b.size(), 0, NULL, 0.0f, 1.0f, ImVec2(172, 80));
                    
                    
                    ImGui::PushStyleColor(ImGuiCol_FrameBg,ImVec4(0.129, 0.215, 0.349, 1));
                    ImGui::RadioButton("##mdLL11", &o.radio_env_mod, 1); 
                    ImGui::SameLine();
                    ImGui::RadioButton("##mdLL12", &o.radio_env_mod, 0); 
                    ImGui::SameLine();
                    ImGui::PushItemWidth(237);
                    if(o.env_slider_type[3]==0)
                    {
                        ImGui::DragFloat("##aENVSCALE4", &o.scale_env_mod_b, 0.1, 0.1, 100.0, "ADSR 4 Scale %.3f", 1);
                    }
                    else if(o.env_slider_type[3]==1)
                    {
                        ImGui::PushStyleColor(ImGuiCol_SliderGrab, IM_COL32(33, 55, 89, 255));
                        ImGui::SliderInt  ("##EnvformD", &o.env_form[3], 0, n_envs-1, env_name4);
                        ImGui::PopStyleColor();
                        
                        if (o.env_form[3]==0) o.env_mod_b.curve = envelope_adsr::CUB;
                        if (o.env_form[3]==1) o.env_mod_b.curve = envelope_adsr::LIN;
                        if (o.env_form[3]==2) o.env_mod_b.curve = envelope_adsr::LOG;
                        if (o.env_form[3]==3) o.env_mod_b.curve = envelope_adsr::SFT;
                    }
                    
                    ImGui::SameLine();
                    if(ImGui::Button("##EnvSliderTypeD", ImVec2(17,17)))
                    {
                        o.env_slider_type[3]++;
                        if(o.env_slider_type[3]>1) o.env_slider_type[3]=0;
                    }
                    }
                    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

                    ImGui::PopStyleColor();
                
                    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                    // LFOs //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                    float (*func_lfo_imprint_a)(void*, int) = plot_f::plot_lfo_imprint_a;
                    float (*func_lfo_imprint_b)(void*, int) = plot_f::plot_lfo_imprint_b;
                    
                
                    ImGui::PlotLines("##LfoImprintA", func_lfo_imprint_a, NULL, o.lfo_imprint_a.size(), 0, NULL, -1.0f, 1.0f, ImVec2(120, 80));
                    ImGui::SameLine();
                    if( ImGui::VSliderFloat("##fLFOA", ImVec2(10, 80), &o.lfo_a.frequency, 0.001f, 10.0f, "", ImGuiSliderFlags_Logarithmic)) o.lfo_imprint_a = imprint(&o.lfo_a, 120, 1); 
                    ImGui::SameLine();
                    if( ImGui::VSliderFloat("##LFOAA", ImVec2(10, 80), &mute.renderer->cvin[12], 0.0f, 1.0f, "")) o.lfo_imprint_a = imprint(&o.lfo_a, 120, 1); 

                    ImGui::SameLine();
                    ImGui::PlotLines("##LfoImprintB", func_lfo_imprint_b, NULL, o.lfo_imprint_b.size(), 0, NULL, -1.0f, 1.0f, ImVec2(120, 80));
                    ImGui::SameLine();
                    if( ImGui::VSliderFloat("##fLFOB", ImVec2(10, 80), &o.lfo_b.frequency, 0.001f, 10.0f, "", ImGuiSliderFlags_Logarithmic)) o.lfo_imprint_b = imprint(&o.lfo_b, 120, 1); 
                    ImGui::SameLine();
                    if( ImGui::VSliderFloat("##LFOBA", ImVec2(10, 80), &mute.renderer->cvin[13], 0.0f, 1.0f, "")) o.lfo_imprint_b = imprint(&o.lfo_b, 120, 1); 
                    ImGui::SameLine();

        
                    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                    // LFOs /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

                    o.lfo_imprint_a = imprint(&o.lfo_a, 120,1);
                    o.lfo_imprint_b = imprint(&o.lfo_b, 120,1);

                    const char* lfo_name1 = (o.lfo_imprint_a_type >= 0 && o.lfo_imprint_a_type < n_lfos) ? lfo_names[o.lfo_imprint_a_type] : "Unknown";
                    const char* lfo_name2 = (o.lfo_imprint_b_type >= 0 && o.lfo_imprint_b_type < n_lfos) ? lfo_names[o.lfo_imprint_b_type] : "Unknown";
                    const char* mod_name_lfo_a = (o.lfo_a_mod_type >= 0 && o.lfo_a_mod_type < n_mods) ? mod_types[o.lfo_a_mod_type] : "Unknown";
                    const char* mod_name_lfo_b = (o.lfo_b_mod_type >= 0 && o.lfo_b_mod_type < n_mods) ? mod_types[o.lfo_b_mod_type] : "Unknown";

                    ImGui::NewLine();
                    ImGui::PushStyleColor(ImGuiCol_SliderGrab,ImVec4(0.14, 0.2, 0.28, 1));

                    if (ImGui::Button("##SwitchLfoA", ImVec2(17, 17)))
                    {
                        switch_lfo_a++;
                        switch_lfo_a>2? switch_lfo_a=0: switch_lfo_a;
                    }
                    ImGui::SameLine();
                    ImGui::PushItemWidth(127);
                    if(switch_lfo_a==0)
                    {
                        if(ImGui::SliderInt("##TypeLfoA", &o.lfo_imprint_a_type,0, n_lfos-1, lfo_name1))
                        {
                            o.lfo_a.form = o.lfo_imprint_a_type;
                            o.lfo_imprint_a = imprint(&o.lfo_a, 120, 1);
                        }
                    }
                    if(switch_lfo_a==1)
                    {
                        if(ImGui::SliderInt("##ModLfoA", &o.lfo_a_mod_type ,0, n_mods-1, mod_name_lfo_a))
                        {

                        }

                    }
                    if(switch_lfo_a==2)
                    {
                        if(ImGui::SliderFloat("##AmountLfoA", &o.lfo_a_mod_amount, 0.0f, 1.0f, "%.4f"))
                        {

                        }
                    }

                    ImGui::SameLine();
                    if (ImGui::Button("##SwitchLfoB", ImVec2(17, 17)))
                    {
                        switch_lfo_b++;
                        switch_lfo_b>2? switch_lfo_b=0: switch_lfo_b;
                    }
                    ImGui::SameLine();
                    ImGui::PushItemWidth(127);
                    if(switch_lfo_b==0)
                    {
                        if(ImGui::SliderInt("##TypeLfoB", &o.lfo_imprint_b_type,0, n_lfos-1, lfo_name2))
                        {
                            o.lfo_b.form = o.lfo_imprint_b_type;
                            o.lfo_imprint_b = imprint(&o.lfo_b, 120, 1);
                        }
                    }
                    if(switch_lfo_b==1)
                    {
                        if(ImGui::SliderInt("##ModLfoB", &o.lfo_b_mod_type ,0, n_mods-1, mod_name_lfo_b))
                        {

                        }

                    }
                    if(switch_lfo_b==2)
                    {
                        if(ImGui::SliderFloat("##AmountLfoB", &o.lfo_b_mod_amount, 0.0f, 1.0f, "%.4f"))
                        {

                        }
                    }
                    ImGui::PopStyleColor();
                    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                    // Oscillograph /////////////////////////////////////////////////////////////////////////////////////////////////////
                    float (*func)(void*, int) = plot_f::plot_output;
                    ImGui::PlotLines("##plot_out", func, NULL, 2048, 0, NULL, -1.0f, 1.0f, ImVec2(300, 101));
                    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                    
                    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

                    ImGui::BeginChild(7,ImVec2(300,40));
                    if (ImGui::Button("Play",ImVec2(38, 17))) mute.play();
                    ImGui::SameLine();
                    if (ImGui::Button("Stop",ImVec2(38, 17))) mute.stop();
                    ImGui::SameLine();
                    ImGui::ArrowButton("##PreviousPreset", ImGuiDir_Left);
                    ImGui::SameLine();
                    
                    ImGui::PushItemWidth(173);
                    ImGui::InputText("##PresetName", pset_name, IM_ARRAYSIZE(pset_name));
                    ImGui::PopItemWidth();
                    ImGui::SameLine();
                    ImGui::ArrowButton("##NextPreset", ImGuiDir_Right);
                    ImGui::Button("i",ImVec2(17, 17));
                    ImGui::SameLine();
                    ImGui::Button("settings",ImVec2(80, 17));
                    ImGui::SameLine();
                    if(ImGui::Button("load",ImVec2(95, 17)))
                    {  
                        o.get_list();
                        ImGui::OpenPopup("Load preset");  
                    }
     
                    ImGui::SetNextWindowSize(ImVec2(788.0f,473.0f),0);
                    ImGui::SetNextWindowPos(ImVec2(0.0f,17.0f));
                    if(ImGui::BeginPopup("Load preset"))
                    {
                        ImGui::BeginChild(11,ImVec2(312,439));
                        
                        static int sel =0;
                        for(size_t i=0; i<o.preset_count;i++)
                        {
                            if (ImGui::Selectable(o.preset_list[i].c_str(),&f[i], ImGuiSelectableFlags_DontClosePopups|ImGuiSelectableFlags_AllowDoubleClick))
                            {
                                for(int j=0; j<32; j++) f[j]=0;
                                f[i] = true;
                                sel = i;
                            }
                        }
                        ImGui::EndChild();
                        if(ImGui::Button("Close",ImVec2(80, 17)))
                        ImGui::CloseCurrentPopup();
                        ImGui::SameLine();
                        if(ImGui::Button("Load",ImVec2(80, 17)))
                        {
                            o.name = o.preset_list[sel];
                            for(size_t i=0; i<o.name.length();i++) 
                            {
                                pset_name[i]= o.name[i];
                            }
                            o.load();

                            mute.renderer->env[0] = o.env_amp_a.adsr; 
                            mute.renderer->env[1] = o.env_amp_b.adsr;
                            mute.renderer->env[2] = o.env_mod_a.adsr;
                            mute.renderer->env[3] = o.env_mod_b.adsr;

                            o.lfo_a.form = o.lfo_imprint_a_type;
                            o.lfo_b.form = o.lfo_imprint_b_type;
                            
                            form_lfo[o.lfo_a.form](&o.lfo_a);
                            form_lfo[o.lfo_b.form](&o.lfo_b);
                            o.lfo_a.init(false);
                            o.lfo_b.init(false);

                            mute.renderer->lfo[0].frequency = o.lfo_a.frequency;
                            mute.renderer->lfo[1].frequency = o.lfo_b.frequency;
                            mute.renderer->lfo[0].form = o.lfo_a.form;
                            mute.renderer->lfo[1].form = o.lfo_b.form;

                            mute.renderer->form_vco[0]= o.form_vco[0];
                            mute.renderer->form_vco[1]= o.form_vco[1];
                            mute.renderer->volume = o.volume;

                            o.lfo_imprint_a = imprint(&o.lfo_a, 120, 1);
                            o.lfo_imprint_b = imprint(&o.lfo_b, 120, 1);
                            mute.renderer->esq.pattern = o.beat;
                            mute.renderer->esq.timeset(int(o.tempo*10000));
                            mute.renderer->esq.set((get_scale(o.note_set, o.oct_set)));
                            
                            ImGui::CloseCurrentPopup();
            
                        }
                        ImGui::EndPopup();
                    }
                

                    ImGui::SameLine();
                    if(ImGui::Button("Save",ImVec2(95, 17)))
                    {
                        o.name = pset_name;
                        o.save();
                    }
                    ImGui::EndChild();
                    ImGui::EndChild();
                    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                    // MODULATION & FEEDBACK ////////////////////////////////////////////////////////////////////////////////////////////

              
                    mute.renderer->env[0] = o.env_amp_a.adsr; 
                    mute.renderer->env[1] = o.env_amp_b.adsr;
                    mute.renderer->env[2] = o.env_mod_a.adsr;
                    mute.renderer->env[3] = o.env_mod_b.adsr;
                    
                    form_lfo[o.lfo_a.form](&o.lfo_a);
                    form_lfo[o.lfo_b.form](&o.lfo_b);
                    o.lfo_a.init(false);
                    o.lfo_b.init(false);

                    mute.renderer->lfo[0].frequency = o.lfo_a.frequency;
                    mute.renderer->lfo[1].frequency = o.lfo_b.frequency;
                    mute.renderer->lfo[0].form = o.lfo_a.form;
                    mute.renderer->lfo[1].form = o.lfo_b.form;

                    mute.renderer->lfo[0].init(false);
                    mute.renderer->lfo[1].init(false);

                    //////////////////////////////////////////////
                    // amp_mod_amount[0]    - Amp 1 Amount      //
                    // amp_mod_amount[1]    - Amp 2 Amount      //
                    // amp_mod_type[0]      - Amp 1 Mod Type    //
                    // amp_mod_type[1]      - Amp 2 Mod Type    //
                    // pwm_mod_amount[0]    - Pwm 1 Amount      //
                    // pwm_mod_amount[1]    - Pwm 2 Amount      //
                    // pwm_mod_type[0]      - Pwm 1 Mod Type    //
                    // pwm_mod_type[1]      - Pwm 2 Mod Type    //
                    // phase_mod_amount[0]  - Phase 1 Amount    //
                    // phase_mod_amount[1]  - Phase 2 Amount    //
                    // phase_mod_type[0]    - Phase 1 Mod Type  //
                    // phase_mod_type[1]    - Phase 2 Mod Type  //
                    // cutoff_mod_amount[0] - Cutoff 1 Amount   //
                    // cutoff_mod_amount[1] - Cutoff 2 Amount   //
                    // cutoff_mod_type[0]   - Cutoff 1 Mod Type //
                    // cutoff_mod_type[1]   - Cutoff 2 Mod Type //
                    //////////////////////////////////////////////

                                                                                                                                                                            //    Delay Time
                    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                    // modmatrix [9][14][2] ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                    //|         |      |      |      |      |      |      |      |      |      |      |      |      |      |      |      |      |      |      |      |      |      |      |      |//
                    //|         |AMP[0]|AMP[1]|AMP[2]|PWM[0]|PWM[1]|PWM[2]|PHS[0]|PHS[1]|PHS[2]|CUT[0]|CUT[1]|CUT[2]|LFO[0]|LFO[1]|PAN[0]|PAN[1]|PAN[2]|FRQ[0]|FRQ[1]|FRQ[2]|DTM[0]|DTM[1]|DTM[2]|// <- Target
                    //|_________|______|______|______|______|______|______|______|______|______|______|______|______|______|______|______|______|______|______|______|______|______|______|______|//
                    //| ADSR[0] |  0   |  1   |  2   |  3   |  4   |  5   |  6   |  7   |  8   |  9   |  10  |  11  |  12  |  13  |  14  |  15  |  16  |  17  |  18  |  19  |  20  |  21  |  22  |//
                    //|--------------------------------------------------------------------------------------------------------------------------------------------------------------------------|//
                    //| ADSR[1] |  1   |      |      |      |      |      |      |      |      |      |      |      |      |      |      |      |      |      |      |      |      |      |      |//
                    //|--------------------------------------------------------------------------------------------------------------------------------------------------------------------------|//
                    //| ADSR[2] |  2   |      |      |      |      |      |      |      |      |      |      |      |      |      |      |      |      |      |      |      |      |      |      |//
                    //|--------------------------------------------------------------------------------------------------------------------------------------------------------------------------|//
                    //| ADSR[3] |  3   |      |      |      |      |      |      |      |      |      |      |      |      |      |      |      |      |      |      |      |      |      |      |//
                    //|--------------------------------------------------------------------------------------------------------------------------------------------------------------------------|//
                    //| LFO[0]  |  4   |      |      |      |      |      |      |      |      |      |      |      |      |      |      |      |      |      |      |      |      |      |      |//
                    //|--------------------------------------------------------------------------------------------------------------------------------------------------------------------------|//
                    //| LFO[1]  |  5   |      |      |      |      |      |      |      |      |      |      |      |      |      |      |      |      |      |      |      |      |      |      |//
                    //|--------------------------------------------------------------------------------------------------------------------------------------------------------------------------|//
                    //| VCO[0]  |  6   |      |      |      |      |      |      |      |      |      |      |      |      |      |      |      |      |      |      |      |      |      |      |//
                    //|--------------------------------------------------------------------------------------------------------------------------------------------------------------------------|//
                    //| VCO[1]  |  7   |      |      |      |      |      |      |      |      |      |      |      |      |      |      |      |      |      |      |      |      |      |      |//
                    //|--------------------------------------------------------------------------------------------------------------------------------------------------------------------------|//
                    //| VCO[2]  |  8   |      |      |      |      |      |      |      |      |      |      |      |      |      |      |      |      |      |      |      |      |      |      |//
                    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                    // [][][0] = amount ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                    // [][][1] = center value //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                    
                    
                    mute.renderer->clear_matrix();

                    if(o.amp_mod_type[0]>0)
                    {
                        mute.renderer->modmatrix[o.amp_mod_type[0]-1][0][0] = o.amp_mod_amount[0];
                        mute.renderer->modmatrix[o.amp_mod_type[0]-1][0][1] = o.center_amp[0];
                    }
                    if(o.amp_mod_type[1]>0)
                    {    
                        mute.renderer->modmatrix[o.amp_mod_type[1]-1][1][0] = o.amp_mod_amount[1];
                        mute.renderer->modmatrix[o.amp_mod_type[1]-1][1][1] = o.center_amp[1];
                    }
                    if(o.amp_mod_type[2]>0)
                    {    
                        mute.renderer->modmatrix[o.amp_mod_type[2]-1][2][0] = o.amp_mod_amount[2];
                        mute.renderer->modmatrix[o.amp_mod_type[2]-1][2][1] = o.center_amp[2];
                    }




                    if(o.pwm_mod_type[0]>0)
                    {
                        mute.renderer->modmatrix[o.pwm_mod_type[0]-1][3][0] = o.pwm_mod_amount[0];
                        mute.renderer->modmatrix[o.pwm_mod_type[0]-1][3][1] = o.center_pwm[0];
                    }
                    if(o.pwm_mod_type[1]>0)    
                    {
                        mute.renderer->modmatrix[o.pwm_mod_type[1]-1][4][0] = o.pwm_mod_amount[1];
                        mute.renderer->modmatrix[o.pwm_mod_type[1]-1][4][1] = o.center_pwm[1];
                    }
                    if(o.pwm_mod_type[2]>0)    
                    {
                        mute.renderer->modmatrix[o.pwm_mod_type[2]-1][5][0] = o.pwm_mod_amount[2];
                        mute.renderer->modmatrix[o.pwm_mod_type[2]-1][5][1] = o.center_pwm[2];
                    }



                    if(o.phase_mod_type[0]>0)
                    {  
                        mute.renderer->modmatrix[o.phase_mod_type[0]-1][6][0] = o.phase_mod_amount[0];
                        mute.renderer->modmatrix[o.phase_mod_type[0]-1][6][1] = o.center_phase[0];
                    }
                    if(o.phase_mod_type[1]>0)  
                    {
                        mute.renderer->modmatrix[o.phase_mod_type[1]-1][7][0] = o.phase_mod_amount[1];
                        mute.renderer->modmatrix[o.phase_mod_type[1]-1][7][1] = o.center_phase[1];
                    }
                    if(o.phase_mod_type[2]>0)  
                    {
                        mute.renderer->modmatrix[o.phase_mod_type[2]-1][8][0] = o.phase_mod_amount[2];
                        mute.renderer->modmatrix[o.phase_mod_type[2]-1][8][1] = o.center_phase[2];
                    }


                    if(o.cutoff_mod_type[0]>0) 
                    {
                        mute.renderer->modmatrix[o.cutoff_mod_type[0]-1][9][0] = o.cutoff_mod_amount[0];
                        mute.renderer->modmatrix[o.cutoff_mod_type[0]-1][9][1] = o.center_cutoff[0];
                    }
                    if(o.cutoff_mod_type[1]>0)
                    {
                        mute.renderer->modmatrix[o.cutoff_mod_type[1]-1][10][0] = o.cutoff_mod_amount[1];
                        mute.renderer->modmatrix[o.cutoff_mod_type[1]-1][10][1] = o.center_cutoff[1];
                    }
                    if(o.cutoff_mod_type[2]>0)
                    {
                        mute.renderer->modmatrix[o.cutoff_mod_type[2]-1][11][0] = o.cutoff_mod_amount[2];
                        mute.renderer->modmatrix[o.cutoff_mod_type[2]-1][11][1] = o.center_cutoff[2];
                    }




                    if(o.lfo_a_mod_type>0)
                    {
                        mute.renderer->modmatrix[o.lfo_a_mod_type-1][12][0] = o.lfo_a_mod_amount;
                        mute.renderer->modmatrix[o.lfo_a_mod_type-1][12][1] = 0;
                    }
                    if(o.lfo_b_mod_type>0)
                    {
                        mute.renderer->modmatrix[o.lfo_b_mod_type-1][13][0] = o.lfo_b_mod_amount;
                        mute.renderer->modmatrix[o.lfo_b_mod_type-1][13][1] = 0;
                    }

                    if(o.pan_mod_type[0]>0)
                    {
                        mute.renderer->modmatrix[o.pan_mod_type[0]-1][14][0] = o.pan_mod_amount[0];
                        mute.renderer->modmatrix[o.pan_mod_type[0]-1][14][1] = 0;
                    }
                    if(o.pan_mod_type[1]>0)
                    {
                        mute.renderer->modmatrix[o.pan_mod_type[1]-1][15][0] = o.pan_mod_amount[1];
                        mute.renderer->modmatrix[o.pan_mod_type[1]-1][15][1] = 0;
                    }
                    if(o.pan_mod_type[2]>0)
                    {
                        mute.renderer->modmatrix[o.pan_mod_type[2]-1][16][0] = o.pan_mod_amount[2];
                        mute.renderer->modmatrix[o.pan_mod_type[2]-1][16][1] = 0;
                    }

                    if(o.detune_mod_type[0]>0)
                    {
                        mute.renderer->modmatrix[o.detune_mod_type[0]-1][17][0] = o.detune_mod_amount[0];
                        mute.renderer->modmatrix[o.detune_mod_type[0]-1][17][1] = 0;
                    }
                    if(o.detune_mod_type[1]>0)
                    {
                        mute.renderer->modmatrix[o.detune_mod_type[1]-1][18][0] = o.detune_mod_amount[1];
                        mute.renderer->modmatrix[o.detune_mod_type[1]-1][18][1] = 0;
                    }
                    if(o.detune_mod_type[2]>0)
                    {
                        mute.renderer->modmatrix[o.detune_mod_type[2]-1][19][0] = o.detune_mod_amount[2];
                        mute.renderer->modmatrix[o.detune_mod_type[2]-1][19][1] = 0;
                    }

                    if(o.ddtime_mod_type[0]>0)
                    {
                        mute.renderer->modmatrix[o.ddtime_mod_type[0]-1][20][0] = o.ddtime_mod_amount[0];
                        mute.renderer->modmatrix[o.ddtime_mod_type[0]-1][20][1] = 0;
                    }
                    if(o.ddtime_mod_type[1]>0)
                    {
                        mute.renderer->modmatrix[o.ddtime_mod_type[1]-1][21][0] = o.ddtime_mod_amount[1];
                        mute.renderer->modmatrix[o.ddtime_mod_type[1]-1][21][1] = 0;
                    }
                    if(o.ddtime_mod_type[2]>0)
                    {
                        mute.renderer->modmatrix[o.ddtime_mod_type[2]-1][22][0] = o.ddtime_mod_amount[2];
                        mute.renderer->modmatrix[o.ddtime_mod_type[2]-1][22][1] = 0;
                    }


            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            ImGui::End();
        }
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Rendering ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        ImGui::Render();
        glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
        glClearColor(0.11, 0.15, 0.17, 1);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        SDL_GL_SwapWindow(window);
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Cleanup //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

