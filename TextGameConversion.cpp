// TextGameConversion.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "overseer.h"
#include <stdio.h> 
#include <process.h>
#include <windows.h>

int main(int argc, char** argv)
{

	SetConsoleTitle(L"Mapper of Dungeons");
	//system("mode 105, 205");
	
	
	CONSOLE_FONT_INFOEX cfi;
	cfi.cbSize = sizeof(cfi);
	cfi.nFont = 0;
	cfi.dwFontSize.X = 10;                   // Width of each character in the font
	cfi.dwFontSize.Y = 10;                  // Height
	cfi.FontFamily = FF_DONTCARE;
	cfi.FontWeight = FW_NORMAL;
	std::wcscpy(cfi.FaceName, L"Raster Fonts"); // Choose your font
	SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);

	HWND console = GetConsoleWindow();
	RECT r;
	GetWindowRect(console, &r);
	MoveWindow(console, r.left, r.top, WINDOW_WIDTH, WINDOW_HEIGHT, TRUE);


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
}

