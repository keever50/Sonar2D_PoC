#pragma once
#include <Arduino.h>

#define MIXER_MAX_CHANNELS  32

class Mixer_Sample
{
public:
    int32_t L=0;
    int32_t R=0;
    bool is_mono=false;
};

class Mixer_AudioGenerator
{
public:
    void(*audioGenerator_cb)(int samplesLeft, Mixer_Sample* sample);
    /*0-1 float value representing 0-100%*/
    void setVolume(float v);
    int32_t _volume_divisor;
private:
    
    
};

class Mixer_Output
{
public:
    void set_channel(int channel, Mixer_AudioGenerator *generator);
    void callback(int samplesLeft, Mixer_Sample* sample);
private:
    Mixer_AudioGenerator *_inputs[MIXER_MAX_CHANNELS];
    int32_t mix(int32_t sample, int32_t pre_sample);
};