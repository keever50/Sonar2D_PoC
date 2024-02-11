#include <Arduino.h>
#include <wav.h>
#include <PWMAudio.h>
#include <LittleFS.h>
#include <mixer.h>

PWMAudio pwm(0, false);
Wav_PCM_Stream wav_stream;
Mixer_Output mixer_output;

void audio_cb()
{
    
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
    }
}

class Test_Audio : public Audio_Component_Output
{
    virtual int get_sample(int samplesLeft, Mixer_Sample* sample) override
    {
        int32_t data = ((int32_t)(wav_stream.read()-0x7F))<<24;
        sample->L=data; 
        sample->is_mono=true;
        return 0;
    }
};

Test_Audio audio_source;

File file0;
void setup()
{
    Serial.begin();
    LittleFS.begin();

    
    file0 = LittleFS.open("/song20.wav", "r");
    if(!file0)
    {
        Serial.println("Opening failed");
    }else{
       if(wav_stream.begin(&file0))
       {
            Serial.println("Loading failed");
       }else{
        
        mixer_output.set_channel(0, &audio_source);
        mixer_output.set_volume(0, 1.0f);

        pwm.setBuffers(4, 100);
        pwm.begin(11025,100000);
        pwm.onTransmit(audio_cb);
       }

    }

}

void loop()
{
    delay(500);
}