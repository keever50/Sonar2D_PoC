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
float trace_distance(const uint8_t *map, float x, float y, float x_dir, float y_dir);