#pragma once

#include <map.h>
#include <audio_components.h>
#include <audio_mixer.h>

class SM_sound_source
{
public:
    map_entity* ent;
    Audio_Component_Output* src;
    bool muted=false;
    int ID=0;
};

class SM_listener
{
public:
    map_entity* ent;
    bool muted=false;    
};

class SM_manager : public Audio_Component_Output
{
public:    
    std::vector<SM_sound_source> sources;
    SM_listener listener;

    int init();
    int start();
    int stop();
    int update();
    int add_source(SM_sound_source src);
    int set_listener(SM_listener lst);

    int get_sample(int samplesLeft, Mixer_Sample* sample) override;

private:
    Mixer_Output _mixer;
    int _temp_id=0;
};