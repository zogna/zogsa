#ifndef _ZOGSA_FILEREAD_H_
#define _ZOGSA_FILEREAD_H_

#include "stdio.h"

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
	//��8�����
	char id[9];
	unsigned int  total;
	struct MIN5DATA_S *data;
};


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

struct DAYDATA_X
{
	//��8�����
	char id[9];
	unsigned int  total;
	struct DAYDATA_S *data;
};


//ȡ����ʡ
#pragma pack()

#endif