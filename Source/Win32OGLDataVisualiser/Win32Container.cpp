#include "Win32Container.h"

Win32Container* Win32Container::pContainer = NULL;

Win32Container* Win32Container::GetInstance(HINSTANCE _hInstance)
{
	if (pContainer == NULL)
	{
		pContainer = new Win32Container();
		pContainer->InitContainer(_hInstance);
	}
	return pContainer;
}

void Win32Container::DestroyContainer()
{
	if (pContainer)
	{
		delete(pContainer);
	}
}

void Win32Container::InitContainer(HINSTANCE _hInstance)
{
	//Set the window dimensions
	windowWidth = 1000;
	windowHeight = 600;

	//set hInstance
	hInstance = _hInstance;

	//create and register the window class
	WNDCLASS wc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.hCursor = LoadCursor(hInstance, IDC_ARROW);
	wc.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
	wc.hInstance = hInstance;
	wc.lpfnWndProc = WndProc;
	wc.lpszClassName = TEXT("WIN");
	wc.lpszMenuName = NULL;
	wc.style = CS_VREDRAW | CS_HREDRAW;
	if (!RegisterClass(&wc))
	{
		MessageBox(NULL, TEXT("Error: cannot register window class!"), TEXT("ERROR!"), MB_OK);
	}
	printf("Window Class Init Successful!\n");

	//create and register the window class for the OpenGL window
	WNDCLASS wcOGL;
	wcOGL.cbClsExtra = 0;
	wcOGL.cbWndExtra = 0;
	wcOGL.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wcOGL.hCursor = LoadCursor(hInstance, IDC_ARROW);
	wcOGL.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
	wcOGL.hInstance = hInstance;
	wcOGL.lpfnWndProc = OGLWndProc;
	wcOGL.lpszClassName = TEXT("GL");
	wcOGL.lpszMenuName = NULL;
	wcOGL.style = CS_VREDRAW | CS_HREDRAW;
	if (!RegisterClass(&wcOGL))
	{
		MessageBox(NULL, TEXT("Error: cannot register window class!"), TEXT("ERROR!"), MB_OK);
	}
	printf("Window Class Init Successful!\n");

	//Create the menu handle
	HMENU menuHandle = LoadMenu(hInstance, MAKEINTRESOURCE(ID_MENU));

	//Create the control window
	controlWindow = CreateWindow(wc.lpszClassName, TEXT("Data Visualiser: Controls"), WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME | WS_VISIBLE, 0, 0, windowWidth, windowHeight, NULL, menuHandle, hInstance, NULL);
	if (controlWindow == NULL)
	{
		MessageBox(NULL, TEXT("Control Window Init Error:"), TEXT("ERROR!"), MB_OK);
	}
	printf("Control Window Init Successful!\n");
	
	//Create the display window
	displayWindow = CreateWindow(wcOGL.lpszClassName, TEXT("Data Visualiser: Visualisation"), WS_OVERLAPPEDWINDOW | WS_VISIBLE, 0, 0, windowWidth, windowHeight, NULL, NULL, hInstance, NULL);
	if (displayWindow == NULL)
	{
		MessageBox(NULL, TEXT("Display Window Init Error:"), TEXT("ERROR!"), MB_OK);
	}
	printf("Display Window Init Successful!\n");	

	hDc = GetDC(displayWindow);
	InitialisePixelFormatDescriptor();

	hRc = wglCreateContext(hDc);
	wglMakeCurrent(hDc, hRc);	

	InitOGL();

	Resize(windowWidth, windowHeight);		

	controlContents = ControlWindowContents(&controlWindow);
}

Win32Container::Win32Container()
{
	windowWidth = 0;
	windowHeight = 0;
}

Win32Container::~Win32Container()
{

}

bool Win32Container::InitialisePixelFormatDescriptor()
{
	ZeroMemory(&pfd, sizeof(pfd));
	pfd.nSize = sizeof(pfd);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 24;
	pfd.cDepthBits = 16;
	pfd.iLayerType = PFD_MAIN_PLANE;
	int iFormat = ChoosePixelFormat(hDc, &pfd);
	SetPixelFormat(hDc, iFormat, &pfd);
	if (&pfd == NULL)
	{
		//Note: come back and find out how to sort this
		MessageBox(NULL, TEXT("Error: pixel format initialisation failure"), TEXT("ERROR!"), MB_OK);
		return 0;
	}
	printf("Pixel Format Descriptor Init Successful!\n");
	return 1;
}

void Win32Container::InitOGL()
{
	glClearColor(0.0f,0.0f,0.0f,0.0f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}

int Win32Container::Run()
{
	MSG msg;
	running = true;

	content.SetControlContents(&controlContents);
	content.CreateText();

	while (running)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				EndRun();
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}			
		}
		else
		{
			content.Render(hDc);
		}
	}

	return (int)msg.wParam;
}

LRESULT CALLBACK Win32Container::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CREATE:		
		break;
	//React to exiting
	case WM_CLOSE:
	case WM_QUIT:
	case WM_DESTROY:
		pContainer->EndRun();
		break;

	//React to Menu Input
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_EXIT:
			ExitProcess(0);
			break;
		default:
			pContainer->content.ReadControlInput(wParam);
			break;
		}
		break;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT CALLBACK Win32Container::OGLWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CREATE:
		break;
		//React to exiting
	case WM_CLOSE:
	case WM_QUIT:
	case WM_DESTROY:
		pContainer->EndRun();
		break;

		//React to resizing
	case WM_SIZE:
		pContainer->Resize(LOWORD(lParam), HIWORD(lParam));
		break;		

	case WM_MOUSEMOVE:
		pContainer->content.MoveMouse(GET_X_LPARAM(lParam) - (float)pContainer->windowWidth / 2.0f, -1.0f * (GET_Y_LPARAM(lParam) - (float)pContainer->windowHeight / 2.0f));
		break;

	case WM_LBUTTONUP:
		pContainer->content.LeftButtonUp();
		break;
	case WM_LBUTTONDOWN:
		POINT mouse;                        // Stores The X And Y Coords For The Current Mouse Position
		GetCursorPos(&mouse);                   // Gets The Current Cursor Coordinates (Mouse Coordinates)
		ScreenToClient(hWnd, &mouse);

		GLfloat winX, winY, winZ;               // Holds Our X, Y and Z Coordinates

		winX = (float)mouse.x;                  // Holds The Mouse X Coordinate
		winY = (float)mouse.y;                  // Holds The Mouse Y Coordinate
		pContainer->content.LeftButtonDown();
		break;

	case WM_RBUTTONUP:
		pContainer->content.RightButtonUp();
		break;
	case WM_RBUTTONDOWN:
		pContainer->content.RightButtonDown();
		break;

	case WM_MOUSEWHEEL:
		pContainer->content.MouseScroll(HIWORD(wParam));
		break;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

void Win32Container::Resize(int width, int height)
{
	pContainer->windowWidth = width;
	pContainer->windowHeight = height;
	glViewport(0, 0, width, height);	

	content.ResizeWindow(width, height);
}




