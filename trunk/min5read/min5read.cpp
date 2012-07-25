
#include "windows.h"
#include "stdio.h"
#include "stdlib.h"

#pragma pack(1)

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

struct MIN5DATA_X
{
	//就8个编号
	char id[9];
	unsigned int  total;
	struct MIN5DATA_S *data;
};

//取消节省
#pragma pack()

unsigned int  total;
//最多存2年
struct MIN5DATA_S data[65500]={0};

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
		total=0;

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
	unsigned int i;

	fp=fopen(path,"rb");

	if(fp) 
	{
		while(!feof(fp))
		{
			fscanf(fp,"%d",&data[total].date);
			fscanf(fp,"%d",&data[total].time);
			fscanf(fp,"%f",&data[total].open_price);
			fscanf(fp,"%f",&data[total].top_price);
			fscanf(fp,"%f",&data[total].low_price);
			fscanf(fp,"%f",&data[total].close_price);
			fscanf(fp,"%d",&data[total].deal_num);
			fscanf(fp,"%lf",&data[total].deal_price);
			total++;
		}
		//会多一个
		total--;

		for(i=0;i<total;i++)
			data[i].now_price=data[i].deal_price/data[i].deal_num;

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
		fwrite(&total,sizeof(unsigned int),1,fp);
		fwrite(data,sizeof(struct MIN5DATA_S),total,fp);

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

	min5datax[i].data=(struct MIN5DATA_S *)calloc(total,sizeof(struct MIN5DATA_S));

	memcpy(&min5datax[i].total,&total,sizeof(unsigned int));
	memcpy(min5datax[i].data,data,sizeof(struct MIN5DATA_S)*total);
}

void min5writeX(const char *path)
{
	FILE *fp;
	unsigned int i;

	fp=fopen(path,"wb");

	if(fp) 
	{
		fwrite(&min5datax_total,sizeof(unsigned int),1,fp);

		for(i=0;i<min5datax_total;i++)
		{
			fwrite(min5datax[i].id,sizeof(char),9,fp);
			fwrite(&min5datax[i].total,sizeof(unsigned int),1,fp);
			fwrite(min5datax[i].data,sizeof(struct MIN5DATA_S),min5datax[i].total,fp);
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