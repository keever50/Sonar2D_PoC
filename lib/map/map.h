#ifndef __MAPS_H
#define __MAPS_H
#include <Arduino.h>

#define MAP_SIZE    128 /*both width and height*/

/*Reads a cell from the map and returns its raw value*/
uint8_t map_read_cell( const uint8_t *map, uint16_t x, uint16_t y );

void map_draw( const uint8_t *map );

class map_vect
{
public:
    float x=0, y=0;
    map_vect operator+(const map_vect &vect);
    map_vect operator-(const map_vect &vect);
    map_vect operator*(const map_vect &vect);
    map_vect operator*(const float M);
};

class map_entity
{
public:
    /*A relative move from its position. It will check for collisions.*/
    void move(map_vect dir);
    /*An absolute move, which sets the position. It will not check for collisions.*/
    void set_pos(map_vect pos);
    map_vect get_pos();
    /*Sets the angle of the entity in degrees and calculate its directions*/
    void set_ang(float deg);
    float get_ang();

    map_vect get_forward();
    map_vect get_left();
    
    char symbol='X';

private:
    map_vect _position;
    map_vect _forward;
    map_vect _left;
    /*Radians*/
    float _ang=45.0F;

    void _calc_dirs();
};

#endif