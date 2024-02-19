/*Coded by Kevin Witteveen*/

#include <Arduino.h>
#include <map.h>

map_vect trace_distance(const uint8_t *map, float x, float y, float x_dir, float y_dir)
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