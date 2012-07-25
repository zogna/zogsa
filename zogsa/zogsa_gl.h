#ifndef __ZOGII_GL__
#define __ZOGII_GL__


#include <windows.h>		// Header File For Windows
#include <stdio.h>			// Header File For Standard Input/Output
#include <gl\gl.h>			// Header File For The OpenGL32 Library


void ReSizeGLScene(GLsizei width, GLsizei height);

void GLkey(WPARAM	wParam);


void InitGL(void);
void DrawGLScene(void);
void clearGL(void);

void min5display(void);


#endif