#pragma once
#include <Arduino.h>
#include <audio_components.h>

class Audio_Delay : public Audio_Component_Output
{
public:
    virtual int get_sample(int samplesLeft, Mixer_Sample* sample) override;
    void set_input(Audio_Component_Output *component);
private:    
    Audio_Component_Output *_input;
    int32_t tape[300];
};