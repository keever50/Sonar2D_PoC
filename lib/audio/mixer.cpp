#include <mixer.h>
#include <hardware/divider.h>

void Mixer_AudioGenerator::setVolume(float v)
{
    const int32_t max = 0x7FFFFFFF-1;
    
    if(v>=1.0f){_volume_divisor=1; return;}
    if((1.0f/v)>=max){_volume_divisor=max; return;}
    _volume_divisor = (int32_t)(1.0f/v);
}

void Mixer_Output::set_channel(int channel, Mixer_AudioGenerator* generator)
{
    if(channel<0) return;
    if(channel>=MIXER_MAX_CHANNELS) return;

    _inputs[channel]=generator;
}

int32_t Mixer_Output::mix(int32_t sample, int32_t pre_sample)
{
    if ((pre_sample > 0 && sample > INT32_MAX - pre_sample)) {
        return INT32_MAX; // Overflow
    } else if ((pre_sample < 0 && sample < INT32_MIN - pre_sample)) {
        return INT32_MIN; // Underflow
    } else {
        return sample + pre_sample; // Normal mix
    }    
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

        /*Volume mix samples if not zero*/
        if (pre_sample.L != 0) {
            pre_sample.L = hw_divider_quotient_s32(pre_sample.L, generator->_volume_divisor);
        }
        if (pre_sample.R != 0 && !pre_sample.is_mono) { /*Only process R if not mono and not zero*/
            pre_sample.R = hw_divider_quotient_s32(pre_sample.R, generator->_volume_divisor);
        }

        /*Mixing based on mono/stereo configurations*/
        if (!pre_sample.is_mono) {
            /*Input is STEREO*/
            if (!sample->is_mono) {
                /*Output is also STEREO*/
                if (pre_sample.L != 0) sample->L = mix(sample->L, pre_sample.L);
                if (pre_sample.R != 0) sample->R = mix(sample->R, pre_sample.R);
            } else {
                /*Output is MONO - Mix both L and R into output L*/
                if (pre_sample.L != 0) sample->L = mix(sample->L, pre_sample.L);
                if (pre_sample.R != 0) sample->L = mix(sample->L, pre_sample.R);
            }
        } else {
            /*Input is MONO*/
            if (sample->is_mono) {
                /*Output is also MONO - Only mix L*/
                if (pre_sample.L != 0) sample->L = mix(sample->L, pre_sample.L);
            } else {
                /*Output is STEREO - Mix input L to both L and R of output*/
                if (pre_sample.L != 0) {
                    sample->L = mix(sample->L, pre_sample.L);
                    sample->R = mix(sample->R, pre_sample.L);
                }
            }
        }


    }
}