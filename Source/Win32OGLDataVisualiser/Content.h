#pragma once
#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <iostream>

#include "Menu.h"
#include "Data.h"
#include "GraphicsBase.h"
#include "ControlWindowContents.h"

//store the location of the mouse and the state of it's buttons
struct MouseData
{
	bool LMB;
	bool RMB;
	float x;
	float y;
};

//stores the core of the program
class Content
{
	//stores the data
	Data currentFile;
	//points to the current graph
	GraphicsBase* display;
	//stores the previous mouse data
	MouseData lastMouse;	

	//points to the control window controls
	ControlWindowContents* controlContents;

public:
	Content();
	~Content();

	//get a pointer to the controls
	void SetControlContents(ControlWindowContents* givenCC);
	//initialise the display's selected text
	void CreateText();

	//render to the opengl window
	void Render(HDC _hDc);
	//get input from the windows procedures
	void ReadControlInput(WPARAM wParam);
private:
	//reads input from the console window, allows for new systems to be implemented without requiring a UI to interface with them
	void ReadDebugInput();

public:
	//react to mouse inputs
	void MoveMouse(float x, float y);
	void LeftButtonDown();
	void LeftButtonUp();
	void RightButtonDown();
	void RightButtonUp();
	void MouseScroll(int amount);

	//react to window resizing
	void ResizeWindow(int newWidth, int newHeight);	
};

