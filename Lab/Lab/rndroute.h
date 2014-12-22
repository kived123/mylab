#ifndef _RNDROUTE_H_
#define _RNDROUTE_H_

#include "router.h"
#include "direction.h"
#include "common.h"

class Map;

class RandomRouter : public IRouter
{
public:
  RandomRouter( int ax, int ay,int d)
    :pmap(0),x(ax),y(ay),dir(d){}
  virtual bool Create(Map& m);
  virtual bool Next();
  virtual RouteItem getRouteItem()
  {
    return ri;
  }
private:
  Map * pmap;
  int x;
  int y;
  direction dir; 
  RouteItem ri;
};

#endif