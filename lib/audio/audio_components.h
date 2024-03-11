#pragma once
#include <Arduino.h>

#define AUDIO_DEFAULT_SAMPLE_RATE       32000
#define AUDIO_DEFAULT_STEREO            true

typedef struct
{
   int samplerate=AUDIO_DEFAULT_SAMPLE_RATE;
   bool stereo=AUDIO_DEFAULT_STEREO;
} Audio_Info;

typedef struct 
{
    int16_t L=0;
    int16_t R=0;
    bool is_mono=false;
} Mixer_Sample;


class Audio_Component_Output 
{
public:
    virtual int get_sample(int samplesLeft, Mixer_Sample* sample)
    {
        return 0;
    }
};

