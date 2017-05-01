#pragma once
#include <stdio.h>
#include <io.h>
#include <fcntl.h>

#include <Windows.h>
#include <Windowsx.h>
#include <gl/GL.h>
#include <gl/GLU.h>

#include "Menu.h"
#include "Content.h"
#include "ControlWindowContents.h"

#pragma comment (lib, "opengl32.lib")
#pragma comment (lib, "glu32.lib")

//singleton class containing all win32 and OGL window initialisations
class Win32Container
{
	//pointer to itself
	static Win32Container* pContainer;
	
	//window dimensions
	int windowWidth, windowHeight;

	//Win32Stuff

	HINSTANCE hInstance;
	//the window containing the controls
	HWND controlWindow;	
	//the control window controls
	ControlWindowContents controlContents;
	//the opengl window
	HWND displayWindow;

	//OGlstuff

	HDC hDc;
	PIXELFORMATDESCRIPTOR pfd;
	HGLRC hRc;
	
	//the core of the program
	Content content;

	//true if the program is currently running
	bool running;

public:	
	//create this class and return a pointer to it
	static Win32Container* GetInstance(HINSTANCE _hInstance);
	//destroy this class
	static void DestroyContainer();
	~Win32Container();
	
	//the window procedure for the control window
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	//the window procedure for the OpenGL window
	static LRESULT CALLBACK OGLWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	//run the program
	int Run();
	//end the program
	inline void EndRun() { running = false; }
		
private:
	Win32Container();
	//initialise this class
	void InitContainer(HINSTANCE _hInstance);

	//create the pixel format descriptor for the opengl window
	bool InitialisePixelFormatDescriptor();

	//initialise opengl
	void InitOGL();

	//resize the opengl window
	void Resize(int _width, int _height);	
};

