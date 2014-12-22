#include <windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include "renderer.h"
#include "direction.h"
#include "map.h"


const int IMAGE_WIDTH = 256;
const int IMAGE_HEIGHT = 256;

static void hwall(float x, float y,float z, float Depth_);
static void vwall(float x, float y,float z, float Depth_);
static void xwall(float x, float y,float z, float Depth_);


void renderer::resize(int w, int h) 
{
	
	glViewport(0,0,w,h);
	aspect_ = (float)w/(float)h;
}

void renderer::init(Map * m)
{
    pmap=m;
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.3f,0.1f,0.7f,0.0f);   
    HBITMAP hImage = (HBITMAP)LoadImage(0,L"../Lab/lab.bmp",IMAGE_BITMAP,0,0,
            LR_LOADFROMFILE | LR_CREATEDIBSECTION);
    if (hImage==0) return;
    
    BITMAP bm;
   
   
    GetObject(hImage,sizeof(bm),&bm);
    
    
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(
        	GL_TEXTURE_2D,
          	0,
          	GL_RGB,
         	IMAGE_WIDTH,
          	IMAGE_HEIGHT,
          	0,
          	32992,
          	GL_UNSIGNED_BYTE,
            bm.bmBits
            
     );

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
   

    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glEnable(GL_TEXTURE_2D);

}

static void xwall(float x, float y,float z, float Depth_)
{
     glBegin(GL_QUADS);
     glVertex3f(x,z+Depth_,y);    
     glVertex3f(x, z+Depth_, y+CELL_SIDE);      
     glVertex3f(x+CELL_SIDE,z+Depth_, y+CELL_SIDE);      
     glVertex3f(x+CELL_SIDE, z+Depth_, y);                
      
     glEnd();
}

static void hwall(float x, float y,float z, float Depth_)
{
    glBegin(GL_QUADS);
    glVertex3f(x,z,y);
    glTexCoord2d(1.0f,1.0f); 
    glVertex3f(x,z+Depth_, y);
    glTexCoord2d(0.0f,1.0f); 
    glVertex3f(x+CELL_SIDE, z+Depth_, y);
    glTexCoord2d(0.0f,0.0f); 
    glVertex3f(x+CELL_SIDE, z, y);                
    glTexCoord2d(1.0f,0.0f); 
    glEnd();
}

static void vwall(float x, float y,float z, float Depth_)
{
    
    glBegin(GL_QUADS);
    glVertex3f(x,z,y);
    glTexCoord2d(1.0f,1.0f); 
    glVertex3f(x,z+Depth_, y);
    glTexCoord2d(0.0f,1.0f); 
    glVertex3f(x, z+Depth_, y+CELL_SIDE);
    glTexCoord2d(0.0f,0.0f);
    glVertex3f(x, z, y+CELL_SIDE);               
    glTexCoord2d(1.0f,0.0f);
    glEnd();
}

void renderer::Frame()
{
	setupMatrices();
        
	float x=StartX_, y=StartY_, z=-0.5f*Depth_;
      //  glRotated(0.1,1.0,1.0,1.0);
	for(int j=0; j<pmap->Height();j++) {
            for (int i=0; i<pmap->Width();i++) {
                pmap->MoveTo(i,j);
                glColor3f(1.0,1.0,1.0);
                //NORTH WALL
                if (pmap->Wall(NORTH)){
                    hwall(x,y,z,Depth_);
                    
                }
                //WEST WALL
                if (pmap->Wall(WEST)){
                    vwall(x,y,z,Depth_);
                }
                
          
                glColor3f(1.0,1.0,0.5);
                xwall(x,y,z,0.0f);
                glColor3f(0.0,1.0,0.0);
             //   xwall(x,y,z,Depth_);
                
                x+=CELL_SIDE;
            }
            x=StartX_;
            y+=CELL_SIDE;
        }
	
        x=StartX_;
        y=StartY_+pmap->Height() * CELL_SIDE;
        glColor3f(1.0,1.0,1.0);
        for (int i=0; i<pmap->Width(); i++){
      
          hwall(x,y,z,Depth_);
          x+=CELL_SIDE;
        }
        
        
        x=StartX_+ pmap->Width() * CELL_SIDE;
        y=StartY_;
        
        for (int i=0; i<pmap->Height(); i++){
          vwall(x,y,z,Depth_);
          y+=CELL_SIDE;
        }   
        
}

void renderer::setupMatrices()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45,aspect_,1,100);
	gluLookAt(posx_, 0, posy_,atx_,0,aty_,0,1,0);
        //gluLookAt(30,50,30,12,0,12,0,1,0);
	glMatrixMode(GL_MODELVIEW);
}