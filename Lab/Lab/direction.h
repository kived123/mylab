#ifndef _DIRECTION_H_
#define _DIRECTION_H_

#include "common.h"

const int    WEST=0;
const int    NORTH=1;
const int    EAST=2;
const int    SOUTH=3;


class direction
{
public:
    direction():dir_(WEST){}
    direction(int dir);
    int value() const
    {
         return dir_;
    }
    direction right() const;
    direction left() const;
    direction opposite() const;
    direction turn(int w) const;
private:
    int dir_;       
};

#endif