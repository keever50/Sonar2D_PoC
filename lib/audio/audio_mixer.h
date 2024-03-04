/*Coded by Kevin Witteveen*/

#pragma once
#include <Arduino.h>
#include <audio_components.h>

#define MIXER_MAX_CHANNELS  32
#define MIXER_VOLUME_ACCURACY   10



class Mixer_Output : public Audio_Component_Output
{
public:
    void set_channel(int channel, Audio_Component_Output *generator);
    //void callback(int samplesLeft, Mixer_Sample* sample);
    virtual int get_sample(int samplesLeft, Mixer_Sample* sample) override;
    void set_volume(int channel, float L, float R);

private:
    Audio_Component_Output *_inputs[MIXER_MAX_CHANNELS];
    int32_t _volume_divisorsL[MIXER_MAX_CHANNELS]={1};
    int32_t _volume_divisorsR[MIXER_MAX_CHANNELS]={1};
    int32_t mix(int32_t sample, int32_t pre_sample);
};