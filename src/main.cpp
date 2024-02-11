#include <Arduino.h>
#include <wav.h>
#include <PWMAudio.h>
#include <LittleFS.h>
#include <mixer.h>

PWMAudio pwm(0, false);
Wav_PCM_Stream wav_stream;

void audio_cb()
{
    
    while(pwm.available())
    {
        uint16_t data = ((uint16_t)(wav_stream.read()-0x7F))<<8;
        pwm.write(data, true);
    }
}

File file0;
void setup()
{
    Serial.begin();
    LittleFS.begin();

    
    file0 = LittleFS.open("/song12.wav", "r");
    if(!file0)
    {
        Serial.println("Opening failed");
    }else{
       if(wav_stream.begin(&file0))
       {
            Serial.println("Loading failed");
       }else{
        
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