#ifndef _ROUTER_H_
#define _ROUTER_H_

#include "common.h"
#include "map.h"

class IRouter 
{
public:
  virtual bool Create(Map & m) = 0;  
  virtual bool Next() = 0;
  virtual RouteItem getRouteItem() = 0; 
};

#endif