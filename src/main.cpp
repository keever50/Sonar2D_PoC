#include <Arduino.h>
#include <map_00_test.h>
#include <map.h>
#include <trace.h>

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

    map_vect dist = trace_distance(MAP_00_test, ent->get_pos().x, ent->get_pos().y, ent->get_forward().x, ent->get_forward().y);
    Serial.printf("\e[35m"); /*Red ON*/
    draw_non_persistent_cell(dist, 'H', 3);


    Serial.printf("\e[0m"); /*Reset*/



}

map_entity player;

void setup()
{
    Serial.begin(250000);   
    Serial.print("\e[25l");

    map_vect pos;
    pos.x=50;
    pos.y=50;
    player.set_pos(pos);
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
    player.set_ang(A);
    player.move(player.get_forward()*vel.y);
    player.move(player.get_left()*vel.x);

    draw_ent(&player);

    Serial.printf("\e[H\n");
    delay(100);
}