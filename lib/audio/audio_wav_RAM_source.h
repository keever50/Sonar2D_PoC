#pragma once
#include <audio_components.h>
#include <wav.h>
#include <FS.h>

#define AUDIO_WAV_RAM_PRECISION 5


class Audio_wav_ram_source : public Audio_Component_Output
{
public:
    int begin(Audio_Info *info);
    int get_sample(int samplesLeft, Mixer_Sample* sample) override;
    int load(File* file);
    int close();

    ~Audio_wav_ram_source()
    {
        close();
    }

    int error=0;
private:
    File* _file;
    wav_pcm_s _wav;
    Audio_Info _info;
    int _step_speed;
    float _pitch=1.0F;
    bool _initialized=false;
    bool _loaded=false;
    int32_t _counter;
    int32_t _counter_max;
    uint32_t _samplesize;
};