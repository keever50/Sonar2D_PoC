#include <Arduino.h>

#include <map_00_test.h>
#include <map.h>
#include <trace.h>

#include <audio_components.h>
#include <audio_mixer.h>
#include <audio_wav_source.h>
#include <PWMAudio.h> /*By Earlephilhower. Audio bitrate and modulation fix contribution by Kevin Witteveen*/
#include <sound_manager.h>
#include <audio_wav_RAM_source.h>

#include <LittleFS.h>
#include <SD.h> 
#include <SDFS.h> 

/*Controls*/
#define JOYSTICK_INNER_DEADZONE 0.06F
#define CONTROLS_AIM        A0
#define CONTROLS_WALK       A1
#define CONTROLS_STRAVE     A2

/*Audio*/
#define AUDIO_MODULATION_FREQUENCY      200'000
#define AUDIO_MASTER_SAMPLE_RATE        8'000
PWMAudio audio_pwm_output(0, true);
Audio_wav_ram_source wav_src;
Audio_Wav_Source wav_src2;
Audio_Wav_Source wav_src3;
Mixer_Output mixer_master;
SM_manager sm_manager;

void audio_cb()
{
    static unsigned long end;
    Serial.printf("between time: %duS\n",micros()-end);
    unsigned long start = micros();
    for(;;)
    {
        int avail = audio_pwm_output.available();
        if(avail==0)break;
        Mixer_Sample sample;
        mixer_master.get_sample(avail, &sample);
        audio_pwm_output.write((int16_t)(sample.L), true);
        audio_pwm_output.write((int16_t)(sample.R), true);
        //pwm.write((int16_t)0, true);
    }
    end = micros();
    unsigned long delta = end - start;
    Serial.printf("sound time: %duS\n\n",delta);
}
///

float joystick(pin_size_t pin)
{
    float in = (float)(analogRead(pin)-512)/512.0F;
    if(in>JOYSTICK_INNER_DEADZONE || in<-JOYSTICK_INNER_DEADZONE) return in;
    else return 0.0F;
}

/*Debug*/
map_vect non_persistent_cells[32];

void draw_non_persistent_cell(map_vect pos, char c, int ID)
{
    map_vect* old_pos = &non_persistent_cells[ID];

    /*Skip when no change*/
    if(old_pos->x==pos.x && old_pos->y==pos.y) return;
    /*Remove old*/
    Serial.printf("\e[%d;%dH ",1+(int)old_pos->y,1+(int)old_pos->x);   
    /*Place new*/
    Serial.printf("\e[%d;%dH%c",1+(int)pos.y,1+(int)pos.x, c); 
    /*Remember old*/
    non_persistent_cells[ID] = pos;
}

void draw_ent(map_entity *ent)
{
    map_vect vect;
    vect = ent->get_pos();
    Serial.printf("\e[32m"); /*Green ON*/
    draw_non_persistent_cell(vect, 'X', 0);

    vect = ent->get_pos()+(ent->get_forward()*3.0f);
    Serial.printf("\e[31m"); /*Red ON*/
    draw_non_persistent_cell(vect, 'F', 1);

    vect = ent->get_pos()+(ent->get_left()*3.0f);
    Serial.printf("\e[34m"); /*Blue ON*/
    draw_non_persistent_cell(vect, 'L', 2);

    /*Test trace*/
    map_vect hit = trace_fire(MAP_00_test, ent->get_pos(), ent->get_forward());
    Serial.printf("\e[35m"); /*Red ON*/
    draw_non_persistent_cell(hit, 'H', 3);

    Serial.printf("\e[0m"); /*Reset*/
}

void show_inputs()
{
    Serial.printf("Joysticks: %f, %f, %f\n", joystick(CONTROLS_STRAVE),joystick(CONTROLS_WALK),joystick(CONTROLS_AIM));
}
///

