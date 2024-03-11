/*Coded by Kevin Witteveen*/
#include <audio_mixer.h>
#include <hardware/divider.h>


void Mixer_Output::set_channel(int channel, Audio_Component_Output* generator)
{
    if(channel<0) return;
    if(channel>=MIXER_MAX_CHANNELS) return;

    _inputs[channel]=generator;
}

int32_t Mixer_Output::mix(int32_t sample, int32_t pre_sample)
{
    if ((pre_sample > 0 && sample > INT16_MAX - pre_sample)) {
        return INT16_MAX; // Overflow
    } else if ((pre_sample < 0 && sample < INT16_MIN - pre_sample)) {
        return INT16_MIN; // Underflow
    } else {
        return sample + pre_sample; // Normal mix
    }    
}

/*We scale our divisor up by 1<<precision. We also scale up the nominator later. This improves integer division accuracy. Something about fixed floating points*/
int32_t mixer_calc_volume_div(float v)
{
    if(v>1.0F) return 1<<MIXER_VOLUME_ACCURACY;
    if(v<0.0F) return INT32_MAX-1;
    if((1.0F/v)>=INT32_MAX-1) return INT32_MAX-1;
    return (int32_t)((1.0F/v)*(1<<MIXER_VOLUME_ACCURACY));    
}

void Mixer_Output::set_volume(int channel, float L, float R)
{
    if(channel<0) return;
    if(channel>=MIXER_MAX_CHANNELS) return;

    _volume_divisorsL[channel]=mixer_calc_volume_div(L);
    _volume_divisorsR[channel]=mixer_calc_volume_div(R);
    // const int32_t max = INT32_MAX-1;
    // int32_t divisor=0;
    // if(L>=1.0f){divisor=1;}
    // if((1.0f/L)>=max){divisor=max;}
    // divisor = (int32_t)(1.0f/L);    
    // _volume_divisorsL[channel]=divisor;

    // divisor=0;
    // if(R>=1.0f){divisor=1;}
    // if((1.0f/R)>=max){divisor=max;}
    // divisor = (int32_t)(1.0f/R);    
    // _volume_divisorsR[channel]=divisor;
}

int Mixer_Output::get_sample(int samplesLeft, Mixer_Sample *sample)
{
    for(int i=0;i<MIXER_MAX_CHANNELS;i++)
    {
        /*Get generator and check existance*/
        Audio_Component_Output *component_output=_inputs[i];
        if(component_output==nullptr) continue;

        /*Catch sample*/
        static Mixer_Sample pre_sample;
        component_output->get_sample(samplesLeft, &pre_sample);

        /*Volume mix samples if not zero*/
        int32_t volume_divisorL = _volume_divisorsL[i];
        int32_t volume_divisorR = _volume_divisorsR[i];

        // if(volume_divisor==INT32_MAX)
        // {
        //     sample->L=0;
        //     sample->R=0;
        //     return 0;
        // }
        //if(volume_divisor>1)
        //{

        //}

        //Force stereo
        pre_sample.is_mono=false;
        pre_sample.R = pre_sample.L;

        /*Panning and volume*/
        if(pre_sample.L != 0 && volume_divisorL>1) {
            //sample->L =sample->L / 2.32F;
            pre_sample.L = hw_divider_quotient_s32((pre_sample.L)<<MIXER_VOLUME_ACCURACY, volume_divisorL);
        }
        if (pre_sample.R != 0 && volume_divisorR>1) { 
            //sample->R =sample->R / 2.32F;
            pre_sample.R = hw_divider_quotient_s32((pre_sample.R)<<MIXER_VOLUME_ACCURACY, volume_divisorR);
        }

        /*FIX THIS SEE NOTES*/
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

    return 0;
}