#include "min5a.h"

#include <windows.h>		// Header File For Windows
#include <stdio.h>			// Header File For Standard Input/Output
#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include "fileread.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////
extern unsigned int  min5datax_total;
extern struct MIN5DATA_X *min5datax;

int timelist[48]=
{
	935,
	940,
	945,
	950,
	955,
	1000,
	1005,
	1010,
	1015,
	1020,
	1025,
	1030,
	1035,
	1040,
	1045,
	1050,
	1055,
	1100,
	1105,
	1110,
	1115,
	1120,
	1125,
	1300,
	1305,
	1310,
	1315,
	1320,
	1325,
	1330,
	1335,
	1340,
	1345,
	1350,
	1355,
	1400,
	1405,
	1410,
	1415,
	1420,
	1425,
	1430,
	1435,
	1440,
	1445,
	1450,
	1455,
	1500,
};

//0935->1500 ��48�����ݵ�
int min5UpRate[48];

//һֻ��Ʊ��ÿ����ǵ���
void min5UpRateOne(unsigned int i)
{
	unsigned int j,k;

	for(j=0;j<48;j++)
		min5UpRate[j]=0;

	for(j=0;j<min5datax[i].total;j++)
	{

		for(k=0;k<48;k++)
		{
			if(min5datax[i].data[j].time==timelist[k])
				break;
		}
		if(48==k)
			k=48;
		if(0==k || 0==j)
			min5UpRate[k]+=UPorDOWN(min5datax[i].data[j].open_price,min5datax[i].data[j].now_price);
		else
		{
			if(min5datax[i].data[j-1].date==min5datax[i].data[j].date)
				min5UpRate[k]+=UPorDOWN(min5datax[i].data[j-1].open_price,min5datax[i].data[j].now_price);
			else
				min5UpRate[k]+=UPorDOWN(min5datax[i].data[j].open_price,min5datax[i].data[j].now_price);
		}
	}
}

char * Getmin5ID(unsigned int i)
{
	return min5datax[i].id;
}

///////////////////////////////////////////////////
extern unsigned int  name_total;
extern struct NAMEDATA_S *namedata;
wchar_t * Getmin5NAME(unsigned int i)
{
	unsigned int j;
	unsigned long int n=atol(&min5datax[i].id[2]);

	for(j=0;j<name_total;j++)
	{
		if(namedata[j].num==n)
			return namedata[j].name;
	}

	return L"";
}