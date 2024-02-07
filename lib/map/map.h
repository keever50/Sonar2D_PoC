#ifndef __MAPS_H
#define __MAPS_H
#include <Arduino.h>

#define MAP_SIZE    128 /*both width and height*/

/*Reads a cell from the map and returns its raw value*/
uint8_t map_read_cell( const uint8_t *map, uint16_t x, uint16_t y );

#endif