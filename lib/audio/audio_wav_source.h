#pragma once
#include <audio_components.h>
#include <wav.h>
#include <FS.h>

#define AUDIO_WAV_SRC_MODE_STREAM   0
#define AUDIO_WAV_SRC_MODE_RAM      1

#define AUDIO_WAV_SRC_BUFFER_SIZE   256

class Audio_Wav_Source : public Audio_Component_Output
{
public:
    int begin(Audio_Info *info);
    int load(File *file);
    int play();
    int stop();
    int seek(int pos);
    int pitch(float v);
    /*The higher, the less samples generated. This improves peformance, but lowers quality significantly.*/
    int degradation=1;
private:
    sqrt
    bool _mode=AUDIO_WAV_SRC_MODE_STREAM; 
    bool _loaded=false;
    bool _running=false;
    File *_file;

};