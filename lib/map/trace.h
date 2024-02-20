/*Coded by Kevin Witteveen*/

#pragma once
#include <Arduino.h>
#include <map.h>

/*
Takes a map array (as example provided by the map.h library or contained in map_data), 
the x and y position within the map and the x and y direction to measure distance from and to.
This will fire a laser to measure the distance from an empty cell and an occupied cell.

Returns distance as float.
*/
map_vect trace_fire(const uint8_t *map, float x, float y, float x_dir, float y_dir);
map_vect trace_fire(const uint8_t *map, map_vect pos, map_vect dir);

/*Measures the distance between trace firing position and hit as a return vector.*/
map_vect trace_fire_distance(const uint8_t *map, map_vect pos, map_vect dir);