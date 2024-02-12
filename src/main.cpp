#include <Arduino.h>
#include <wav.h>
#include <PWMAudio.h> /*By Earlephilhower. Audio bitrate fix contribution by Kevin Witteveen*/
#include <LittleFS.h> /*By Earlephilhower*/
#include <audio_mixer.h>
#include <pico/stdlib.h>
#include <audio_delay.h>
PWMAudio pwm(0, false);
Wav_PCM_Stream wav_stream;
Mixer_Output mixer_output;

void audio_cb()
{
    // static unsigned long end;
    // unsigned long start = micros();
    // unsigned long off_time = start - end;
    for(;;)
    {
        int avail = pwm.available();
        if(avail==0)break;
        Mixer_Sample sample;
        sample.L=0;
        sample.R=0;
        sample.is_mono=true;
        mixer_output.get_sample(avail, &sample);
        pwm.write((int16_t)(sample.L>>16), true);
        //pwm.write((int16_t)0, true);
    }
    // end = micros();
    // Serial.printf("time: %duS. Off time: %dus\n",end-start,off_time);
}

class Test_Audio : public Audio_Component_Output
{
    virtual int get_sample(int samplesLeft, Mixer_Sample* sample) override
    {
        int32_t data = 0;//((int32_t)(wav_stream.read()-0x7F))<<24;

        // data |= (int32_t)(wav_stream.read())<<0;
        // data |= (int32_t)(wav_stream.read())<<8;
        // data = data <<16;
        // sample->L=data; 

        static int previous_sample_count;
        static Mixer_Sample previous_sample;

        if(previous_sample_count==samplesLeft)
        {
            *sample=previous_sample;
            return 0;
        }

        sample->L=((int32_t)(wav_stream.read()-0x7F))<<24;
        sample->is_mono=true;

        previous_sample=*sample;
        previous_sample_count=samplesLeft;
        return 0;
    }
};

Test_Audio audio_source;
Audio_Delay audio_delay;

File file0;
void setup()
{

    // vreg_set_voltage(VREG_VOLTAGE_1_20);
    // set_sys_clock_khz(300000000 / 1000, false);

    // for(int i=0;i<1000000;i++)
    // {
    //     volatile int32_t A = i+5;
    // }

    // set_sys_clock_khz(250000000 / 1000, false);
    // vreg_set_voltage(VREG_VOLTAGE_1_10);
    

    Serial.begin(250000);

    LittleFS.begin();
    
    static File TEST = LittleFS.open("/amen_break_8000hz.wav", "r");
    file0 = LittleFS.open("/train_32k_16b.wav", "r");
    if(!file0)
    {
        Serial.println("Opening failed");
    }else{
       if(wav_stream.begin(&TEST))
       {
            Serial.println("Loading failed");
       }else{
        
        audio_delay.set_input(&audio_source);
        mixer_output.set_channel(0, &audio_source);
        mixer_output.set_volume(0, 0.6f);
        mixer_output.set_channel(1, &audio_delay);
        mixer_output.set_volume(1, 0.4f);

        pwm.setBuffers(4, 100);
        pwm.begin(8000,100000);
        pwm.onTransmit(audio_cb);
       }

    }

}

void loop()
{
    delay(500);
    Serial.printf("temp: %fC\n",analogReadTemp());
}