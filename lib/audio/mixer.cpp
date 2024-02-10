#include <mixer.h>
#include <hardware/divider.h>

void Mixer_AudioGenerator::setVolume(float v)
{
    const uint32_t max = 0x7FFFFFFF-1;
    
    if(v>=1.0f){_volume_divisor=1; return;}
    if((1.0f/v)>=max){_volume_divisor=max; return;}
    _volume_divisor = (uint32_t)(1.0f/v);
}

void Mixer_Output::set_channel(int channel, Mixer_AudioGenerator* generator)
{
    if(channel<0) return;
    if(channel>=MIXER_MAX_CHANNELS) return;

    _inputs[channel]=generator;
}

void Mixer_Output::callback(int samplesLeft, Mixer_Sample *sample)
{
    for(int i=0;i<MIXER_MAX_CHANNELS;i++)
    {
        /*Get generator and check existance*/
        Mixer_AudioGenerator *generator=_inputs[i];
        if(generator==nullptr) continue;

        /*Catch sample*/
        static Mixer_Sample pre_sample;
        generator->audioGenerator_cb(samplesLeft, &pre_sample);

        /*Apply volume using hardware division*/
/*TODO: CHECK STEREO*/
        pre_sample.L = hw_divider_quotient_s32(pre_sample.L,generator->_volume_divisor);
        pre_sample.R = hw_divider_quotient_s32(pre_sample.R,generator->_volume_divisor);

        /*Mix*/
        sample->L=sample->L+pre_sample.L;
        sample->R=sample->R+pre_sample.R;

    }
}