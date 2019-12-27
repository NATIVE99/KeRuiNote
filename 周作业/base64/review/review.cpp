// review.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
using namespace std;
#pragma warning(disable:4996)  

void base64_encode(const char* strI, int nLength, char* strO);
void dec(const char* strI, int nLength, char* strO);
void Find_Index(const char* str, char* Index);
void Find_Index2(const char* str, char* Index);


char base64[65] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxzy0123456789+/";
//内容是序号
//比如dekey['A']=0
//这张表可以通过字符找序号
const int dekey[] = {
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	62, // '+'
	-1, -1, -1,
	63, // '/'
	52, 53, 54, 55, 56, 57, 58, 59, 60, 61, // '0'-'9'
	-1, -1, -1, -1, -1, -1, -1,
	0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,
	13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, // 'A'-'Z'
	-1, -1, -1, -1, -1, -1,
	26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38,
	39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, // 'a'-'z'
};

int _tmain(int argc, _TCHAR* argv[])
{
    //ifstream fp;
    //ofstream ofp;
    //ofstream decyOfp;
    //char *p = NULL;
    //char szBuf[256] = { 0 };
    //char szOutBuf[256] = { 0 };
    //fp.open("enc.txt");
    ////fp.open("测试账号.txt");
    //decyOfp.open("decy.txt");
    //if (fp.is_open()&&decyOfp.is_open())
    //{
    //    fp.seekg(3,fp.beg);
    //  
    //        while (!fp.eof())
    //        {
    //            memset(szBuf, 0, 256);
    //            
    //            fp.getline(szBuf, 256);
    //            //账号
    //            p = (char*)malloc(sizeof(char)*strlen(szBuf));
    //            p=strtok(szBuf, "----");
    //            if (NULL == p)
    //            {
    //                break;
    //            }
    //            decyOfp << p;
    //            //密码
    //            p = strtok(NULL, "----");
    //          
    //            dec(p, strlen(p), szOutBuf);
    //            decyOfp << "----" << szOutBuf << endl;
    //        }
    //   

    //}
   
    //fp.close();
    //ofp.close();
	char szbuf[130] = { 0 };
	char szbuf2[120] = { 0 };
	char* str = "ABCDE";

	base64_encode(str, strlen(str), szbuf);
	printf("%s\r\n", szbuf);

	dec(szbuf, strlen(szbuf),szbuf2);
	printf("%s\r\n", szbuf2);

	Find_Index2(str, szbuf2);
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
    memset(strO, 0, nLength+1);

	nCount = nLength / 3;
    for (int i = 0; i < nCount; i++)
    {
        arry3B[0] = strI[3 * i];
        arry3B[1] = strI[3 * i+1];
        arry3B[2] = strI[3 * i + 2];
       // memset(arry4B, 0, 4);
        //arry4B[0] = base64[arry3B[0] >> 2 & 0x3F];
        strO[n++] = base64[arry3B[0] >> 2 & 0x3F];
        //arry4B[1] = base64[(arry3B[0] << 4 & 0x30) | (arry3B[1] >> 4 & 0x0F)];
        strO[n++] = base64[(arry3B[0] << 4 & 0x30) | (arry3B[1] >> 4 & 0x0F)];
        //arry4B[2] = base64[(arry3B[1] << 2 & 0x3c) | (arry3B[2] >> 6 & 0x03)];
        strO[n++] = base64[(arry3B[1] << 2 & 0x3c) | (arry3B[2] >> 6 & 0x03)];
        //char c = arry3B[3] & 0x3F;
        strO[n++] = base64[arry3B[2] & 0x3F];
    }


   // 补齐空格的等号个数
    if (nLength % 3 == 1)
    {
		arry3B[0] = strI[nCount*3];
		strO[n++] = base64[arry3B[0] >> 2 & 0x3F];
		strO[n++] = base64[(arry3B[0] << 4 & 0x30) | (0)];
        strO[n] = strO[n+1] = '=';
    } 
    else if (nLength % 3 == 2)
    {
		arry3B[0] = strI[nCount * 3];
		arry3B[1] = strI[nCount * 3 + 1];
		//arry4B[0] = base64[arry3B[0] >> 2 & 0x3F];
		strO[n++] = base64[arry3B[0] >> 2 & 0x3F];
		//arry4B[1] = base64[(arry3B[0] << 4 & 0x30) | (arry3B[1] >> 4 & 0x0F)];
		strO[n++] = base64[(arry3B[0] << 4 & 0x30) | (arry3B[1] >> 4 & 0x0F)];
		//arry4B[2] = base64[(arry3B[1] << 2 & 0x3c) | (arry3B[2] >> 6 & 0x03)];
		strO[n++] = base64[(arry3B[1] << 2 & 0x3c) | (0)];

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
    nCount = nLength / 4-1;
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


	//最后4个字节的处理，判断是否有‘=’
	// 8	 8     8
	// 6	2+4  4+2 6

	arry4B[0] = strI[nCount * 4];
	arry4B[1] = strI[nCount * 4 + 1];
	arry4B[2] = strI[nCount * 4 + 2];
	arry4B[3] = strI[nCount * 4 + 3];

	//传入字符串，寻找在base64位置，并且放入数组
	Find_Index(&strI[nCount * 4], nIndex);

	//arry3B[0] =(nIndex[0] << 2 & 0xFC) | (nIndex[1] >> 4 & 0x03);
	strO[n++] = (nIndex[0] << 2 & 0xFC) | (nIndex[1] >> 4 & 0x03);

	//我们知道有等号的情况，只有可能补一个或者2个，凑成3个8字节转4个加密字符
	//‘=’我们补的8个位是0，不是‘=’的asilc
	//如果倒数第二个是等号，那么最后一个肯定也是等号
	if(arry4B[2]!='=')
	{
		//arry3B[1] = (nIndex[1] << 4 & 0xF0) | (nIndex[2] >> 2 & 0x0F);
		strO[n++] = (nIndex[1] << 4 & 0xF0) | (nIndex[2] >> 2 & 0x0F);

		//最后一个字符是否是等号的判断
		if (arry4B[3] == '=')
		{
			//如果是等号和0或
			//arry3B[2] = (nIndex[2] << 6 & 0xC0) | nIndex[3]; 
			strO[n++] = (nIndex[2] << 6 & 0xC0) | 0;
		}
		else
		{
			//arry3B[2] = (nIndex[2] << 6 & 0xC0) | nIndex[3]; 
			strO[n++] = (nIndex[2] << 6 & 0xC0) | nIndex[3];
		}
	}
	else
	{
		//如果是等号和0或
		//arry3B[1] = (nIndex[1] << 4 & 0xF0) | (nIndex[2] >> 2 & 0x0F);
		strO[n++] = (nIndex[1] << 4 & 0xF0) | (0);
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

//通过逆向表来查序号
void Find_Index2(const char* str, char* Index)
{
	for (int i = 0; i < 4; i++)
	{	
		Index[i] = dekey[str[i]];
	}
}