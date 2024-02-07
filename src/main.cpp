#include <Arduino.h>
#include <logging.h>
#include <trace.h>
#include <wav.h>

#include <PWMAudio.h>
#include <LittleFS.h>

#include <map.h>
#include <map_00_test.h>

#define scale 65535
#define steps 2048 /*32 steps per scale*/

wav_pcm_s wav_pcm;

PWMAudio pwm(0, false); // GP0 = left, GP1 = right
volatile float d=0;

void audio_callback()
{
  static int32_t p;
  while(pwm.availableForWrite())
  {
    pwm.write((wav_pcm.data[p]-128)*256);
    p++;
    if(p>=wav_pcm.data_size) p=0;
  }
}

void setup() {
  Serial.begin();

  LittleFS.begin();
  pwm.setBuffers(4, 32); // Give larger buffers since we're are 48khz sample rate
  pwm.onTransmit(audio_callback);
  pwm.begin(22000);

  File file = LittleFS.open("/02-approved_mlem.wav", "r");

  if(!file)
  {
    Serial.println("Opening failed");
  }else{
    wav_load_pcm(file, &wav_pcm);
    wav_print_pcm_info(&wav_pcm);
    delay(3000);
    Serial.println(wav_pcm.sample_rate);
    
  }
  file.close();


}

float a=0;
void loop() {
  
  


}