#ifndef RENDERER_H
#define	RENDERER_H

class Map;

class renderer
{
public:
	renderer()  {};
        renderer(float x, float y, float Depth):StartX_(x),StartY_(y),
             Depth_(Depth){}
        void init(Map * m);
        
        
        void setPos(float posx, float posy)
	{
		posx_=posx;
                posy_=posy;
	}
        
	void setLookAt(float atx, float aty)
	{
		atx_=atx;
                aty_=aty;
	}
        
	void resize(int w, int h) ;
	void Frame();
private:
	float posx_, posy_;
	float atx_, aty_;
	float aspect_;
        float StartX_, StartY_, Depth_;
        Map * pmap;
	void setupMatrices();
};

#endif