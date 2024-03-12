// #include <Arduino.h>

// #include <map_00_test.h>
// #include <map.h>
// #include <trace.h>

// #include <audio_components.h>
// #include <audio_mixer.h>
// #include <audio_wav_source.h>
// #include <PWMAudio.h> /*By Earlephilhower. Audio bitrate and modulation fix contribution by Kevin Witteveen*/
// #include <sound_manager.h>
// #include <audio_wav_RAM_source.h>

// #include <LittleFS.h>
// #include <SD.h> 
// #include <SDFS.h> 

// /*Controls*/
// #define JOYSTICK_INNER_DEADZONE 0.06F
// #define CONTROLS_AIM        A0
// #define CONTROLS_WALK       A1
// #define CONTROLS_STRAVE     A2

// /*Audio*/
// #define AUDIO_MODULATION_FREQUENCY      200'000
// #define AUDIO_MASTER_SAMPLE_RATE        16'000
// PWMAudio audio_pwm_output(0, true);
// Audio_wav_ram_source wav_src;
// Audio_wav_ram_source wav_src2;
// Audio_wav_ram_source wav_src3;
// Mixer_Output mixer_master;
// SM_manager sm_manager;

// void audio_cb()
// {
//     static unsigned long end;
//     //Serial.printf("between time: %duS\n",micros()-end);
//     unsigned long start = micros();
//     for(;;)
//     {
//         int avail = audio_pwm_output.available();
//         if(avail==0)break;
//         Mixer_Sample sample;
//         sm_manager.get_sample(avail, &sample);
//         audio_pwm_output.write((int16_t)(sample.L), true);
//         audio_pwm_output.write((int16_t)(sample.R), true);
//         //pwm.write((int16_t)0, true);
//     }
//     end = micros();
//     unsigned long delta = end - start;
//     //Serial.printf("sound time: %duS\n\n",delta);
// }
// ///

// float joystick(pin_size_t pin)
// {
//     float in = (float)(analogRead(pin)-512)/512.0F;
//     if(in>JOYSTICK_INNER_DEADZONE || in<-JOYSTICK_INNER_DEADZONE) return in;
//     else return 0.0F;
// }

// /*Debug*/
// map_vect non_persistent_cells[32];

// void draw_non_persistent_cell(map_vect pos, char c, int ID)
// {
//     map_vect* old_pos = &non_persistent_cells[ID];

//     /*Skip when no change*/
//     if(old_pos->x==pos.x && old_pos->y==pos.y) return;
//     /*Remove old*/
//     Serial.printf("\e[%d;%dH ",1+(int)old_pos->y,1+(int)old_pos->x);   
//     /*Place new*/
//     Serial.printf("\e[%d;%dH%c",1+(int)pos.y,1+(int)pos.x, c); 
//     /*Remember old*/
//     non_persistent_cells[ID] = pos;
// }

// void draw_ent(map_entity *ent)
// {
//     map_vect vect;
//     vect = ent->get_pos();
//     Serial.printf("\e[32m"); /*Green ON*/
//     draw_non_persistent_cell(vect, 'X', 0);

//     vect = ent->get_pos()+(ent->get_forward()*3.0f);
//     Serial.printf("\e[31m"); /*Red ON*/
//     draw_non_persistent_cell(vect, 'F', 1);

//     vect = ent->get_pos()+(ent->get_left()*3.0f);
//     Serial.printf("\e[34m"); /*Blue ON*/
//     draw_non_persistent_cell(vect, 'L', 2);

//     /*Test trace*/
//     map_vect hit = trace_fire(MAP_00_test, ent->get_pos(), ent->get_forward());
//     Serial.printf("\e[35m"); /*Red ON*/
//     draw_non_persistent_cell(hit, 'H', 3);

//     Serial.printf("\e[0m"); /*Reset*/
// }

// void show_inputs()
// {
//     Serial.printf("Joysticks: %f, %f, %f\n", joystick(CONTROLS_STRAVE),joystick(CONTROLS_WALK),joystick(CONTROLS_AIM));
// }
// ///

// map_entity player(MAP_00_test);
// map_entity test_ent(MAP_00_test);
// map_entity test_ent2(MAP_00_test);

// void sonar()
// {
//     map_vect vec_dist = trace_fire_distance(MAP_00_test, player.get_pos(), player.get_forward());
//     wav_src3.pitch( sqrt( pow(vec_dist.x,2)+pow(vec_dist.y,2) )/25.0F );    
// }

// void vibrate()
// {
//     map_vect vec_distL = trace_fire_distance(MAP_00_test, player.get_pos(), player.get_left());    
//     float distL = sqrt( pow(vec_distL.x,2)+pow(vec_distL.y,2) );

