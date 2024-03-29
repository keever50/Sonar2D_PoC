#pragma once
#include <audio_components.h>
#include <wav.h>
#include <FS.h>

#define AUDIO_WAV_SRC_MODE_STREAM   0
#define AUDIO_WAV_SRC_MODE_RAM      1

#define AUDIO_WAV_SRC_BUFFER_SIZE   256

/*Todo: Create a more elegant way to check stereo without accessing the stream or audio_info class*/
class Audio_Wav_Source : public Audio_Component_Output
{
public:
    int get_sample(int samplesLeft, Mixer_Sample* sample) override;

    /*Initializes audio source with the required audio information. This information should probably match the output / master properties*/
    int begin(Audio_Info *info);
    /*Takes audio file and based on wether stream is true or false, it will load it either in memory, or stream it directly from the filesystem*/
    int load(File *file, bool stream);
    int play();
    int stop();
    int seek(int pos);
    int pitch(float v);
    /*The higher, the less samples generated. This improves peformance, but lowers quality significantly.*/
    int degradation=1;
private:
    bool _mode=AUDIO_WAV_SRC_MODE_STREAM; 
    bool _loaded=false;
    bool _running=false;
    float _pitch=1.0F;
    int _step_speed=0;
    Wav_PCM_Stream _stream;

    Audio_Info _info;
    File *_file;
    uint32_t _counter;

};