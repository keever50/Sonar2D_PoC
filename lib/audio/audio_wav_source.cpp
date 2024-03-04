#include <audio_wav_source.h>

/*Precision as in sampling precision. Difference in master sample rate and file sample rate has to be adjusted with a specific precision. The higher is better, but takes less long audio files*/
#define AUDIO_WAV_PRECISION 6

int Audio_Wav_Source::begin(Audio_Info *info)
{
    _info = *info;   
    return 0;     
}

int Audio_Wav_Source::load(File *file, bool stream)
{
    _file = file;
    if(_stream.begin(_file)) return 1;
   
    float speed = (float)(_stream.get_sample_rate()) / (float)(_info.samplerate);
    _step_speed = (1<<AUDIO_WAV_PRECISION)*speed*_pitch;

    /*Mono*/
    if(_stream._wav.channels==1)
    {
        _step_speed=_step_speed/2.0F;
    }
    

    _loaded=true;
    return 0;
    
}

int Audio_Wav_Source::play()
{
    _running=true;
    return 0;    
}

int Audio_Wav_Source::stop()
{
    _running=false;
    return 0;    
}

int Audio_Wav_Source::pitch(float v)
{
    if(_pitch==v) return 0;
    _pitch=v;

    float speed = (float)(_stream.get_sample_rate()) / (float)(_info.samplerate);
    _step_speed = (1<<AUDIO_WAV_PRECISION)*speed*_pitch;

    /*Mono*/
    if(_stream._wav.channels==1)
    {
        _step_speed=_step_speed/2.0F;
    }
    return 0;    
}

int Audio_Wav_Source::get_sample(int samplesLeft, Mixer_Sample* sample)
{
    
    static int _counter;
    _counter+=_step_speed;

    /*Modify this to work with multiple bitrates and mono*/
    /*Optimize this. Prevent double reads. Prevent single seeks*/
    _stream.seek((_counter>>AUDIO_WAV_PRECISION)*4);

    int16_t data;
    data |= (int16_t)(_stream.read())<<0;
    data |= (int16_t)(_stream.read())<<8;
    //data = data <<16;
    sample->L=data; 

    /*Dont do this in mono!*/
    if(_stream._wav.channels==2)
    {
        data=0;
        data |= (int16_t)(_stream.read())<<0;
        data |= (int16_t)(_stream.read())<<8;
        //data = data <<16;
        sample->R=data; 
    }else{
        /*Do this in mono tho*/
        sample->is_mono=true;
    }


    return 0;    
}