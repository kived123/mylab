#include <cmath>
#include <stdio.h>

#include "direction.h"
#include "phys.h"




const float  vViewLen = 0.5f;
const float  kTime = 0.05f;
const float M_PI = 3.14159f;

static float Dir2Ang(int Dir);

static float Dir2Ang(int Dir)
{
  switch (Dir) {
    case WEST:
      return M_PI;
    case NORTH:
      return 0.5f*M_PI;
    case EAST:
      return 0.0f;
  }
  
  return 1.5f*M_PI;
}

Phys::Phys() : X(0.0f), Y(0.0f), Dir(EAST), Update(true), 
                     V(0.05f), VRot(0.05f)
{
}

Phys::Phys(float x, float y, int dir) :
                             X(x), Y(y), Dir(dir), Update(true),
                             V(0.1f), VRot(0.05f)
{
}
 

void Phys::Reset(const RouteItem& ri)
{
  if ( ! Update )
    return;
  
  Update = false;
  
  Dist = CELL_SIDE*ri.dist;

  XE=X; YE=Y;
  vViewX=0.0f; vViewY=0.0f;

  switch ( Dir ) {
    case WEST: 
      XE = X - Dist;
      vViewX = -vViewLen;
      break;

    case NORTH:
      YE = Y - Dist;
      vViewY = -vViewLen;
      break;

    case EAST:
      XE = X + Dist;
      vViewX = vViewLen;
      break;
 
    default:
      YE = Y + Dist;
      vViewY = vViewLen;          
   }
     
  

   BeginAng =  Dir2Ang(Dir);
   MaxAng = (ri.turn == BACK ? M_PI : 0.5f*M_PI);
   Turn = ri.turn;
   
   X0 = X;
   Y0 = Y;
   MoveState = true;
   ResetTimer = true;   
}


void Phys::Frame(int time)
{
  if ( Update )
    return;
  
  if ( ResetTimer) {
    StartTime = time;
    ResetTimer = false; 
  }
  
  float t = kTime * (time - StartTime);
  float dx=0.0f;
  float dy=0.0f;
 
  if ( MoveState) {
    switch (Dir) {
      case WEST:  dx = - V*t;  break;
      case NORTH:  dy = - V*t; break;      
      case EAST:   dx = V*t; break;
      default: dy = V*t; break;
     }

    if (std::abs(dx) >= Dist || std::abs(dy) >= Dist) {       
       X=XE; Y=YE;
       ResetTimer =true;
       MoveState = false;
       return ;
    }
    X = X0 + dx;
    Y = Y0 + dy;
  } else { /*Rotate state*/ 
      float da=0.0f;
      switch(Turn)
      {
          case LEFT:
          case BACK:  da = VRot * t; break;
          default: da = -VRot *t;break;
      }
      
      if (std::abs(da) >= MaxAng)
      {
          Dir = direction(Dir).turn(Turn).value();
          Update = true;
          return;
      }
    
     float Ang = BeginAng + da;
     vViewX = vViewLen * cos(Ang);
     vViewY = -vViewLen * sin(Ang);    
  }
}