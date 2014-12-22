#ifndef _COMMON_H_
#define _COMMON_H_


const int FRONT=100;
const int LEFT=101;
const int RIGHT=102;
const int REAR=103;
const int BACK=500;
const float CELL_SIDE = 3.0f;

struct RouteItem
{
    int dist;
    int turn;
};

struct map_buffer_desc
{
    int width;
    int height;
    int * pData;
};



#endif
