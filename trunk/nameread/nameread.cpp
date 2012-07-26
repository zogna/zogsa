#include "windows.h"
#include "stdio.h"
#include "stdlib.h"
#include <locale.h> 

#pragma pack(1)

struct NAMEDATA_S
{
	unsigned long int num;
	wchar_t name[10];
};

//取消节省
#pragma pack()

unsigned int  total;
struct NAMEDATA_S data[8500]={0};

void nameread(const char *path);
void namewrite(const char *path);

//Security Analysis
int main(void)
{
	total=0;
	nameread("name.txt");
	namewrite("name.dat");
}

void nameread(const char *path)
{
	FILE *fp;
	int i=1;

	//必须设置 否则乱码
	setlocale(LC_ALL,"chs");  

	fp=fopen(path,"r");

	if(fp) 
	{
		while(!feof(fp))
		{
			fscanf(fp,"%d",&data[total].num);
			//fgetws(data[total].name,10,fp);
			fwscanf(fp,L"%s",data[total].name);

			total++;
		}
		//会多一个
		total--;

		fclose(fp);
	}
	else
	{
		fprintf(stderr,"%s con't read\n",path );
		getchar();
	}	
}

void namewrite(const char *path)
{
	FILE *fp;

	fp=fopen(path,"wb");

	if(fp) 
	{
		fwrite(&total,sizeof(unsigned int),1,fp);
		fwrite(data,sizeof(struct NAMEDATA_S),total,fp);

		fclose(fp);
	}
	else
	{
		fprintf(stderr,"%s con't write\n",path );
		getchar();
	}	
}
