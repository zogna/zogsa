#include "zogftes.h"

//前256个为两种字号
#define MAX_FTES_TEXTURE_NUM 512

FT_Face face;
FT_Library library;

//纹理对象
unsigned int zogftes_textures[MAX_FTES_TEXTURE_NUM];
//预加载的数据数组
struct zogftes_font_data zogftes_prefont[MAX_FTES_TEXTURE_NUM];
//指向最后一个纹理对象
int texturep = 0;

//非预加载的
unsigned int zogftes_spec_textures;
int movew, moveh;

//补齐成2的N次方
int zogftes_next_p2(int a)
{
    int rval = 1;
    while(rval < a) rval *= 2;
    return rval;
}

//显示单个字符
void zogftes_load_single(FT_Face face, unsigned long int charcode, int fontsize)
{
    FT_Load_Char(face, charcode, FT_LOAD_RENDER);

    //This reference will make accessing the bitmap easier
    FT_Bitmap &bitmap = face->glyph->bitmap;

    //Use our helper function to get the widths of
    //the bitmap data that we will need in order to create
    //our texture.
    int width = zogftes_next_p2( bitmap.width );
    int height = zogftes_next_p2( bitmap.rows );

    zogftes_prefont[texturep].charcode = charcode;
    zogftes_prefont[texturep].advancex = face->glyph->advance.x;
    zogftes_prefont[texturep].advancey = face->glyph->advance.y;
    zogftes_prefont[texturep].left = (float)face->glyph->bitmap_left;
    zogftes_prefont[texturep].top = (float)face->glyph->bitmap_top;
    zogftes_prefont[texturep].boxh = (float)bitmap.rows;
    zogftes_prefont[texturep].boxw = (float)bitmap.width;
    zogftes_prefont[texturep].texx = (float)bitmap.width / (float)width,
                              zogftes_prefont[texturep].texy = (float)bitmap.rows / (float)height;
    zogftes_prefont[texturep].fontsize = fontsize;

    /* 2011 .12.27改进
    //Allocate memory for the texture data.
    unsigned char* expanded_data = new unsigned char[ 2*width*height];

    unsigned long int tempp;
    int i,j;
    for(j=0; j<height;j++)
    {
    	for(i=0; i<width; i++)
    	{
    		tempp=2*(i+j*width);

    		expanded_data[tempp]=
    		expanded_data[tempp+1] =
    			(i>=bitmap.width || j>=bitmap.rows) ?
    			0 : bitmap.buffer[i + bitmap.width*j];
    	}
    }

    //Now we just setup some texture paramaters.
     glPixelStorei(GL_UNPACK_ALIGNMENT,1);
    glBindTexture( GL_TEXTURE_2D,zogftes_textures[texturep]);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexEnvx(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    //glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    //glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    // glTexEnvx(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

    //Here we actually create the texture itself, notice
    //that we are using GL_LUMINANCE_ALPHA to indicate that
    //we are using 2 channel data.
    glTexImage2D( GL_TEXTURE_2D, 0, GL_LUMINANCE_ALPHA, width, height,
    	  0, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, expanded_data);
    */

    //Allocate memory for the texture data.
    unsigned char *expanded_data = new unsigned char[ width*height];

    unsigned long int tempp;
    int i, j;
    for(j = 0; j < height; j++)
    {
        for(i = 0; i < width; i++)
        {
            tempp = i + j * width;

            expanded_data[tempp] =
                (i >= bitmap.width || j >= bitmap.rows) ?
                0 : bitmap.buffer[i + bitmap.width*j];
        }
    }

    //Now we just setup some texture paramaters.
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glBindTexture( GL_TEXTURE_2D, zogftes_textures[texturep]);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
#if ZOGFTES_WINCE_MODE
    glTexEnvx(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
#else
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
#endif

    //glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    //glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    // glTexEnvx(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

    //Here we actually create the texture itself, notice
    //that we are using GL_LUMINANCE_ALPHA to indicate that
    //we are using 2 channel data.
    glTexImage2D( GL_TEXTURE_2D, 0, GL_ALPHA, width, height,
                  0, GL_ALPHA, GL_UNSIGNED_BYTE, expanded_data);

    //With the texture created, we don't need to expanded data anymore
    delete[] expanded_data;
    expanded_data = NULL;
}

void zogftes_draw_single(int texid)
{
    float box[] =
    {
        0, 0,
        zogftes_prefont[texid].boxw, 0,
        0, zogftes_prefont[texid].boxh,
        zogftes_prefont[texid].boxw, zogftes_prefont[texid].boxh,
    };

    float texCoords[] =
    {
        0.0f, 0.0f,
        zogftes_prefont[texid].texx, 0.0f,
        0.0f, zogftes_prefont[texid].texy,
        zogftes_prefont[texid].texx, zogftes_prefont[texid].texy,
    };
    /*
    	float cc[]=
    	{
    		1.0,0.0,0.0,1.0,
    		1.0,0.0,0.0,1.0,
    		1.0,0.0,0.0,1.0,
    		1.0,0.0,0.0,1.0,
    	};
    	*/
    glBindTexture(GL_TEXTURE_2D, zogftes_textures[texid]);
    glPushMatrix();

    //留间隔
    glTranslatef( zogftes_prefont[texid].left, -zogftes_prefont[texid].top, 0);

    //glColorPointer(4, GL_FLOAT, 0, cc);
    glVertexPointer(2, GL_FLOAT, 0, box);
    glTexCoordPointer(2, GL_FLOAT, 0, texCoords);

    //glEnableClientState(GL_VERTEX_ARRAY);
    //glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    //glDisableClientState(GL_VERTEX_ARRAY);
    //glDisableClientState(GL_TEXTURE_COORD_ARRAY);

    glPopMatrix();
}


//不预加载显示单个字符
void zogftes_spec_single(FT_Face face, unsigned long int charcode)
{
    FT_Load_Char(face, charcode, FT_LOAD_RENDER);

    //This reference will make accessing the bitmap easier
    FT_Bitmap &bitmap = face->glyph->bitmap;

    //Use our helper function to get the widths of
    //the bitmap data that we will need in order to create
    //our texture.
    int width = zogftes_next_p2( bitmap.width );
    int height = zogftes_next_p2( bitmap.rows );

    movew = face->glyph->advance.x / 64;
    moveh = face->glyph->advance.y / 64;

    /*2011.12.27改进

    	//Allocate memory for the texture data.
    	unsigned char* expanded_data = new unsigned char[ 2*width*height];
    	//Here we fill in the data for the expanded bitmap.
    	//Notice that we are using two channel bitmap (one for
    	//luminocity and one for alpha), but we assign
    	//both luminocity and alpha to the value that we
    	//find in the FreeType bitmap.
    	//We use the ?: operator so that value which we use
    	//will be 0 if we are in the padding zone, and whatever
    	//is the the Freetype bitmap otherwise.
    	unsigned long int tempp;
    	int i,j;
    	for(j=0; j<height;j++)
    	{
    		for(i=0; i<width; i++)
    		{
    			tempp=2*(i+j*width);

    			expanded_data[tempp]=
    			expanded_data[tempp+1] =
    				(i>=bitmap.width || j>=bitmap.rows) ?
    				0 : bitmap.buffer[i + bitmap.width*j];
    		}
    	}

    	//Now we just setup some texture paramaters.
    	glPixelStorei(GL_UNPACK_ALIGNMENT,1);
        glBindTexture( GL_TEXTURE_2D,zogftes_spec_textures);
    	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    	glTexEnvx(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    	//Here we actually create the texture itself, notice
    	//that we are using GL_LUMINANCE_ALPHA to indicate that
    	//we are using 2 channel data.
        	glTexImage2D( GL_TEXTURE_2D, 0, GL_LUMINANCE_ALPHA, width, height,
    		  0, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, expanded_data);
    */

    //Allocate memory for the texture data.
    unsigned char *expanded_data = new unsigned char[ width*height];
    //Here we fill in the data for the expanded bitmap.
    //Notice that we are using two channel bitmap (one for
    //luminocity and one for alpha), but we assign
    //both luminocity and alpha to the value that we
    //find in the FreeType bitmap.
    //We use the ?: operator so that value which we use
    //will be 0 if we are in the padding zone, and whatever
    //is the the Freetype bitmap otherwise.
    unsigned long int tempp;
    int i, j;
    for(j = 0; j < height; j++)
    {
        for(i = 0; i < width; i++)
        {
            tempp = i + j * width;

            expanded_data[tempp] =
                (i >= bitmap.width || j >= bitmap.rows) ?
                0 : bitmap.buffer[i + bitmap.width*j];
        }
    }

    //Now we just setup some texture paramaters.
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glBindTexture( GL_TEXTURE_2D, zogftes_spec_textures);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
#if ZOGFTES_WINCE_MODE
    glTexEnvx(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
#else
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
#endif
    //Here we actually create the texture itself, notice
    //that we are using GL_LUMINANCE_ALPHA to indicate that
    //we are using 2 channel data.
    glTexImage2D( GL_TEXTURE_2D, 0, GL_ALPHA, width, height,
                  0, GL_ALPHA, GL_UNSIGNED_BYTE, expanded_data);
    //With the texture created, we don't need to expanded data anymore

    delete [] expanded_data;
    expanded_data = NULL;
    //Now we need to account for the fact that many of
    //our textures are filled with empty padding space.
    //We figure what portion of the texture is used by
    //the actual character and store that information in
    //the x and y variables, then when we draw the
    //quad, we will only reference the parts of the texture
    //that we contain the character itself.
    float	x = (float)bitmap.width / (float)width,
            y = (float)bitmap.rows / (float)height;

    float box[] =
    {
        0, 0,
        (float)bitmap.width, 0,
        0, (float)bitmap.rows,
        (float)bitmap.width, (float)bitmap.rows
    };

    float texCoords[] =
    {
        0.0f, 0.0f,
        x, 0.0f,
        0.0f, y,
        x, y,
    };

    glBindTexture(GL_TEXTURE_2D, zogftes_spec_textures);
    glPushMatrix();

    //留间隔
    glTranslatef( (float)face->glyph->bitmap_left,(float) -(face->glyph->bitmap_top), 0);

    glVertexPointer(2, GL_FLOAT, 0, box);
    glTexCoordPointer(2, GL_FLOAT, 0, texCoords);

    //glEnableClientState(GL_VERTEX_ARRAY);
    //glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    //glDisableClientState(GL_VERTEX_ARRAY);
    //glDisableClientState(GL_TEXTURE_COORD_ARRAY);

    glPopMatrix();

}

int  zogftes_init(const char *fname)
{
    //Create and initilize a freetype font library.
    if (FT_Init_FreeType( &library ))
    {
        //MessageBox(hWnd, _T("FT_Init_FreeType failed"), _T("Error"), MB_OK|MB_ICONEXCLAMATION);
        printf("FT_Init_FreeType failed");
        return -1;
    }
    //The object in which Freetype holds information on a given
    //font is called a "face".

    //This is where we load in the font information from the file.
    //Of all the places where the code might die, this is the most likely,
    //as FT_New_Face will die if the font file does not exist or is somehow broken.
    if (FT_New_Face( library, fname, 0, &face ))
    {
        //MessageBox(hWnd, _T("FT_New_Face failed (there is probably a problem with your font file)"), _T("Error"), MB_OK|MB_ICONEXCLAMATION);
        printf("FT_New_Face failed (there is probably a problem with your font file)");
        return -1;
    }

    glGenTextures( 1, &zogftes_spec_textures );
    glGenTextures( MAX_FTES_TEXTURE_NUM, zogftes_textures );

    return 0;
}

void zogftes_preload_font(int fontsizeA, int fontsizeB)
{
#if (MAX_FTES_TEXTURE_NUM >= 256)
    {
        //增加预加载纹理0-128字符
        FT_Set_Char_Size( face, fontsizeA * 64, fontsizeA * 64, 96, 96);
        for(int i = 0; i < 128; i++)
        {
            zogftes_load_single(face, i, fontsizeA);
            //要++
            texturep++;
        }
        //增加预加载纹理0-128字符
        FT_Set_Char_Size( face, fontsizeB * 64, fontsizeB * 64, 96, 96);
        for(int i = 0; i < 128; i++)
        {
            zogftes_load_single(face, i, fontsizeB);
            //要++
            texturep++;
        }
    }
#endif
}

void zogftes_preload_font_lite(int fontsize)
{
#if (MAX_FTES_TEXTURE_NUM >= 128)
    {
        //增加预加载纹理0-128字符
        FT_Set_Char_Size( face, fontsize * 64, fontsize * 64, 96, 96);
        for(int i = 0; i < 128; i++)
        {
            zogftes_load_single(face, i, fontsize);
            //要++
            texturep++;
        }
    }
#endif
}

void zogftes_clean()
{
    //We don't need the face information now that the display
    //lists have been created, so we free the assosiated resources.
    FT_Done_Face(face);
    //Ditto for the library.
    FT_Done_FreeType(library);

    glDeleteTextures(1, &zogftes_spec_textures);
    glDeleteTextures(MAX_FTES_TEXTURE_NUM, zogftes_textures);

}

/// A fairly straight forward function that pushes
/// a projection matrix that will make object world
/// coordinates identical to window coordinates.
void zogftes_pushScreenCoordinateMatrix(void)
{
    int	viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
#if ZOGFTES_WINCE_MODE
    glOrthof(viewport[0], viewport[2], viewport[3], viewport[1], -10, 10);
#else
	glOrtho(viewport[0], viewport[2], viewport[3], viewport[1], -1, 1);
#endif
}

/// Pops the projection matrix without changing the current
/// MatrixMode.
void zogftes_pop_projection_matrix(void)
{
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
}

///Much like Nehe's glPrint function, but modified to work
///with freetype fonts.
void zogftes_print(float x, float y,  wchar_t  *fmt, size_t fmtsize, int fontsize)
{
    int tempmove = 0;
    int flag = 0;
    FT_Set_Char_Size( face, fontsize * 64, fontsize * 64, 96, 96);

    //zogftes_pushScreenCoordinateMatrix();

    //glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glDisable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);

    /* 2011.12.27改进
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //黑
    //glBlendFunc(GL_ZERO, GL_ONE_MINUS_SRC_ALPHA);
    //白
    //glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    //背景反色
    glBlendFunc(GL_ONE_MINUS_DST_COLOR, GL_ONE_MINUS_SRC_ALPHA);
    //背景色
    //glBlendFunc(GL_DST_COLOR, GL_ONE_MINUS_SRC_ALPHA);
    */

    //2011.12.27这个颜色就是显示字的颜色。再也不受限制
    //红色字 
    //glColor4f(1,0,0,1);
	//这里不设颜色。在调用前设置颜色吧，或者把下面这个颜色开起来
  //  glColor4f(0.1f, 0.1f, 0.1f, 1.0f);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glPushMatrix();

    glTranslatef(x, y, 0);

    //	glEnableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    for(size_t i = 0; i < fmtsize; i++)
    {
        glTranslatef((float)tempmove, 0.0, 0.0);

        //查找字
        flag = 0;
        for(int j = 0; j < texturep; j++)
        {
            if(zogftes_prefont[j].charcode == fmt[i] && zogftes_prefont[j].fontsize == fontsize)
            {
                //找到字
                zogftes_draw_single(j);
                tempmove = zogftes_prefont[j].advancex / 64;
                flag = 1;
                break;
            }
        }
        //找到字
        if(1 == flag) continue;
        //找不到字
        if(texturep < MAX_FTES_TEXTURE_NUM)
        {
            //增加预加载纹理
            zogftes_load_single(face, fmt[i], fontsize);
            zogftes_draw_single(texturep);
            tempmove = zogftes_prefont[texturep].advancex / 64;
            //要++
            texturep++;
            //	MessageBox(hWnd, _T("11"), _T("Error"), MB_OK);
        }
        else
        {
            //预加载纹理已满，使用特殊纹理
            zogftes_spec_single(face, fmt[i]);
            tempmove = movew;
            //	MessageBox(hWnd, _T("22)"), _T("Error"), MB_OK);
        }
    }
    //	glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);

    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_BLEND);

    //zogftes_pop_projection_matrix();
}


///Much like Nehe's glPrint function, but modified to work
///with freetype fonts.
void zogftes_print_lite(float x, float y,  wchar_t  *fmt, size_t fmtsize, int fontsize)
{
    int tempmove = 0;
    int flag = 0;
    FT_Set_Char_Size( face, fontsize * 64, fontsize * 64, 96, 96);

    glPushMatrix();

    glTranslatef(x, y, 0);
    for(size_t i = 0; i < fmtsize; i++)
    {
        glTranslatef((float)tempmove, 0.0, 0.0);

        //查找字
        flag = 0;
        for(int j = 0; j < texturep; j++)
        {
            if(zogftes_prefont[j].charcode == fmt[i] && zogftes_prefont[j].fontsize == fontsize)
            {
                //找到字
                zogftes_draw_single(j);
                tempmove = zogftes_prefont[j].advancex / 64;
                flag = 1;
                break;
            }
        }
        //找到字
        if(1 == flag) continue;
        //找不到字
        if(texturep < MAX_FTES_TEXTURE_NUM)
        {
            //增加预加载纹理
            zogftes_load_single(face, fmt[i], fontsize);
            zogftes_draw_single(texturep);
            tempmove = zogftes_prefont[texturep].advancex / 64;
            //要++
            texturep++;
            //	MessageBox(hWnd, _T("11"), _T("Error"), MB_OK);

        }
        else
        {
            //预加载纹理已满，使用特殊纹理
            zogftes_spec_single(face, fmt[i]);
            tempmove = movew;
            //	MessageBox(hWnd, _T("22)"), _T("Error"), MB_OK);
        }
    }

    glPopMatrix();
}

