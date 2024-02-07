#include <Arduino.h>
#include <map.h>

uint8_t map_read_cell( const uint8_t *map, uint16_t x, uint16_t y )
{
    if(x<0||y<0){return 0xFF;}
    return map[x+(y*MAP_SIZE)];
}