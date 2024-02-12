#include <Arduino.h>
#include <audio_components.h>
#include <audio_delay.h>


/*CONCEPT*/
int Audio_Delay::get_sample(int samplesLeft, Mixer_Sample* sample)
{
    static int pos;
    _input->get_sample(samplesLeft, sample);

    /*Behind*/
    static int32_t olds;
    tape[pos]=sample->L+(tape[pos]/1.05);

    // for(int i=0;i<5;i++)
    // {
    //     int r = random(0,5);
    //     tape[(pos-(i*5)-r)%(sizeof(tape)/4)]=sample->L+(tape[pos]/1.05);
    // }
    




    /*Filter*/
    uint32_t delta = (tape[pos]-olds)/2;
    tape[pos]=tape[pos]-delta;

    olds=tape[pos];
    pos++;
    if(pos>=(sizeof(tape)/4)) pos=0;
    /*Ahead*/
    sample->L=tape[pos];

    return 0;
}

void Audio_Delay::set_input(Audio_Component_Output *component)
{
    if(component!=nullptr) _input = component;
}