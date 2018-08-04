#ifndef IMOS_H
#define IMOS_H
#include <stdio.h>
#include <string.h>
#include <omp.h>
#pragma warning(disable:4996)
#define FLAG_MAX 5120
typedef struct tagPersonBaseData//定义人的基本信息结构体
{
    char  m_strName[64];//0
    char  m_strPhone[16];//64
    char  m_strAddress[944];//80
                            //1024+80=1104/8=138
}tagPersonBaseData;
typedef struct tagStatistical
{
    int  m_nCount;
    char m_strName[64];
}tagStatistical;
void Increse(tagPersonBaseData* BaseData);//增加信息
void Delete(int n);//删除联系人
void ShowAll();//显示所有联系人信息
void ShowFlag();
void InitFlag();
void Delete(int n);
void  Alter();
int Find_str(char* str);
void ShowId(int id);
void Find_str_name(char* str);
void Find_str_ph(char* str);
void Find_str_adress(char* str);
int Islength();
void ConserveFlag();
int find_id(int n);
int TotelU();
void Alter(int id, tagPersonBaseData* BaseData);
void DiskMagment();
void Statistical();

#endif

