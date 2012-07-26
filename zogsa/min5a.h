#ifndef _ZOGSA_MIN5A_H_
#define _ZOGSA_MIN5A_H_

#include "stdio.h"

#define UPorDOWN(a,b) (a)<(b) ? (1):(-1)


void min5UpRateOne(unsigned int i);
void min10UpRateOne(unsigned int i);
void min30UpRateOne(unsigned int i);
void min60UpRateOne(unsigned int i);
void min120UpRateOne(unsigned int i);

char * Getmin5ID(unsigned int i);
wchar_t * Getmin5NAME(unsigned int i);

#endif