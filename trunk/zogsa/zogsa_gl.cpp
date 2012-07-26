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
		else
			IDp=2414;

		break;

		//pagedown
	case	VK_NEXT:
		if(IDp<2414)
			IDp++;
		else
			IDp=0;
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
}

void clearGL(void)
{
	clear();
}

void min5display(void)
{
	wchar_t str[9];

	mbstowcs(str,Getmin5ID(IDp),9);

	glColor3ub(0,0,0);
	zogftes_print(10,20,str,8,10);
	zogftes_print(100,20,Getmin5NAME(IDp),wcslen(Getmin5NAME(IDp)),10);

	min5UpRateOneDisplay(0,100);
	min10UpRateOneDisplay(0,200);
	
	min30UpRateOneDisplay(0,300);
	min60UpRateOneDisplay(0,400);
min120UpRateOneDisplay(0,500);
}


extern int min5UpRate[48];
void min5UpRateOneDisplay(int x,int y)
{
	int i;

	min5UpRateOne(IDp);

	glColor3ub(255,0,0);
	zogftes_print(x,y-50,L"5�����ǵ���",6,10);

	glLineWidth(2);

	glBegin(GL_LINES);
	glVertex2d(x+240,y);
	glVertex2d(x,y);
	glEnd();

	glLineWidth(4);

	glColor3ub(0,0,255);
	glBegin(GL_LINES);
	for(i=0;i<48;i++)
	{
		glVertex2d(x+3+i*5,y-min5UpRate[i]);
		glVertex2d(x+3+i*5,y);
	}
	glEnd();

}

extern int min10UpRate[24];
void min10UpRateOneDisplay(int x,int y)
{
	int i;

	min10UpRateOne(IDp);

	glColor3ub(255,0,0);
	zogftes_print(x,y-50,L"10�����ǵ���",7,10);

	glLineWidth(2);

	glBegin(GL_LINES);
	glVertex2d(x+240,y);
	glVertex2d(x,y);
	glEnd();

	glLineWidth(9);

	glColor3ub(0,0,255);
	glBegin(GL_LINES);
	for(i=0;i<24;i++)
	{
		glVertex2d(x+5+i*10,y-min10UpRate[i]);
		glVertex2d(x+5+i*10,y);
	}
	glEnd();

}

extern int min30UpRate[8];
void min30UpRateOneDisplay(int x,int y)
{
	int i;

	min30UpRateOne(IDp);

	glColor3ub(255,0,0);
	zogftes_print(x,y-50,L"30�����ǵ���",7,10);

	glLineWidth(2);

	glBegin(GL_LINES);
	glVertex2d(x+240,y);
	glVertex2d(x,y);
	glEnd();

	glColor3ub(0,0,255);
	
	glBegin(GL_QUADS);
	for(i=0;i<8;i++)
	{
		glVertex2d(x+i*30,y-min30UpRate[i]);
		glVertex2d(x+i*30,y);
		glVertex2d(x+(i+1)*30-1,y);
		glVertex2d(x+(i+1)*30-1,y-min30UpRate[i]);
	}
	glEnd();

}

extern int min60UpRate[4];
void min60UpRateOneDisplay(int x,int y)
{
	int i;

	min60UpRateOne(IDp);

	glColor3ub(255,0,0);
	zogftes_print(x,y-50,L"60�����ǵ���",7,10);

	glLineWidth(2);

	glBegin(GL_LINES);
	glVertex2d(x+240,y);
	glVertex2d(x,y);
	glEnd();

	glColor3ub(0,0,255);
	glBegin(GL_QUADS);
	for(i=0;i<4;i++)
	{
		glVertex2d(x+i*60,y-min60UpRate[i]);
		glVertex2d(x+i*60,y);
		glVertex2d(x+(i+1)*60-1,y);
		glVertex2d(x+(i+1)*60-1,y-min60UpRate[i]);
	}
	glEnd();

}

extern int min120UpRate[2];
void min120UpRateOneDisplay(int x,int y)
{
	int i;

	min120UpRateOne(IDp);

	glColor3ub(255,0,0);
	zogftes_print(x,y-50,L"2Сʱ�ǵ���",6,10);

	glLineWidth(2);

	glBegin(GL_LINES);
	glVertex2d(x+240,y);
	glVertex2d(x,y);
	glEnd();

	glColor3ub(0,0,255);
	
	glBegin(GL_QUADS);
	for(i=0;i<2;i++)
	{
		glVertex2d(x+i*120,y-min120UpRate[i]);
		glVertex2d(x+i*120,y);
		glVertex2d(x+(i+1)*120-1,y);
		glVertex2d(x+(i+1)*120-1,y-min120UpRate[i]);
	}
	glEnd();

}