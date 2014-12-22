#ifndef _MAP_H_
#define _MAP_H_

#include <string>
#include "direction.h"
#include "common.h"

class Map
{
public:
    Map();
    bool LoadFromMemory(const map_buffer_desc& mbd);
    bool LoadFromFile(const std::string& fname){
       /*UNIMPLEMENT*/
       return false;
    }

    
    bool MoveTo(int x, int y);
    bool Wall(int side) const;

    void setDirection(const direction& d)
    {
       dir = d;
    }

    int Width() const
    {
       return width;  
    }

    int Height() const
    {
       return  height;     
    }
    ~Map();   
private:
    int ** cells;
    int width;
    int height; 
    bool walls[4];
    direction dir;
};

#endif
