#ifndef FTES_ZOG_H
#define FTES_ZOG_H

#define ZOGFTES_WINCE_MODE 0
//FreeType Headers
#include <ft2build.h>
#include <freetype/freetype.h>
#include <freetype/ftglyph.h>
#include <freetype/ftoutln.h>
#include <freetype/fttrigon.h>

//OpenGL Headers 
#include <windows.h>		//(the GL headers need it)

#if ZOGFTES_WINCE_MODE
#include <GLES/gl.h>
#else
#include <gl\gl.h>	
#endif

//Ô¤¼ÓÔØ×Ö·û
struct zogftes_font_data 
{
	//×Ö·û±àºÅ
	unsigned long int charcode;	
	//×Ö·û´óÐ¡
	int fontsize;
	//ÒÆ¶¯¿í¶È
	int advancex;
	int advancey;
	//texCoords×ø±ê
	float texx;
	float texy;
	//box×ø±ê
	float boxw;
	float boxh;
	//ÄÚÆ«ÒÆ
	float left;
	float top;
};

void zogftes_pop_projection_matrix(void);
void zogftes_pushScreenCoordinateMatrix(void);

int zogftes_next_p2(int a);
int zogftes_init(const char * fname);
void zogftes_preload_font(int fontsizeA,int fontsizeB);
void zogftes_preload_font_lite(int fontsize);
void zogftes_clean();

void zogftes_load_single(FT_Face face, unsigned long int charcode,int fontsize);
void zogftes_draw_single(int texid);
void zogftes_spec_single(FT_Face face, unsigned long int charcode);

void zogftes_print(float x, float y,  wchar_t  *fmt,size_t fmtsize,int fontsize);
void zogftes_print_lite(float x, float y,  wchar_t  *fmt,size_t fmtsize,int fontsize);



#endif