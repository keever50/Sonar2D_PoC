// #include <Arduino.h>
// #include <logging.h>
// #include <trace.h>
// #include <wav.h>

// #include <PWMAudio.h>
// #include <LittleFS.h>

// #include <mixer.h>

// #include <map.h>
// #include <map_00_test.h>

// #define scale 65535
// #define steps 2048 /*32 steps per scale*/

// wav_pcm_s wav_pcm0;
// wav_pcm_s wav_pcm1;
// wav_pcm_s wav_pcm2;

// PWMAudio pwm(0, true); // GP0 = left, GP1 = right

// Mixer_Output mixer_output;

// File test_stream;

// Mixer_AudioGenerator audioGenerator0;
// void audioGenerator0_cb(int samples_todo, Mixer_Sample* sample)
// {
//   static int p;
  
//   sample->L = (uint32_t)(wav_pcm0.data[p]-0x7F)<<24;
//   sample->R = (uint32_t)(wav_pcm0.data[p]-0x7F)<<24;

//   p++;
//   if(p>=wav_pcm0.data_size)
//   {
//     p=0;
//   }
// }

// Mixer_AudioGenerator audioGenerator1;
// void audioGenerator1_cb(int samples_todo, Mixer_Sample* sample)
// {
//   static int p;
//   sample->L = (uint32_t)(wav_pcm1.data[p]-0x7F)<<24;
//   sample->R = (uint32_t)(wav_pcm1.data[p]-0x7F)<<24;

//   p++;
//   if(p>=wav_pcm1.data_size)
//   {
//     p=0;
//   }  
// }

// Mixer_AudioGenerator audioGenerator2;
// void audioGenerator2_cb(int samples_todo, Mixer_Sample* sample)
// {
//   static int p;
//   sample->is_mono=true;
//   sample->L = (int32_t)(wav_pcm2.data[p]-0x7F)<<24;

//   p++;
//   if(p>=wav_pcm2.data_size)
//   {
//     p=0;
//   }  
// }

// void pwm_cb()
// {
//   for(;;)
//   {
//     int avail = pwm.available();
//     if(avail<=0) break;
//     static Mixer_Sample sample;
//     sample.L=0;
//     sample.R=0;
//     mixer_output.callback(avail, &sample);
// /*TODO CHECK STEREO*/
//     pwm.write((int16_t)(sample.L>>16), true);
//     pwm.write((int16_t)(sample.R>>16), true);
//   }
  
// }

// void setup() {
//   Serial.begin();

//   LittleFS.begin();


//   File file0 = LittleFS.open("/underground.wav", "r");
//   if(!file0)
//   {
//     Serial.println("Opening failed");
//   }else{
//     if(!wav_load_pcm(file0, &wav_pcm0))
//     {
//       audioGenerator0.audioGenerator_cb=audioGenerator0_cb;
//       audioGenerator0.setVolume(0.5);
//       mixer_output.set_channel(0, &audioGenerator0);
//     }

//   }
//   file0.close();

//   File file1 = LittleFS.open("/alarm.wav", "r");
//   if(!file1)
//   {
//     Serial.println("Opening failed");
//   }else{
//     if(!wav_load_pcm(file1, &wav_pcm1))
//     {
//       audioGenerator1.audioGenerator_cb=audioGenerator1_cb;
//       audioGenerator1.setVolume(0.01f);
//       mixer_output.set_channel(1, &audioGenerator1);
//     }
//   }
//   file1.close();

//   File file2 = LittleFS.open("/heli.wav", "r");
//   if(!file2)
//   {
//     Serial.println("Opening failed");
//   }else{
//     if(!wav_load_pcm(file2, &wav_pcm2))
//     {
//       audioGenerator2.audioGenerator_cb=audioGenerator2_cb;
//       audioGenerator2.setVolume(0.1);
//       mixer_output.set_channel(2, &audioGenerator2);
//     }
//   }
//   file2.close();

//   pwm.setBuffers(4, 100); // Give larger buffers since we're are 48khz sample rate
//   pwm.begin(11025,100000);
//   pwm.onTransmit(pwm_cb);

// }

// float a=0;
// void loop() {
  
  
//   delay(10);


// }