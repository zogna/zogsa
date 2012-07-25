
#include "windows.h"
#include "stdio.h"
#include "stdlib.h"

struct MIN5DATA_S
{
	unsigned long int date;
	unsigned int time;
	float open_price;
	float top_price;
	float low_price;
	float close_price;
	float now_price;
	unsigned long int deal_num;
	double deal_price;
};

unsigned int  min5data_total;
//最多存2年
struct MIN5DATA_S min5data[65500]={0};

struct MIN5DATA_X
{
	//就8个编号
	char id[9];
	unsigned int  min5data_total;
	struct MIN5DATA_S *min5data;
};

unsigned int  min5datax_total=0;
struct MIN5DATA_X min5datax[8000];


int dir_total=0;
char dir[8000][256];
char outdir[8000][256];

void Getfile(void);
void min5write(const char *path);
void min5read(const char *path);
void min5copy(int i,const char *path);
void min5writeX(const char *path);


//Security Analysis
int main(void)
{
	int i;

	Getfile();

	for(i=0;i<dir_total;i++)
	{
		min5data_total=0;

		min5read(dir[i]);
		min5copy(i,dir[i]);

		//min5write(outdir[i]);
	}
	min5datax_total=dir_total;

	 min5writeX("min5.dat");
}

void min5read(const char *path)
{
	FILE *fp;
	int i;

	fp=fopen(path,"rb");

	if(fp) 
	{
		while(!feof(fp))
		{
			fscanf(fp,"%d",&min5data[min5data_total].date);
			fscanf(fp,"%d",&min5data[min5data_total].time);
			fscanf(fp,"%f",&min5data[min5data_total].open_price);
			fscanf(fp,"%f",&min5data[min5data_total].top_price);
			fscanf(fp,"%f",&min5data[min5data_total].low_price);
			fscanf(fp,"%f",&min5data[min5data_total].close_price);
			fscanf(fp,"%d",&min5data[min5data_total].deal_num);
			fscanf(fp,"%lf",&min5data[min5data_total].deal_price);
			min5data_total++;
		}
		//会多一个
		min5data_total--;

		for(i=0;i<min5data_total;i++)
			min5data[i].now_price=min5data[i].deal_price/min5data[i].deal_num;

		fclose(fp);
	}
	else
	{
		fprintf(stderr,"%s con't read\n",path );
		getchar();
	}	
}

void min5write(const char *path)
{
	FILE *fp;

	fp=fopen(path,"wb");

	if(fp) 
	{
		fwrite(&min5data_total,sizeof(unsigned int),1,fp);
		fwrite(min5data,sizeof(struct MIN5DATA_S),min5data_total,fp);

		fclose(fp);
	}
	else
	{
		fprintf(stderr,"%s con't write\n",path );
		getchar();
	}	
}

void min5copy(int i,const char *path)
{
	int k=strlen(path);

	min5datax[i].id[0]=path[k-12];
	min5datax[i].id[1]=path[k-11];
	min5datax[i].id[2]=path[k-10];
	min5datax[i].id[3]=path[k-9];
	min5datax[i].id[4]=path[k-8];
	min5datax[i].id[5]=path[k-7];
	min5datax[i].id[6]=path[k-6];
	min5datax[i].id[7]=path[k-5];

	min5datax[i].min5data=(struct MIN5DATA_S *)calloc(min5data_total,sizeof(struct MIN5DATA_S));

	memcpy(&min5datax[i].min5data_total,&min5data_total,sizeof(unsigned int));
	memcpy(min5datax[i].min5data,min5data,sizeof(struct MIN5DATA_S)*min5data_total);
}

void min5writeX(const char *path)
{
	FILE *fp;
	int i;

	fp=fopen(path,"wb");

	if(fp) 
	{
		fwrite(&min5datax_total,sizeof(unsigned int),1,fp);

		for(i=0;i<min5datax_total;i++)
		{
			fwrite(&min5datax[i].min5data_total,sizeof(unsigned int),1,fp);
			fwrite(min5datax[i].min5data,sizeof(struct MIN5DATA_S),min5datax[i].min5data_total,fp);
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
	system("dir /s /b *.txt >MIN5list.list");
	//读到内存
	if((fr=fopen("MIN5list.list","rb"))==NULL) 
	{
		fputs("can't r",stderr);
		getchar();
	}

	for(i=0;fgets(dir[i],256,fr);i++)
		NULL;

	dir_total=i;

	fclose(fr);
	//删除文件
	system("del MIN5list.list");
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
		outdir[i][k-3]='m';
		outdir[i][k-2]='i';
		outdir[i][k-1]='n';

	}
}