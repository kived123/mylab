#ifndef _PHYS_H_
#define _PHYS_H_

#include "common.h"


class Phys
{
public:
  Phys();
  Phys(float x, float y, int dir);

  void Reset(const RouteItem& ri);
  void Frame(int time);

  bool isUpdate() const {
    return Update;
  }  
   
  float getX() const {
    return X;
  }

  float getY() const {
    return Y;
  }

  float getLookPosX() const {
    return vViewX + X;
  }

  float getLookPosY() const {
    return vViewY + Y;   
  }

  void setVelocity(float v){
    V = v;
  }
   
    
private:
  bool Update;
  bool MoveState;
  bool ResetTimer;
  float X, Y;
  float X0, Y0;
  float XE, YE;
  float vViewX, vViewY;  
  int StartTime;
  float BeginAng, MaxAng;
  float V, VRot;
  int Dir, Turn;
  float Dist;

};

#endif