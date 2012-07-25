#include "zogftes.h"
#include "zogsa_gl.h"
#include "fileread.h"
#include "min5a.h"

#define ClearColor	0.6627f,0.89019f,0.643137f,1.0f


extern unsigned short width_win ,height_win;

#define FONT_SIZE_BIG 20
#define FONT_SIZE_SMALL 10

void loadfont(void)
{

	if(zogftes_init(".\\MSYH.TTF")<0)
	{
		fprintf(stderr,"no load font");	
		exit(1);
	}
	zogftes_preload_font(FONT_SIZE_BIG,FONT_SIZE_SMALL);
}

void printftest(void)
{
	glColor4f(1,0,0,1);
	zogftes_print(500,500,L"0123456789",10,16);

}

void clear(void)
{
	zogftes_clean();
}

unsigned int IDp;

void GLkey(WPARAM	wParam)
{
	switch (wParam)							// Check System Calls
	{

	case VK_SPACE:
		break;
	case VK_RETURN:

		break;
	case VK_UP:
		break;

	case VK_DOWN:
		break;
	case VK_LEFT:
		break;

	case VK_RIGHT:
		break;
	case 'A':
	case 'a':
		break;
	case 'S':
	case 's':
		break;
		//pagedown
	case	VK_PRIOR:
		if(IDp)
			IDp--;
		break;

		//pagedown
	case	VK_NEXT:
		if(IDp<5841)
			IDp++;
		break;

	default:
		break;
	}

}

void ReSizeGLScene(GLsizei width, GLsizei height)		// Resize And Initialize The GL Window
{
	if (height==0)										// Prevent A Divide By Zero By
	{
		height=1;										// Making Height Equal One
	}

	glViewport(0,0,width,height);						// Reset The Current Viewport

	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix

	// Calculate The Aspect Ratio Of The Window
	glOrtho(0,width, height,0,-1,1 );

	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();									// Reset The Modelview Matrix
}

void InitGL(void)										// All Setup For OpenGL Goes Here
{
	//	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glShadeModel(GL_FLAT);
	glClearColor(ClearColor);				// Black Background

	glDisable(GL_DEPTH_TEST);

	//glGenTextures(10, textureID);
	// glPixelStorei(GL_UNPACK_ALIGNMENT, 1);	


	readdata();
	loadfont();
	IDp=0;
}

void DrawGLScene(void)	
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear Screen And Depth Buffer
	glLoadIdentity();		


	min5display();
	printftest();
}

void clearGL(void)
{
	clear();
}

void min5display(void)
{

	wchar_t str[9];

	mbstowcs(str,Getmin5ID(IDp),9);

	zogftes_print(400,400,str,8,16);
	min5UpRateOne(IDp);



}
