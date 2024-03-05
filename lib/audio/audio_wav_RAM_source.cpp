#include <audio_wav_RAM_source.h>

int Audio_wav_ram_source::begin(Audio_Info *info)
{
    _info = *info;
    return 0;
}

int Audio_wav_ram_source::load(File* file)
{
    float speed = (float)(_wav.sample_rate) / (float)(_info.samplerate);
    _step_speed = (1<<AUDIO_WAV_RAM_PRECISION)*speed*_pitch;    


    return 0;
}