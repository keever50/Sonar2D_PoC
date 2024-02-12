#pragma once
#include <Arduino.h>

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


class Mixer_AudioGenerator
{
public:
    void(*audioGenerator_cb)(int samplesLeft, Mixer_Sample* sample);
    /*0-1 float value representing 0-100%*/
    void setVolume(float v);
    int32_t _volume_divisor;
private:
    
    
};