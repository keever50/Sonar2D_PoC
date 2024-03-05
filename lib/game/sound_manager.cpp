#include <sound_manager.h>
#include <audio_mixer.h>
#include <audio_components.h>

int SM_manager::init()
{
    return 0;
}

int SM_manager::update()
{
    map_entity* lst_ent = listener.ent;

    for(SM_sound_source src : sources)
    {
        map_entity* src_ent = src.ent;

        map_vect vec; 
        vec = src_ent->get_pos();
        vec = vec - lst_ent->get_pos();
        vec = vec.normalize();
        float z = lst_ent->get_forward().crossZ(vec);
        float L, R;

        if(z>0)
        {
            R=1;
            L=1.0F-z;
        }else{
            R=1.0F+z;
            L=1;
        }

        //Serial.printf("L%f, R%f, Z%f\n", L, R, z);
        _mixer.set_volume(src.ID, L, R);
    }
    return 0;   
}

int SM_manager::start()
{
    return 0;
}

int SM_manager::set_listener(SM_listener lst)
{
    if(lst.ent==nullptr) return 1;
    listener = lst;

    return 0;
}

int SM_manager::add_source(SM_sound_source src)
{
    if(src.ent==nullptr || src.src==nullptr) return 1;
    auto it = sources.insert(sources.end(), src);
    auto index = std::distance(sources.begin(), it);
    src.ID=_temp_id++;
    _mixer.set_channel(src.ID, src.src);
    if(src.muted==false)
    {
        _mixer.set_volume(src.ID,1.0F,1.0F);
    }

    return 0;    
}


int SM_manager::get_sample(int samplesLeft, Mixer_Sample* sample)
{
    _mixer.get_sample(samplesLeft, sample);
    return 0;    
}