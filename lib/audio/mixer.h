#pragma once
#include <Arduino.h>

#define MIXER_MAX_CHANNELS  32

class Mixer_Sample
{
public:
    uint32_t L;
    uint32_t R;
};

class Mixer_AudioGenerator
{
public:
    void(*audioGenerator_cb)(int samplesLeft, Mixer_Sample* sample);
    /*0-1 float value representing 0-100%*/
    void setVolume(float v);
    uint32_t _volume_divisor;
private:
    
    
};

class Mixer_Output
{
public:
    void set_channel(int channel, Mixer_AudioGenerator *generator);
    void callback(int samplesLeft, Mixer_Sample* sample);
private:
    Mixer_AudioGenerator *_inputs[MIXER_MAX_CHANNELS];
};