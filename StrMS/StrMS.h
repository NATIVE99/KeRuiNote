#ifndef  _STRMS_H
#define  _STRMS_H
#include <stdio.h>
#include <String.h>
#pragma warning(disable:4996)
#define Capacity 1000

void  Increase(char[]);
void  ShowAll();
void  Record(int );
void  ShowFlag();
void  Delete();
void  ShowId(int index);
void  Statistical();
void  InitFlag();
void  ShowStatistical(int* szRecord);
void  Alter(int n, char* str);
int   Find_str(char* str);
int   IsLength(int strlength);
int   Find_id(int id);
void  DiskMagment();
void ShowFlag_1();
#endif