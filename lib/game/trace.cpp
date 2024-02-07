#include <Arduino.h>
#include <map.h>

float trace_distance(const uint8_t *map, float x, float y, float x_dir, float y_dir)
{
    for(;;)
    {
        if(map_read_cell(map, x, y)) return sqrtf(powf(x,2)+powf(y,2));
        x=x+x_dir; y=y+y_dir;
    }
}