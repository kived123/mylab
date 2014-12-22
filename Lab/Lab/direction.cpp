#include <assert.h>
#include "direction.h"


direction::direction(int dir):dir_(dir)
{
  assert(dir>=0 && dir<4);
}

direction direction::right() const
{
  return direction((dir_+1)%4);
}


direction direction::left() const
{
  int d = dir_-1;
  if (d<0) {
    return direction(SOUTH);
  }
  return direction(d);
}

direction direction::opposite() const
{
  return direction((dir_+2)%4);
}

direction direction::turn(int w) const
{  

  switch (w) {
    case RIGHT: return right();
    case LEFT: return left();    
    case BACK: return opposite();
  }

  return direction(dir_);
  
}