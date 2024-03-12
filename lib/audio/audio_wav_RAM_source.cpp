#include <audio_wav_RAM_source.h>
#include <wav.h>
#include <logging.h>

int Audio_wav_ram_source::begin(Audio_Info *info)
{
    if(_initialized==false)
    {
        LOG_DEBUG("WAV_RAM:begin...");
        if(info==nullptr) 
        {
            LOG_ERROR("WAV_RAM:begin null info");
            return 1;
        }

        if(info->samplerate<=0)
        {
            LOG_ERROR("WAV_RAM:begin <=0 samplerate");
            return 1;
        }

        _info = *info;
        _initialized=true;
        return 0;
    }

    return 1;
}

int Audio_wav_ram_source::load(File* file)
{
    if(_initialized==true && _loaded==false)
    {
        LOG_DEBUG("WAV_RAM:load...");
        _file = file;
        if(_file == nullptr)
        {
            LOG_ERROR("WAV_RAM:load null file"); 
            return 1;
        }
   
        int err = wav_load_pcm(*_file,&_wav);
        if(err) return 1;

        wav_print_pcm_info(&_wav);

        /*Calculate sample size*/
        _samplesize=_wav.channels*(_wav.bits_per_sample/8);

        /*Calculate sample speed*/
        float speed = (float)(_wav.sample_rate) / (float)(_info.samplerate);
        _step_speed = (1<<AUDIO_WAV_RAM_PRECISION)*speed*_pitch*_samplesize;   /*Fixed floating point counter steps*/



        _counter_max=(_wav.data_size-_samplesize)<<AUDIO_WAV_RAM_PRECISION;      

        _loaded=true;
        return 0;

    }
    if(_initialized==false) LOG_DEBUG("WAV_RAM:load not initialized");

    return 1;
}

int Audio_wav_ram_source::close()
{
    LOG_DEBUG("WAV_RAM:unloading");
    int err = wav_unload_pcm(&_wav);    
    if(err) return 1;
    _loaded=false;
    return 0;
}

uint16_t inline audio_wav_ram_amplify_u8_u16(uint8_t b)
{
    
    uint16_t bb=b;
    bb|=b<<8;
    return bb;
}

int16_t inline audio_wav_ram_tosign_u16_s16(uint16_t bb)
{
    if(bb<0x7FFF) return bb-0x7FFF;
    return bb>>1;
}

int Audio_wav_ram_source::get_sample(int samplesLeft, Mixer_Sample* sample)
{
    _counter+=_step_speed;
    /*Limit counter and rollover*/
    if(_counter>=_counter_max) _counter=_counter-_counter_max;
    if(_counter<0) _counter=_counter+_counter_max;

    //static int seek;
    uint32_t seek = ((uint32_t)_counter>>AUDIO_WAV_RAM_PRECISION); /*Fixed floating point. Lower precision so we round into the correct addresses*/

    /*Safety*/
    if(seek>=_wav.data_size-_samplesize || seek<0)
    {
        error=1;
        return 1;
    }

    uint16_t pre_sample;

    /*Get L and R, including extra bits*/
    /*16 bits*/
    if(_wav.bits_per_sample==16)
    {
        pre_sample=_wav.data[seek++];
        pre_sample|=(_wav.data[seek++]<<8);
        sample->L=(int16_t)pre_sample;

        if(_wav.channels>1)
        {
            pre_sample=0;
            pre_sample=_wav.data[seek++];
            pre_sample|=(_wav.data[seek++]<<8);
            sample->R=(int16_t)pre_sample;

            sample->is_mono=false;
        }else 
        {
            sample->is_mono=true;
        }
    }
    
    /*TODO: Add 8 bit support. Also 24 and 32 bit if possible.*/
    if(_wav.bits_per_sample==8)
    {
        //sample->L=audio_wav_ram_tosign_u16_s16(audio_wav_ram_amplify_u8_u16(_wav.data[seek++]));

        sample->L=((int16_t)_wav.data[seek++] - 128) * 256;

        if(_wav.channels>1)
        {
            sample->R=sample->L=((int16_t)_wav.data[seek++] - 128) * 256;
            sample->is_mono=false;           
        }else{
            sample->is_mono=true;
        }

    }

    

    return 0;
}

int Audio_wav_ram_source::pitch(float p)
{
    if(_pitch==p) return 0;
    _pitch=p;

    /*Calculate sample speed*/
    float speed = (float)(_wav.sample_rate) / (float)(_info.samplerate);
    _step_speed = (1<<AUDIO_WAV_RAM_PRECISION)*speed*_pitch*_samplesize;   /*Fixed floating point counter steps*/

    return 0;    
}