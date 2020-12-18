#ifndef MYOGL_H
#define MYOGL_H

#pragma warning( disable : 4305 )
#pragma warning( disable : 4244 )

#include <vector>
#include "MyVector3d.h"

#include <windows.h>
#include <gl/GL.h>
#include "gl/GLU.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include <string>

class OpenGL;


//тип для делегата void f (void)
typedef void(*action)(OpenGL *) ;
typedef void(*MouseEventDelegate)(OpenGL *,int,int);
typedef void(*WheelEventDelegate)(OpenGL *, int);
typedef void(*KeyEventDelegate)(OpenGL*, int);
typedef void(*ResizeEventDelegate)(OpenGL *, int, int);

class Camera;
class Light;

class OpenGL
{
	HWND g_hWnd;
	HDC g_hDC;
	HGLRC g_hRC;
	int width, height;
	

	HBITMAP bitmap;
	HDC bitmap_hdc;
	HDC dc;

	void createBitmap();
	

public:

	

	//функции рендера
	std::vector < action > renderFunc;

	//render GUI

	action renderGuiFunc;
	ResizeEventDelegate resizeFunc;

	//функции инициализации
	std::vector < action > initFunc;
	
	std::vector <MouseEventDelegate>  mouseFunc;
	std::vector <WheelEventDelegate>  wheelFunc;
	std::vector <KeyEventDelegate>  keyDownFunc;
	std::vector <KeyEventDelegate>  keyUpFunc;

	Camera *mainCamera;
	Light *mainLight;

	int getWidth()
	{
		return width;
	}

	int getHeight()
	{
		return height;
	}
	HWND getHwnd()
	{
		return g_hWnd;
	}

	OpenGL();
	~OpenGL();

	void setHWND(HWND window);
	

	int OldMouseX, OldMouseY;
	void mouseMovie(int mX, int mY);
	void wheelEvent(float delta);
	void keyDownEvent(int key);
	void keyUpEvent(int key);

	void DrawAxes();

	void render();
	void resize(int w, int h);
	void init(void);

	double aspect;

	std::string message;

	static bool isKeyPressed(int key)
	{
		
		//стандартная функция WinApi для чтения нажатой клавиши
		//см https://msdn.microsoft.com/ru-ru/library/windows/desktop/ms646293(v=vs.85).aspx
		
		short state = GetAsyncKeyState(key);


		//  0x8000 припереводе из 16ричной  в двоичную давет 1000 0000 0000 0000 .
		//  операция & совершает побитовое И над операндами.    Таким образом после нее остется первый бит (0 или 1),
		//  в нем и содержится информация о там нажата ли клавиша
		return static_cast<bool>(state & 0x8000);
	}

	//загрузка BMP из файла,
	
	
	static void drawSphere()
	{

	}

};







#endif