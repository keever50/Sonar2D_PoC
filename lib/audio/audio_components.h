#pragma once
#include <Arduino.h>

#define AUDIO_DEFAULT_SAMPLE_RATE       32000
#define AUDIO_DEFAULT_STEREO            true

class Audio_Info
{
public:
   int samplerate=AUDIO_DEFAULT_SAMPLE_RATE;
   bool stereo=AUDIO_DEFAULT_STEREO;
};

class Mixer_Sample
{
public:
    int32_t L=0;
    int32_t R=0;
    bool is_mono=false;
};


class Audio_Component_Output 
{
public:
    virtual int get_sample(int samplesLeft, Mixer_Sample* sample)
    {
        return 0;
    }
};

