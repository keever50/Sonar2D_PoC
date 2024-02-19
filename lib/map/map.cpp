#include <Arduino.h>
#include <map.h>

#define MAP_RADS90D  (PI/2.0F)

uint8_t map_read_cell( const uint8_t *map, uint16_t x, uint16_t y )
{
    if(x<0||y<0){return 0xFF;}
    return map[x+(y*MAP_SIZE)];
}

void map_draw( const uint8_t *map )
{
    int sequential=0;
    for(uint y=0;y<MAP_SIZE;y++)
    {
        Serial.printf("\e[%d;1H", 1+y);
        for(uint x=0; x<MAP_SIZE; x++)
        {
            /*Check if there is a cell. If not, do not draw. But when there is one again, start printing on and after that position*/
            if(map_read_cell(map, x, y))
            {
                if(sequential==0)
                {
                    Serial.printf("\e[%d;%dH", 1+y, 1+x);
                    sequential=1;
                }
                
                Serial.print('#');
            }
            else
            {
                sequential=0;
                //Serial.print(' ');
            }
        }
    }
    Serial.printf("\e[%d;1H", MAP_SIZE);
}



map_vect map_vect::operator+(const map_vect &vect)
{
    map_vect new_vect;
    new_vect.x=this->x+vect.x;
    new_vect.y=this->y+vect.y;
    return new_vect;
}

map_vect map_vect::operator-(const map_vect &vect)
{
    map_vect new_vect;
    new_vect.x=this->x-vect.x;
    new_vect.y=this->y-vect.y;
    return new_vect;
}

map_vect map_vect::operator*(const map_vect &vect)
{
    map_vect new_vect;
    new_vect.x=this->x*vect.x;
    new_vect.y=this->y*vect.y;
    return new_vect;
}

map_vect map_vect::operator*(const float M)
{
    map_vect new_vect;
    new_vect.x=this->x*M;
    new_vect.y=this->y*M;
    return new_vect;
}

void map_entity::_calc_dirs()
{
    _forward.x = sinf(_ang);
    _forward.y = cosf(_ang);

    _left.x = sinf(_ang+MAP_RADS90D);
    _left.y = cosf(_ang+MAP_RADS90D);    
}

void map_entity::move(map_vect dir)
{
    _position = _position + dir;
}

void map_entity::set_pos(map_vect pos)
{
    _position = pos;
}

map_vect map_entity::get_pos()
{
    return _position;
}

void map_entity::set_ang(float deg)
{
    /*Degrees to radians*/
    _ang = (deg*PI)/180.0F;
    _calc_dirs();
}

float map_entity::get_ang()
{
    /*Radians to degrees*/
    return (_ang*180.0F)/PI;
}

map_vect map_entity::get_forward()
{
    return _forward;
}

map_vect map_entity::get_left()
{
    return _left;
}




