#include "Main.h"

void InitialiseDebugWindow()
{
	int hConHandle;
	long lStdHandle;
	CONSOLE_SCREEN_BUFFER_INFO coninfo;
	FILE* fp;

	//Allocate a console window
	AllocConsole();

	//Set the screen buffer to be large to allow us to scroll text
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE),(&coninfo));
	coninfo.dwSize.Y = 500;
	SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), coninfo.dwSize);
	
	//redirect stdout
	lStdHandle = (long)GetStdHandle(STD_OUTPUT_HANDLE);
	hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
	fp = _fdopen(hConHandle, "w");
	*stdout = *fp;
	setvbuf(stdout, NULL, _IONBF, 0);

	//redirect stdin
	lStdHandle = (long)GetStdHandle(STD_INPUT_HANDLE);
	hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
	fp = _fdopen(hConHandle, "r");
	*stdin = *fp;
	setvbuf(stdin, NULL, _IONBF, 0);

	//redirect stderr
	lStdHandle = (long)GetStdHandle(STD_ERROR_HANDLE);
	hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
	fp = _fdopen(hConHandle, "w");
	*stderr = *fp;
	setvbuf(stderr, NULL, _IONBF, 0);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevious, LPSTR lpCmdLine, int nCmdShow)
{
	int exitcode = 0;

	if (DEBUG)
	{
		InitialiseDebugWindow();
	}	
	
	Win32Container* core = Win32Container::GetInstance(hInstance);

	exitcode = core->Run();
	
	ExitProcess(0);

	core->DestroyContainer();

	FreeConsole();

	return exitcode;
}