//     map_vect empty;
//     map_vect vec_distR = trace_fire_distance(MAP_00_test, player.get_pos(), empty-player.get_left());    
//     float distR = sqrt( pow(vec_distR.x,2)+pow(vec_distR.y,2) );

//     /*Vibration*/
//     analogWriteFreq(22'000);

//     if(distR<2) analogWrite(21, 200);
//     else analogWrite(21, 0);
    
//     if(distL<2) analogWrite(9, 200); 
//     else analogWrite(9, 0);   
    

// }

// /*.wav into RAM*/
// int load_sound( map_entity *ent, String path, SM_sound_source *src, float loudness, bool global  )
// {
//     static Audio_Info inf;
//     inf.samplerate=AUDIO_MASTER_SAMPLE_RATE;
//     inf.stereo=true;

//     /*Open file*/
//     static File f = LittleFS.open(path, "r");
//     if(!f) return 1;

//     /*Create wav*/
//     //auto wav = std::make_shared<Audio_wav_ram_source>();
//     Audio_wav_ram_source* wav = new Audio_wav_ram_source;
//     wav->begin(&inf);
//     int err = wav->load(&f);
//     //f.close();
//     if(err) return 1;

//     /*Fill in source class*/
//     src->ent=ent;
//     src->src=wav;
//     src->muted=false;
//     src->loudness=loudness;
//     src->global=global;
//     sm_manager.add_source(*src);

//     return 0;
// }

// void demo()
// {
//     static Audio_Info inf;
//     inf.samplerate=AUDIO_MASTER_SAMPLE_RATE;
//     inf.stereo=true;


// }

// void setup()
// {
//     while(!Serial);
//     delay(500);


//     Serial.begin(115200);   
//     Serial.print("\e[25l");
    
//     map_vect pos;
//     pos.x=20;
//     pos.y=20;
//     player.set_pos(pos);

//     pos.x=100; pos.y=100;
//     test_ent2.set_pos(pos);

//     pos.x=4; pos.y=4;
//     test_ent.set_pos(pos);

//     /*FS*/
//     SPI.setSCK(2);
//     SPI.setTX(3);
//     SPI.setRX(4);
//     SPI.setCS(5);
//     SPI.begin(false);
//     SD.begin(5, SPI);
//     LittleFSConfig c2;
//     //c2.setSPISpeed(125'000'000);
//     //c2.setCSPin(5);
    
//     LittleFS.setConfig(c2);
//     LittleFS.begin();




//     audio_pwm_output.setBuffers(4, 100);
//     audio_pwm_output.begin(AUDIO_MASTER_SAMPLE_RATE,AUDIO_MODULATION_FREQUENCY);
//     audio_pwm_output.onTransmit(audio_cb);
//     ///



//     SM_listener listener;
//     listener.ent=&player;
//     listener.muted=false;
//     sm_manager.set_listener(listener);

//     static map_entity ent0 = map_entity(MAP_00_test);
//     map_vect vec;
//     vec.x=10;
//     vec.y=10;
//     ent0.set_pos(vec);
//     static SM_sound_source SM_src0;
//     int err = load_sound(&ent0, "/SONAR_RAM", &SM_src0, 1, true);
//     while(err);

//     sm_manager.start();

//     //mixer_master.set_channel(0, &sm_manager);
//     //mixer_master.set_channel(1, &wav_src3);
//     // //mixer_master.set_channel(2, &wav_src3);
//     //mixer_master.set_volume(0,1,1);
//     //mixer_master.set_volume(1,0.1,0.1);
//     //mixer_master.set_volume(2,1.0F,0.9F);

 
    
//     rp2040.wdt_begin(200);
// }


// void loop()
// {
//     static unsigned long next_map_draw;
//     if(millis()>=next_map_draw)
//     {
//         map_draw(MAP_00_test);
        
//         next_map_draw=next_map_draw+2000;
//     }
    
    
//     //draw_ent(&test_ent);
//     //show_inputs();

//     static float ply_ang;
//     ply_ang=ply_ang+joystick(CONTROLS_AIM)*10.0F;
//     player.set_ang(ply_ang);

//     map_vect move;
//     move = player.get_forward()*joystick(CONTROLS_WALK)*2.0F;
//     move = move + player.get_left()*joystick(CONTROLS_STRAVE)*2.0F;
//     player.move(move);

//     sm_manager.update();
//    // sonar();
//     vibrate();

//     draw_ent(&player);
//     rp2040.wdt_reset();
//     delay(100);
// }