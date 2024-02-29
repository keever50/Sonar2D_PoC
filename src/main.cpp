#include <Arduino.h>

#include <map_00_test.h>
#include <map.h>
#include <trace.h>

#include <audio_components.h>
#include <audio_mixer.h>
#include <audio_wav_source.h>
#include <PWMAudio.h> /*By Earlephilhower. Audio bitrate and modulation fix contribution by Kevin Witteveen*/

#include <LittleFS.h>
#include <SD.h> 
#include <SDFS.h> 

/*Audio*/
#define AUDIO_MODULATION_FREQUENCY      100000
#define AUDIO_MASTER_SAMPLE_RATE        8000
PWMAudio audio_pwm_output(0, true);
Audio_Wav_Source wav_src;
Mixer_Output mixer_master;

void audio_cb()
{
    for(;;)
    {
        int avail = audio_pwm_output.available();
        if(avail==0)break;
        Mixer_Sample sample;
        mixer_master.get_sample(avail, &sample);
        audio_pwm_output.write((int16_t)(sample.L>>16), true);
        audio_pwm_output.write((int16_t)(sample.R>>16), true);
        //pwm.write((int16_t)0, true);
    }
}
///

/*Controls*/
#define JOYSTICK_INNER_DEADZONE 10
void joystick(int* X, int* Y)
{
    int x=analogRead(A1)-510;
    int y=analogRead(A0)-510;

    if(abs(x)>JOYSTICK_INNER_DEADZONE)
    {
        *X=x;
    }else{
        *X=0;
    }

    if(abs(y)>JOYSTICK_INNER_DEADZONE)
    {
        *Y=y;
    }else{
        *Y=0;
    }

}
///

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
///

map_entity player(MAP_00_test);
void setup()
{
    Serial.begin(115200);   
    Serial.print("\e[25l");
    
    map_vect pos;
    pos.x=10;
    pos.y=10;
    player.set_pos(pos);

    /*FS*/
    SPI.setSCK(2);
    SPI.setTX(3);
    SPI.setRX(4);
    SPI.setCS(5);
    SPI.begin(false);
    SD.begin(5, SPI);
    SDFSConfig c2;
    c2.setSPISpeed(125000000);
    c2.setCSPin(5);
    
    SDFS.setConfig(c2);
    SDFS.begin();

    /*Audio*/
    static File file0 = SDFS.open("/BBD.wav", "r");
    if(!file0)
    {
        while(true){ delay(500); Serial.println("No file"); }
    }
    audio_pwm_output.setBuffers(4, 256);
    audio_pwm_output.begin(AUDIO_MASTER_SAMPLE_RATE,AUDIO_MODULATION_FREQUENCY);
    audio_pwm_output.onTransmit(audio_cb);

    Audio_Info inf;
    inf.samplerate=AUDIO_MASTER_SAMPLE_RATE;
    inf.stereo=true;
    wav_src.begin(&inf);

    wav_src.load(&file0, true);

    mixer_master.set_channel(0, &wav_src);
    mixer_master.set_volume(0,1.0F,1.0F);

    analogWriteFreq(22000);
    analogWrite(21, 80);

    analogWriteFreq(22000);
    analogWrite(9, 80);    
    
}


void loop()
{
    static unsigned long next_map_draw;
    if(millis()>=next_map_draw)
    {
        map_draw(MAP_00_test);
        next_map_draw=next_map_draw+2000;
    }
    

    int X,Y;
    joystick(&X,&Y);
    map_vect vel;
    vel.x=-((float)X/100.0f);
    vel.y=((float)Y/100.0f);
    
    static float A;
    A=A+10;
    //player.set_ang(A);
    player.move(player.get_forward()*vel.y);
    //player.move(player.get_left()*vel.x);
    player.set_ang(player.get_ang()+vel.x*4.0f);

    draw_ent(&player);

    Serial.printf("\e[H\n");

    map_vect vec;
    vec.x=0; vec.y=1;
    float panning = player.get_forward().crossZ(vec);
    vec.x=1; vec.y=0;
    float front = 0.5+(1+player.get_forward().crossZ(vec))/4.0;

    float L,R;
    if(panning>0){
        R=(1.0-panning);
        L=1.0;
    }else{
        R=1.0;
        L=(1.0+panning);
    }
    //mixer_master.set_volume(0,L*front, R*front);
    Serial.printf("cross %f\n", panning);

    delay(100);
}