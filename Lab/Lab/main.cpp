#include <windows.h>
#include <GL/gl.h>
#include <mmsystem.h>

using namespace std;
#include "direction.h"
#include "common.h"
#include "map.h"
#include "router.h"
#include "rndroute.h"
#include "phys.h"
#include "renderer.h"



void MainLoop();
void Frame();
bool InitGlWindow(HINSTANCE hi, int nCmdShow);
LONG WINAPI WndProc (HWND hwnd, UINT msg, WPARAM  wp,LPARAM  lp);
void GLRelease(HWND hwnd);
bool Init();

HDC   g_hdc;
HGLRC g_hglrc;


#define WIDTH  800
#define HEIGHT 600
const int MAPWIDTH=9;
const int MAPHEIGHT=9;   


Map map;
IRouter * router = 0;
Phys * phys = 0;
renderer * r = 0;

int cells[]=
{	0,0,0,0,0,0,0,0,0,
	0,3,1,1,2,1,1,3,2,
        0,2,1,1,0,3,0,2,2,
        0,2,1,1,2,2,1,0,2,
        0,1,1,1,0,1,1,1,0,
        0,3,1,3,2,1,1,1,2,
        0,2,2,0,2,3,1,0,2,
        0,3,0,1,0,2,1,1,2,
        0,1,1,1,0,1,1,1,0
};

void Frame()
{
       
    if (!phys->isUpdate())
    {
        phys->Frame(timeGetTime());        
    } 
    else
    {
        RouteItem ri = router->getRouteItem();
        phys->Reset(ri);
        router->Next();
        phys->Frame(timeGetTime());  
    }
    r->setPos(phys->getX(),phys->getY());
    r->setLookAt(phys->getLookPosX(),phys->getLookPosY());
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );    
    r->Frame();   

    SwapBuffers(g_hdc);  
    
}


bool InitGlWindow(HINSTANCE hi, int nCmdShow)
{
    WNDCLASS wc;
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc =  (WNDPROC)WndProc;
    wc.cbClsExtra=0;
    wc.cbWndExtra=0;
    wc.hInstance = hi;
    wc.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor (NULL,IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1); 
    wc.lpszMenuName  = NULL;  
    wc.lpszClassName = L"LABWNDCLS";
    
    if (!RegisterClass (&wc) )   { 
       	MessageBox(NULL, L"Register class failed", L"Error", MB_OK); 
    	return false; 
    } 
    HWND hwnd;   
    hwnd = CreateWindow (L"LABWNDCLS", 
            L"OGL", 
            WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 
            CW_USEDEFAULT, 
            CW_USEDEFAULT, 
            WIDTH, 
            HEIGHT,
            NULL,
            NULL,
            hi,
            NULL);  
    
    if (!hwnd) {
        MessageBox(NULL, L"Cretae Windows failed", L"Error", MB_OK); 
    	return false; 
    } 
    
    g_hdc = GetDC(hwnd);
    
    PIXELFORMATDESCRIPTOR pfd; 
     
    ZeroMemory(&pfd, sizeof(PIXELFORMATDESCRIPTOR));
    
    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR); 
    pfd.nVersion = 1; 
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL |  
            PFD_DOUBLEBUFFER; 
    pfd.dwLayerMask = PFD_MAIN_PLANE; 
    pfd.iPixelType = PFD_TYPE_RGBA; 
    pfd.cColorBits = 32; 
    pfd.cDepthBits = 16; 
    pfd.cAccumBits = 0; 
    pfd.cStencilBits = 0; 
    
    int pixfmt = ChoosePixelFormat(g_hdc, &pfd);  
    
    if ( (pixfmt = ChoosePixelFormat(g_hdc, &pfd)) == 0 ) 
    {
        MessageBox(NULL, L"ChoosePixelFormat failed", L"Error", MB_OK); 
        return false; 
    }
    
    if (SetPixelFormat(g_hdc, pixfmt, &pfd) == FALSE) 
    {
        MessageBox(NULL, L"SetPixelFormat failed", L"Error", MB_OK); 
        return false; 
    } 
    	 

    g_hglrc = wglCreateContext(g_hdc); 
    wglMakeCurrent(g_hdc, g_hglrc); 



    ShowWindow (hwnd, nCmdShow);  
    UpdateWindow (hwnd); 
    


    return true;
}

LONG WINAPI WndProc (HWND hwnd, UINT msg, WPARAM  wp,LPARAM  lp)
{
    RECT rect;
    switch(msg)
    {
        case WM_SIZE:
            GetClientRect(hwnd, &rect); 
            r->resize(rect.right, rect.bottom);
            break;	

	case WM_DESTROY:
            GLRelease(hwnd);
            PostQuitMessage(0);
            
            break;
	default:
	    return DefWindowProc(hwnd, msg, wp, lp);
    }
    
     return 0;
}

void MainLoop()
{
    MSG  msg; 
    
    while (1) 
    {
        while (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE) == TRUE) 
        { 
            if (GetMessage(&msg, NULL, 0, 0) ) 
            { 
                TranslateMessage(&msg); 
                DispatchMessage(&msg); 
            }
            else 
            {
                return; 
            }
        } 
	Frame(); 
    } 
}

void GLRelease(HWND hwnd)
{
    if (g_hglrc)
    { 
	wglDeleteContext(g_hglrc); 
    }
    
    if (g_hdc) 
    {
	ReleaseDC(hwnd, g_hdc); 
    }

    
}

int WINAPI WinMain (HINSTANCE hi, HINSTANCE hpi, LPSTR lpCmdLine, int nCmdShow) 
{
    if (!Init()) 
      return -1;   
 
    if (!InitGlWindow(hi, nCmdShow))
        return -1;
    r->init(&map);
    MainLoop();
    return 0;
}



bool Init()
{
    map_buffer_desc mpd;
    mpd.height = MAPHEIGHT;
    mpd.width = MAPWIDTH;
    mpd.pData = cells;
    
    
    map.LoadFromMemory(mpd);
    
    router =new RandomRouter(0,0,EAST) ;  
    if (!router->Create(map)) {
       MessageBox(0, L"Create rout failed", L"Error", MB_OK);
      return false;
    }

    phys = new Phys(1.5f,1.5f, EAST);
    r = new renderer(0.0f,0.0f,3.0f);
   
    return true;
}