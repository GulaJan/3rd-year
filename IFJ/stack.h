
#ifndef STACK_H
#define STACK_H
#include <stdio.h>
#include <stdlib.h>


extern int error_type;

typedef struct
{
	int *data;
	int *data_type;
	int top;
	int *is_variable;
	void **value[];
}tStack;

void Serror  (int error_type);
void SInit   (tStack *s);
int SEmpty   (tStack *s);
int STop   	 (tStack *s);
void SPush   (tStack *s,int data, int data_type);
int STopPop	 (tStack *s);
int STopSec  (tStack *s);
void SFree	 (tStack *s);
void SPrint	 (tStack *s);

#endif