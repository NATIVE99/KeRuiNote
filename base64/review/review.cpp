// review.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
using namespace std;
#pragma warning(disable:4996)  
char base64[65] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxzy0123456789+/";
void base64_encode(const char* strI, int nLength, char* strO);
void dec(const char* strI, int nLength, char* strO);
void Find_Index(const char* str, char* Index);
int _tmain(int argc, _TCHAR* argv[])
{
    ifstream fp;
    ofstream ofp;
    ofstream decyOfp;
    char *p = NULL;
    char szBuf[256] = { 0 };
    char szOutBuf[256] = { 0 };
    fp.open("enc.txt");
    //fp.open("测试账号.txt");
    decyOfp.open("decy.txt");
    if (fp.is_open()&&decyOfp.is_open())
    {
        fp.seekg(3,fp.beg);
      
            while (!fp.eof())
            {
                memset(szBuf, 0, 256);
                
                fp.getline(szBuf, 256);
                //账号
                p = (char*)malloc(sizeof(char)*strlen(szBuf));
                p=strtok(szBuf, "----");
                if (NULL == p)
                {
                    break;
                }
                decyOfp << p;
                //密码
                p = strtok(NULL, "----");
              
                dec(p, strlen(p), szOutBuf);
                decyOfp << "----" << szOutBuf << endl;
            }
       

    }
   
    fp.close();
    ofp.close();
	return 0;
}
void base64_encode(const  char* strI, int nLength, char* strO)
{
    /*  8   8   8
        6  2+4 4+2 6
    */
    unsigned char arry3B[3] = { 0 };
    unsigned char arry4B[4] = { 0 };
    int nCount = 0;
    int n = 0;
    memset(strO, 0, 256);
    nLength % 3 ? nCount = nLength/3 : nCount = nLength/3 + 1;
    
    for (int i = 0; i < nCount; i++)
    {
        arry3B[0] = strI[3 * i];
        arry3B[1] = strI[3 * i+1];
        arry3B[2] = strI[3 * i + 2];
        memset(arry4B, 0, 4);
        //arry4B[0] = base64[arry3B[0] >> 2 & 0x3F];
        strO[n++] = base64[arry3B[0] >> 2 & 0x3F];
        //arry4B[1] = base64[(arry3B[0] << 4 & 0x30) | (arry3B[1] >> 4 & 0x0F)];
        strO[n++] = base64[(arry3B[0] << 4 & 0x30) | (arry3B[1] >> 4 & 0x0F)];
        //arry4B[2] = base64[(arry3B[1] << 2 & 0x3c) | (arry3B[2] >> 6 & 0x03)];
        strO[n++] = base64[(arry3B[1] << 2 & 0x3c) | (arry3B[2] >> 6 & 0x03)];
        //char c = arry3B[3] & 0x3F;
        strO[n++] = base64[arry3B[2] & 0x3F];
    }
    //补齐空格的等号个数
    if (nLength % 3 == 1)
    {
        strO[n] = strO[n+1] = '=';
    } 
    else if (nLength % 3 == 2)
    {
        strO[n] = '=';
    }

}
void dec( const char* strI, int nLength, char* strO)
{
    /*  6   6   6  6
        6+2 4+4 2+6
    */
    unsigned char arry3B[3] = { 0 };
    unsigned char arry4B[4] = { 0 };
    int nCount = 0;
    int n = 0;
    char nIndex[4] = { 0 };
    nCount = nLength / 4;
    for (int j = 0; j < nCount; j++)
    {
        arry4B[0] = strI[j * 4];
        arry4B[1] = strI[j * 4+1];
        arry4B[2] = strI[j * 4+2];
        arry4B[3] = strI[j * 4+3];
        Find_Index(&strI[j * 4], nIndex);
        //arry3B[0] =(nIndex[0] << 2 & 0xFC) | (nIndex[1] >> 4 & 0x03);
        strO[n++] = (nIndex[0] << 2 & 0xFC) | (nIndex[1] >> 4 & 0x03);
        //arry3B[1] = (nIndex[1] << 4 & 0xF0) | (nIndex[2] >> 2 & 0x0F);
        strO[n++] = (nIndex[1] << 4 & 0xF0) | (nIndex[2] >> 2 & 0x0F);
        //arry3B[2] = (nIndex[2] << 6 & 0xC0) | nIndex[3]; 
        strO[n++] = (nIndex[2] << 6 & 0xC0) | nIndex[3];

    
    }



}



void Find_Index(const char* str,char* Index)
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 64; j++)
        {
            if (str[i] == base64[j])
            {
                Index[i] = j;
                break;
            }
        
        }
    
    }

}