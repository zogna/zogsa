#ifndef __ZOGII_GL__
#define __ZOGII_GL__


#include <windows.h>		// Header File For Windows
#include <stdio.h>			// Header File For Standard Input/Output
#include <gl\gl.h>			// Header File For The OpenGL32 Library

int winOpenFileJPG(wchar_t *buf, int len);
int winOpenFileTXT(wchar_t *buf, int len);

void ReSizeGLScene(GLsizei width, GLsizei height);

void InitGL(void);
void DrawGLScene(void);
void clearGL(void);


#endif