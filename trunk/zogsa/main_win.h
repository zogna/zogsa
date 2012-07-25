#ifndef __MAIN_WIN__
#define __MAIN_WIN__


#include <windows.h>		// Header File For Windows
#include <stdio.h>			// Header File For Standard Input/Output

LRESULT	CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);	// Declaration For WndProc
void KillGLWindow(void);

#endif