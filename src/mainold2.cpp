// #include <Arduino.h>
// #include <wav.h>
// #include <PWMAudio.h> /*By Earlephilhower. Audio bitrate fix contribution by Kevin Witteveen*/
// #include <LittleFS.h> /*By Earlephilhower*/
// #include <audio_mixer.h>
// #include <pico/stdlib.h>
// #include <audio_delay.h>

// #include <SD.h>
// #include <SDFS.h>

// PWMAudio pwm(0, true);
// Wav_PCM_Stream wav_stream;
// Mixer_Output mixer_output;

// void audio_cb()
// {
//     static Audio_Info info;
//     info.samplerate=32000;
//     info.stereo=true;

//     static unsigned long end;
//     unsigned long start = micros();
//     unsigned long off_time = start - end;
//     for(;;)
//     {
//         int avail = pwm.available();
//         if(avail==0)break;
//         Mixer_Sample sample;
//         sample.L=0;
//         sample.R=0;
//         sample.is_mono=false;
//         mixer_output.get_sample(avail, &sample);
//         pwm.write((int16_t)(sample.L>>16), true);
//         pwm.write((int16_t)(sample.R>>16), true);
//         //pwm.write((int16_t)0, true);
//     }
//     end = micros();
//     Serial.printf("time: %duS. Off time: %dus\n",end-start,off_time);
// }

// class Test_Audio : public Audio_Component_Output
// {
//     virtual int get_sample(int samplesLeft, Mixer_Sample* sample) override
//     {
//         int32_t data = 0;//((int32_t)(wav_stream.read()-0x7F))<<24;
        

//         static int previous_sample_count;
//         static Mixer_Sample previous_sample;

//         if(previous_sample_count==samplesLeft)
//         {
//             *sample=previous_sample;
//             return 0;
//         }

//         //sample->L=((int32_t)(wav_stream.read()-0x7F))<<24;
//         data |= (int32_t)(wav_stream.read())<<0;
//         data |= (int32_t)(wav_stream.read())<<8;
//         data = data <<16;
//         sample->L=data; 
//         data=0;
//         data |= (int32_t)(wav_stream.read())<<0;
//         data |= (int32_t)(wav_stream.read())<<8;
//         data = data <<16;
//         sample->R=data; 

//         sample->is_mono=false;

//         previous_sample=*sample;
//         previous_sample_count=samplesLeft;
//         return 0;
//     }
// };

// Test_Audio audio_source;
// Audio_Delay audio_delay;

// File file0;
// void setup()
// {

//     delay(3000);
//     Serial.begin(250000);

//     //LittleFS.begin();
//     SPI.setSCK(2);
//     SPI.setTX(3);
//     SPI.setRX(4);
//     SPI.setCS(5);
//     SPI.begin(false);
//     SD.begin(5, SPI);
//     SDFSConfig c2;
//     c2.setSPISpeed(125000000);
//     c2.setCSPin(5);
    
//     SDFS.setConfig(c2);
//     SDFS.begin();

//     //static File TEST = LittleFS.open("/train_32k_16b.wav", "r");
//     file0 = SDFS.open("/BBD.wav", "r");
//     if(!file0)
//     {
//         Serial.println("Opening failed");
//     }else{
//        if(wav_stream.begin(&file0))
//        {
//             Serial.println("Loading failed");
//        }else{
        
//         /*Set up delay*/
//         //audio_delay.set_input(&audio_source);

//         /*Set mixer channels*/
//         mixer_output.set_channel(0, &audio_source);
//         //mixer_output.set_channel(1, &audio_delay);

//         /*Set mixer volume*/
//         mixer_output.set_volume(0, 1);
//         //mixer_output.set_volume(1, 0.1f);

//         pwm.setBuffers(4, 256);
//         pwm.begin(16000,100000);
//         pwm.onTransmit(audio_cb);
//        }

//     }

// }

// void loop()
// {
//     delay(500);
//     Serial.printf("temp: %fC\n",analogReadTemp());
// }