map_entity player(MAP_00_test);
map_entity test_ent(MAP_00_test);
map_entity test_ent2(MAP_00_test);
void setup()
{
    while(!Serial);
    /*Watch dog to reset the microcontroller when its crashing*/
    

    Serial.begin(115200);   
    Serial.print("\e[25l");
    
    map_vect pos;
    pos.x=10;
    pos.y=10;
    player.set_pos(pos);

    pos.x=50; pos.y=50;
    test_ent2.set_pos(pos);

    /*FS*/
    SPI.setSCK(2);
    SPI.setTX(3);
    SPI.setRX(4);
    SPI.setCS(5);
    SPI.begin(false);
    SD.begin(5, SPI);
    LittleFSConfig c2;
    //c2.setSPISpeed(125'000'000);
    //c2.setCSPin(5);
    
    LittleFS.setConfig(c2);
    LittleFS.begin();

    /*Audio*/
    static File file0 = LittleFS.open("/amen.wav", "r");
    if(!file0)
    {
        while(true){ delay(500); Serial.println("No file"); }
    }

    // static File file1 = LittleFS.open("/SONAR.wav", "r");
    // if(!file1)
    // {
    //     while(true){ delay(500); Serial.println("No file"); }
    // }

    // static File file2 = LittleFS.open("/drip_16b.wav", "r");
    // if(!file2)
    // {
    //     while(true){ delay(500); Serial.println("No file"); }
    // }


    audio_pwm_output.setBuffers(4, 100);
    audio_pwm_output.begin(AUDIO_MASTER_SAMPLE_RATE,AUDIO_MODULATION_FREQUENCY);
    audio_pwm_output.onTransmit(audio_cb);
    ///


    /*Source*/
    Audio_Info inf;
    inf.samplerate=AUDIO_MASTER_SAMPLE_RATE;
    inf.stereo=true;

    
    wav_src.begin(&inf);
    int err = wav_src.load(&file0);
    while(err) delay(500);
    

    // wav_src2.begin(&inf);
    // wav_src2.load(&file1, true);
    // //wav_src2.pitch(0.5);

    // wav_src3.begin(&inf);
    // wav_src3.load(&file2, true);
    // //wav_src3.pitch(1.2);

    SM_listener listener;
    listener.ent=&player;
    listener.muted=false;
    sm_manager.set_listener(listener);

    // SM_sound_source src;
    // src.ent=&test_ent;
    // src.src=&wav_src;
    // src.muted=false;
    // sm_manager.add_source(src);

    // SM_sound_source src2;
    // src2.ent=&test_ent2;
    // src2.src=&wav_src2;
    // src2.muted=false;
    // sm_manager.add_source(src2);



    //sm_manager.start();

    mixer_master.set_channel(0, &wav_src);
    //mixer_master.set_channel(1, &wav_src2);
    //mixer_master.set_channel(2, &wav_src3);
    mixer_master.set_volume(0,0.7F,0.9F);
    //mixer_master.set_volume(1,0.5F,0.5F);
    //mixer_master.set_volume(2,1.0F,0.9F);

    /*Vibration*/
    // analogWriteFreq(22'000);
    // analogWrite(21, 0);
    // analogWriteFreq(22'000);
    // analogWrite(9, 0);    
    
    rp2040.wdt_begin(1000);
}


void loop()
{
    static unsigned long next_map_draw;
    if(millis()>=next_map_draw)
    {
        //map_draw(MAP_00_test);
        //draw_ent(&player);
        next_map_draw=next_map_draw+2000;
    }
    
    static float ply_ang;
    ply_ang=ply_ang+joystick(CONTROLS_AIM)*1.0F;
    player.set_ang(ply_ang);

    if(wav_src.error)
    {
        Serial.printf("wav err\n");
    }
    

    //sm_manager.update();

    // map_vect vec;
    // vec.x=0;
    // vec.y=1;
    // float z =player.get_forward().crossZ(vec);
    // float L, R;

    // if(z>0)
    // {
    //     R=1;
    //     L=1.0F-z;
    // }else{
    //     R=1.0F+z;
    //     L=1;
    // }



    //Serial.printf("L%f, R%f, Z%f\n", L, R, z);
    //map_vect vec_dist = trace_fire_distance(MAP_00_test, player.get_pos(), player.get_forward());

    //wav_src.pitch( sqrt( pow(vec_dist.x,2)+pow(vec_dist.y,2) )/25.0F );
   // mixer_master.set_volume(0, L,R);

    rp2040.wdt_reset();
    delay(500);
}