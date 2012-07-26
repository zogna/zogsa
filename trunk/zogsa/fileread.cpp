#include "windows.h"
#include "stdio.h"
#include "stdlib.h"
#include "fileread.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned int  min5datax_total;
struct MIN5DATA_X *min5datax;
////////////////////////////////////////////////////
void min5readX(const char *path)
{
	FILE *fp;
	unsigned int i;

	fp=fopen(path,"rb");

	if(fp) 
	{
		fread(&min5datax_total,sizeof(unsigned int),1,fp);

		min5datax=(struct MIN5DATA_X *)calloc(min5datax_total,sizeof(struct MIN5DATA_X));

		for(i=0;i<min5datax_total;i++)
		{
			fread(min5datax[i].id,sizeof(char),9,fp);
			fread(&min5datax[i].total,sizeof(unsigned int),1,fp);
			
			min5datax[i].data=(struct MIN5DATA_S *)calloc(min5datax[i].total,sizeof(struct MIN5DATA_S));
			fread(min5datax[i].data,sizeof(struct MIN5DATA_S),min5datax[i].total,fp);
		}

		fclose(fp);
	}
	else
	{
		fprintf(stderr,"%s con't read\n",path );
		getchar();
	}	
}

////////////////////////////////////////////////////////////////////////////////////////////////////////

unsigned int  daydatax_total;
struct DAYDATA_X *daydatax;
////////////////////////////////////////////////////
void dayreadX(const char *path)
{	
	FILE *fp;
	unsigned int i;

	fp=fopen(path,"rb");

	if(fp) 
	{
		fread(&daydatax_total,sizeof(unsigned int),1,fp);

		daydatax=(struct DAYDATA_X *)calloc(daydatax_total,sizeof(struct DAYDATA_X));

		for(i=0;i<daydatax_total;i++)
		{
			fread(daydatax[i].id,sizeof(char),9,fp);
			fread(&daydatax[i].total,sizeof(unsigned int),1,fp);

			daydatax[i].data=(struct DAYDATA_S *)calloc(daydatax[i].total,sizeof(struct DAYDATA_S));
			fread(daydatax[i].data,sizeof(struct DAYDATA_S),daydatax[i].total,fp);
		}

		fclose(fp);
	}
	else
	{
		fprintf(stderr,"%s con't read\n",path );
		getchar();
	}	
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned int  name_total;
struct NAMEDATA_S *namedata;

////////////////////////////////////////////////////
void nameread(const char *path)
{
	FILE *fp;

	fp=fopen(path,"rb");

	if(fp) 
	{
		fread(&name_total,sizeof(unsigned int),1,fp);

		namedata=(struct NAMEDATA_S *)calloc(name_total,sizeof(struct NAMEDATA_S));
		fread(namedata,sizeof(struct NAMEDATA_S),name_total,fp);

		fclose(fp);
	}
	else
	{
		fprintf(stderr,"%s con't read\n",path );
		getchar();
	}	
}

//Security Analysis
void readdata(void)
{
	 min5readX("min5.dat");
	 dayreadX("day.dat");
	 nameread("name.dat");
}
