////////////////////////////////////////////////////////////////////////////////////////
// CALLBACK WRAPPER 
// V.0.0.1 31-12-2021
////////////////////////////////////////////////////////////////////////////////////////
#ifndef __FEEDER
#define __FEEDER

#include <iostream>
#include <cstdint>
#include <cstdlib>
#include <SDL2/SDL.h>
#include <SDL2/SDL_audio.h>
#include "iospecs.hpp"
#include "wavering.hpp"
#include "spawner.hpp"
#include "vco.hpp"

////////////////////////////////////////////////////////////////////////////////////////
// Feed the destination audio buffer ///////////////////////////////////////////////////

class feeder
{ 
    private:
    public:
        circular*               data;
        spawner*                renderer;
        SDL_AudioDeviceID       id;
        static inline void callback(void*, Uint8*, int);

    public:
        void play();
        void stop();
        feeder();
        ~feeder();
};

void feeder::play()
{
        SDL_PauseAudioDevice(id, 0);
}

void feeder::stop()
{
        SDL_PauseAudioDevice(id, 1);
}

feeder::feeder()
{
        SDL_AudioSpec desired, obtained;

        SDL_zero(desired);
        desired.freq       = settings.sample_rate;
        desired.format     = settings.format;
        desired.channels   = settings.channels;
        desired.samples    = settings.buffer_size;
        desired.callback   = callback;
        desired.userdata   = this;

        id = SDL_OpenAudioDevice(NULL, 0, &desired, &obtained, 0 ); 
        if (!id) std::cerr<< "Audio init error : "<<SDL_GetError()<<"\n";
        
        std::cout<<"Sample rate:\t"<<settings.sample_rate<<"\n";
        std::cout<<"Format     :\t"<<settings.format<<"\n";
        std::cout<<"Channels   :\t"<<settings.channels<<"\n";
        std::cout<<"Buffer size:\t"<<settings.buffer_size<<"\n";

        data = new circular(desired.samples);
        renderer = new spawner(data);
}

feeder::~feeder()
{
        SDL_CloseAudioDevice(id);
        delete renderer;
        delete data;
}

inline void feeder::callback(void *self, Uint8 *buffer, int length)
{  
        feeder* ref = (feeder*)self;
        float* stream = (float*)(buffer);
        ref->renderer->spawn();

        for(int i = 0; i < (length/8); i+=2)
        {
                stream[2 * i + 0] = ref->data->get()[i]; // L
                stream[2 * i + 1] = ref->data->get()[i+1]; // R
                ref->renderer->departed++;
        }
}

#endif /////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////