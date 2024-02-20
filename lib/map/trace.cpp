/*Coded by Kevin Witteveen*/

#include <Arduino.h>
#include <map.h>

map_vect trace_fire(const uint8_t *map, float x, float y, float x_dir, float y_dir)
{
    map_vect vec;
    for(;;)
    {
        vec.x=x;
        vec.y=y;
        if(map_read_cell(map, floor(x), floor(y))) return vec;
        x=x+x_dir; y=y+y_dir;
    }
}

map_vect trace_fire(const uint8_t *map, map_vect pos, map_vect dir)
{
    
    map_vect hit_pos;
    if(dir.x==0 && dir.y==0) return hit_pos;

    float x=pos.x, y=pos.y;
    for(;;)
    {
        hit_pos.x=x;
        hit_pos.y=y;
        if(map_read_cell(map, floor(x), floor(y))) return hit_pos;
        x=x+dir.x; y=y+dir.y;
    }        
}


map_vect trace_fire_distance(const uint8_t *map, map_vect pos, map_vect dir)
{
    map_vect dist;
    if(dir.x==0 && dir.y==0) return dist;

    float x=pos.x, y=pos.y;
    float dx=0, dy=0;
    for(;;)
    {
        dist.x=dx;
        dist.y=dy;
        if(map_read_cell(map, floor(x), floor(y))) return dist;
        x=x+dir.x; y=y+dir.y;
        dx=dx+abs(dir.x); dy=dy+abs(dir.y);
    }        
}