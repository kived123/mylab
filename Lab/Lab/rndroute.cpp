#include <time.h>
#include <stdlib.h>
#include <assert.h>
#include <iostream>

#include "rndroute.h"
#include "common.h"
#include "map.h"

bool RandomRouter::Create(Map& m)
{
  if (pmap!=0)
    return false;  

  pmap = &m;
  srand ((unsigned int)time(NULL));
  return Next();
}




bool RandomRouter::Next()
{
  if (pmap==0)
    return false;        
  
  int steps = 0;
  
  pmap->setDirection(dir);
  if (!pmap->MoveTo(x,y)) 
    return false;
  
  while (!pmap->Wall(FRONT)) {
    steps++;
    
    switch (dir.value())
    {
      case WEST: x--; break;
      case NORTH:y--;break;
      case EAST:x++;break;             
      default:y++;
    }	

    bool test=pmap->MoveTo(x,y);
    assert(test==true);

    if ( !pmap->Wall(RIGHT) || !pmap->Wall(LEFT) )
      if (rand() % 2 == 1) 		
        break;	  
  }

  if (steps==0) {
    return false;
  }

  ri.dist = steps;

  if ( !pmap->Wall(RIGHT) && !pmap->Wall(LEFT) )
    ri.turn = (rand() % 2 == 1) ? RIGHT : LEFT;
  else if ( !pmap->Wall(RIGHT) )
    ri.turn = RIGHT;
  else if  ( !pmap->Wall(LEFT) ) 
    ri.turn = LEFT;
  else 
    ri.turn = BACK;
     
  dir = dir.turn(ri.turn);  

  return true;     
}