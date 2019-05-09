// TextGameConversion.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "overseer.h"
#include <stdio.h> 
#include <process.h>
#include <windows.h>
#include <windowsx.h>



// the WindowProc function prototype
LRESULT CALLBACK WindowProc(HWND hWnd,
	UINT message,
	WPARAM wParam,
	LPARAM lParam);

// the entry point for any Windows program
int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	// the handle for the window, filled by a function
	HWND hWnd;
	// this struct holds information for the window class
	WNDCLASSEX wc;

	// clear out the window class for use
	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	// fill in the struct with the needed information
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.lpszClassName = L"WindowClass1";

	// register the window class
	RegisterClassEx(&wc);

	// create the window and use the result as the handle
	hWnd = CreateWindowEx(NULL,
		L"WindowClass1",    // name of the window class
		L"Windows Conversion",   // title of the window
		WS_OVERLAPPEDWINDOW,    // window style
		300,    // x-position of the window
		300,    // y-position of the window
		500,    // width of the window
		400,    // height of the window
		NULL,    // we have no parent window, NULL
		NULL,    // we aren't using menus, NULL
		hInstance,    // application handle
		NULL);    // used with multiple windows, NULL

				  // display the window on the screen
	ShowWindow(hWnd, nCmdShow);

	// enter the main loop:

	// this struct holds Windows event messages
	MSG msg;

	// wait for the next message in the queue, store the result in 'msg'
	while (GetMessage(&msg, NULL, 0, 0))
	{
		// translate keystroke messages into the right format
		TranslateMessage(&msg);

		// send the message to the WindowProc function
		DispatchMessage(&msg);
	}

	// return this part of the WM_QUIT message to Windows
	return msg.wParam;
}

// this is the main message handler for the program
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	// sort through and find what code to run for the message given
	switch (message)
	{
		// this message is read when the window is closed
	case WM_DESTROY:
	{
		// close the application entirely
		PostQuitMessage(0);
		return 0;
	} break;
	}

	// Handle any messages the switch statement didn't
	return DefWindowProc(hWnd, message, wParam, lParam);
}

/*int main(int argc, char** argv)
{
	
	SetConsoleTitle(L"Mapper of Dungeons");
	//system("mode 105, 205");
	
	
	CONSOLE_FONT_INFOEX cfi;
	cfi.cbSize = sizeof(cfi);
	cfi.nFont = 0;
	cfi.dwFontSize.X = 8;                   // Width of each character in the font
	cfi.dwFontSize.Y = 9;                  // Height
	cfi.FontFamily = FF_DONTCARE;
	cfi.FontWeight = FW_NORMAL;
	std::wcscpy(cfi.FaceName, L"Raster Fonts"); // Choose your font
	SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);

	HWND console = GetConsoleWindow();
	RECT r;
	GetWindowRect(console, &r);
	MoveWindow(console, r.left, r.top, WINDOW_WIDTH - 157, WINDOW_HEIGHT - 61, TRUE);


	HANDLE loggerPipe;
	HANDLE storyPipe;
	DWORD dwWritten;
	DWORD dwWritten_s;

	storyPipe = CreateFile(TEXT("\\\\.\\pipe\\Story"),
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		0,
		NULL);
	loggerPipe = CreateFile(TEXT("\\\\.\\pipe\\Pipe"),
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		0,
		NULL);
	if (storyPipe != INVALID_HANDLE_VALUE) {
		WriteFile(storyPipe, "Welcome.\n", 10, &dwWritten_s, NULL);
	}
	if (loggerPipe != INVALID_HANDLE_VALUE)
	{
		WriteFile(loggerPipe,
			"hello\n",
			7,   // = length of string + terminating '\0' !!!
			&dwWritten,
			NULL);

		
	}

	

	Overseer* game = new Overseer(loggerPipe, storyPipe);

	if (loggerPipe != INVALID_HANDLE_VALUE)
	{
		WriteFile(loggerPipe,
			"exit program\n",
			14,   // = length of string + terminating '\0' !!!
			&dwWritten,
			NULL);

		CloseHandle(loggerPipe);
	}
	if (storyPipe != INVALID_HANDLE_VALUE) {
		WriteFile(storyPipe, "exit program\n", 14, &dwWritten_s, NULL);
		CloseHandle(storyPipe);
	}
	
	return 0;
}*/

