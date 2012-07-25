
#include "windows.h"
#include "stdio.h"
#include "stdlib.h"

struct DAYDATA_S
{
	unsigned long int date;
	float open_price;
	float top_price;
	float low_price;
	float close_price;
	float aver_price;
	unsigned long int deal_num;
	double deal_price;
};

unsigned int  daydata_total;
//最多存2年
struct DAYDATA_S daydata[65500]={0};

struct DAYDATA_X
{
	//就8个编号
	char id[9];
	unsigned int  daydata_total;
	struct DAYDATA_S *daydata;
};

unsigned int  daydatax_total=0;
struct DAYDATA_X daydatax[8000];


int dir_total=0;
char dir[8000][256];
char outdir[8000][256];

void Getfile(void);
void daywrite(const char *path);
void dayread(const char *path);
void daycopy(int i,const char *path);
void daywriteX(const char *path);


//Security Analysis
int main(void)
{
	int i;

	Getfile();

	for(i=0;i<dir_total;i++)
	{
		daydata_total=0;

		dayread(dir[i]);
		daycopy(i,dir[i]);

		//daywrite(outdir[i]);
	}
	daydatax_total=dir_total;

	 daywriteX("day.dat");
}

void dayread(const char *path)
{
	FILE *fp;
	int i;

	fp=fopen(path,"rb");

	if(fp) 
	{
		while(!feof(fp))
		{
			fscanf(fp,"%d",&daydata[daydata_total].date);
			fscanf(fp,"%f",&daydata[daydata_total].open_price);
			fscanf(fp,"%f",&daydata[daydata_total].top_price);
			fscanf(fp,"%f",&daydata[daydata_total].low_price);
			fscanf(fp,"%f",&daydata[daydata_total].close_price);
			fscanf(fp,"%d",&daydata[daydata_total].deal_num);
			fscanf(fp,"%lf",&daydata[daydata_total].deal_price);
			daydata_total++;
		}
		//会多一个
		daydata_total--;

		for(i=0;i<daydata_total;i++)
			daydata[i].aver_price=daydata[i].deal_price/daydata[i].deal_num;

		fclose(fp);
	}
	else
	{
		fprintf(stderr,"%s con't read\n",path );
		getchar();
	}	
}

void daywrite(const char *path)
{
	FILE *fp;

	fp=fopen(path,"wb");

	if(fp) 
	{
		fwrite(&daydata_total,sizeof(unsigned int),1,fp);
		fwrite(daydata,sizeof(struct DAYDATA_S),daydata_total,fp);

		fclose(fp);
	}
	else
	{
		fprintf(stderr,"%s con't write\n",path );
		getchar();
	}	
}

void daycopy(int i,const char *path)
{
	int k=strlen(path);

	daydatax[i].id[0]=path[k-12];
	daydatax[i].id[1]=path[k-11];
	daydatax[i].id[2]=path[k-10];
	daydatax[i].id[3]=path[k-9];
	daydatax[i].id[4]=path[k-8];
	daydatax[i].id[5]=path[k-7];
	daydatax[i].id[6]=path[k-6];
	daydatax[i].id[7]=path[k-5];

	daydatax[i].daydata=(struct DAYDATA_S *)calloc(daydata_total,sizeof(struct DAYDATA_S));

	memcpy(&daydatax[i].daydata_total,&daydata_total,sizeof(unsigned int));
	memcpy(daydatax[i].daydata,daydata,sizeof(struct DAYDATA_S)*daydata_total);
}

void daywriteX(const char *path)
{
	FILE *fp;
	int i;

	fp=fopen(path,"wb");

	if(fp) 
	{
		fwrite(&daydatax_total,sizeof(unsigned int),1,fp);

		for(i=0;i<daydatax_total;i++)
		{
			fwrite(&daydatax[i].daydata_total,sizeof(unsigned int),1,fp);
			fwrite(daydatax[i].daydata,sizeof(struct DAYDATA_S),daydatax[i].daydata_total,fp);
		}

		fclose(fp);
	}
	else
	{
		fprintf(stderr,"%s con't write\n",path );
		getchar();
	}	
}

//遍历文件
void Getfile(void)
{
	FILE *fr;
	int i;
	size_t k;
	//得到目录
	system("dir /s /b *.txt >DAYlist.list");
	//读到内存
	if((fr=fopen("DAYlist.list","rb"))==NULL) 
	{
		fputs("can't r",stderr);
		getchar();
	}

	for(i=0;fgets(dir[i],256,fr);i++)
		NULL;

	dir_total=i;

	fclose(fr);
	//删除文件
	system("del DAYlist.list");
	//后缀转换
	for(i=0;i<dir_total;i++)
	{
		k=strlen(dir[i]);
		//回车占两格
		dir[i][k-1]='\0';
		dir[i][k-2]='\0';

		memcpy(outdir[i],dir[i],256*sizeof(char));

		k=strlen(outdir[i]);
		//回车占两格
		outdir[i][k-3]='d';
		outdir[i][k-2]='a';
		outdir[i][k-1]='y';

	}
}