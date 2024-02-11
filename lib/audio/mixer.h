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

class Mixer_Output : public Audio_Component_Output
{
public:
    void set_channel(int channel, Audio_Component_Output *generator);
    //void callback(int samplesLeft, Mixer_Sample* sample);
    virtual int get_sample(int samplesLeft, Mixer_Sample* sample) override;
    void set_volume(int channel, float v);
private:
    Audio_Component_Output *_inputs[MIXER_MAX_CHANNELS];
    int32_t _volume_divisors[MIXER_MAX_CHANNELS]={1};
    int32_t mix(int32_t sample, int32_t pre_sample);
};