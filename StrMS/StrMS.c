#include "StrMS.h"
static char    szStr[Capacity] = {0};
static char    szFlag[Capacity] = {0};

void Increase(char* str)//增加字符串
{   
    int nLength = 0;
    int i = 0;
    int j = 0;
    nLength = strlen(str);
    i = IsLength(nLength);
    szFlag[i] = '1';
    if (i < 0)
    {
        printf("空间不足\r\n");
        return ;
    }
    while (str[j] != '\0')
    {
        szStr[i] = str[j];
        Record(i+1);
        i++;
        j++;
    }
    szStr[i] = '\0';
    Record(i);
    
    
    

}
void ShowAll()//显示所有字符串
{
    int i = 0;
    for (i = 0; i < Capacity; i++)
    {
        if (szFlag[i] == '1')
        {
            while (szStr[i]!='\0')
            {
                printf("%c", szStr[i]);
                i++;
            }
            printf("\r\n");
        }
    }
   
}
void Record(int i)//记录 szStr各个位置被使用的情况
{
    szFlag[i] = 'U';//表示被占用
}
void InitFlag()//初始化szStr各位置为F状态
{
    int i = 0;

    for (i = 0; i < Capacity; i++)
    {
        szFlag[i] = 'F';//F表示空闲
    }
}

void ShowFlag()//测试使用函数 观察使用情况
{
    int i = 0;

    for (i = 0; i < Capacity; i++)
    {
        printf("%c", szFlag[i]);
    }
    printf("\r\n");
}

void ShowFlag_1()//外部调用观看UF数据存储情况
{

    int i = 0;

    for (i = 0; i < Capacity; i++)
    {
        if (szFlag[i] == '1')
        {
            printf("%c", 'U');
        }
        else
        {
            printf("%c", szFlag[i]);
        }
       
    }
    printf("\r\n");

}
int IsLength(int strlength)//检查是否有足够的空间放下字符串，如果有返回下标，如果没有返回-1；
{
    int nCount = 0;//记录有多少个连续的F
    int i = 0;
    int j = 0;
    for (i = 0; i < Capacity - strlength; i++)
    {
        for (j = i; j < Capacity; j++)
        {
            if (szFlag[j] == 'U' || szFlag[j] == '1')
            {
                break;
            }
            nCount++;
            if (nCount>strlength)
            {
                return i;
            }
        }
    }
    return -1;
}

void Delete()//清空内容
{
    memset(szStr, 0, Capacity);
    InitFlag();
}

void Statistical()//统计字符串字母
{
    char szAlphabet[52] = { 0 };
    int szRecord[52] = { 0 };
    int i = 0;
    int j = 0;
    int nCount = 0;;
    for (i = 0; i < 26; i++)//初始化小写字字母
    {
        szAlphabet[i] = 'a' + i;
        //printf("%c ", arry_alphabet[i]);
    }
    for (i = 26; i < 52; i++)//初始化大写字母
    {
        szAlphabet[i] = 39 + i;
        //printf("%c ", arry_alphabet[i]);
    }
    for (i = 0; i < Capacity; i++)//将szStr中所有的位置比较
    {
        for (j = 0; j < 52; j++)
        {
            if (szStr[i] == szAlphabet[j])
            {
                szRecord[j] = szRecord[j] + 1;
            }
        }
    }
    ShowStatistical(szRecord);

}
void ShowStatistical(int* szRecord)//显示统计结果
{
    int i = 0;
    double nSum = 0;
    for (i = 0; i < 52; i++)//显示所有字母数量
    {
        if (szRecord[i] != 0)
        {
            nSum = nSum + szRecord[i]; 
        }
    }
    printf("总共有%d个 ", (int)nSum);

    for (i = 0; i < 26; i++)//显示小写字母
    {
        if (szRecord[i] != 0)
        {
            printf("%c有%d个占%g%% ", i + 97, szRecord[i], szRecord[i]/nSum*100);
        }
    }
    for (i = 26; i < 52; i++)
    {
        if (szRecord[i] != 0)//显示大写字母
        {
            printf("%c有%d个占%g%% ", i + 39, szRecord[i], szRecord[i] / nSum * 100);
        }
    }
    
    printf("\r\n");
   
}

int Find_str(char* str)//支持模糊查找字符串，如果查到返回字符串下标，没找到返回-1
{
    int i = 0;
    int j = 0;
    int nStr1Len = 0;
    int nStr2Len = 0;
    int ncount = 0;
    nStr2Len = strlen(str);
    for (i = 0; i < Capacity; i++)
    {
        if (szFlag[i] == '1')
        {
            nStr1Len = strlen(&szStr[i]);
            for (j = 0; j < nStr1Len&& nStr1Len - j >= nStr2Len; j++)
            {
                ncount = 0;
                while (szStr[ncount + j+i] != '\0'&&str[ncount] != '\0')
                {
                    if (szStr[ncount + j+i] != str[ncount])
                    {
                        break;
                    }
                    ncount++;
                }
                if (ncount == nStr2Len)
                {
                    return i;
                }

            }
        }
    }
    return -1;
}
int TotalStr()//返回有多少个字符串
{ 
    int i = 0;
    int nSum = 0;
    for (i = 0; i < Capacity; i++)
    {
        if (szFlag[i] == '1')
        {
            nSum++;
        }
    }
    
    return nSum;
}

int Find_id(int id)//按id查找字符串
{
    int nTotle_str = 0;
    int i = 0;
    int nSum = 0;

    nTotle_str = TotalStr();
    if (id>nTotle_str)
    {
      
        return 0;
    }
   
   
        for (i = 0; i < Capacity; i++)
        {
            if (szFlag[i] == '1')
            {
                nSum++;
            }
            if (nSum == id)
            {
                return i;
            }
        }
   
  
   
}
void ShowId(int index)//显示id查找的字符串
{
    int i = 0;
    i = index;
    while (szStr[index] != '\0')
    {
        printf("%c", szStr[index]);
        index++;
    }
    printf("\r\n");
}
void Alter(int n, char* str)//修改字符串
{
    int index = 0;
    int index_1 = 0;
    int i = 0;
    int nLength_1 = 0;
    int nLength_2 = 0;

    index = Find_id(n);
    nLength_1 = strlen(&szStr[index]);
    nLength_2 = strlen(str);
    if (nLength_1<nLength_2)
    {
        Increase(str);
        while (szStr[index] != '\0')
        {
            szFlag[index] = 'F';
            index++;
        }
        szFlag[index] = 'F';
    }
    else
    {
        index_1 = index;
        while (szStr[index] != '\0')
        {
            szFlag[index+1] = 'F';
            index++;
        }
        while (str[i] != '\0')
        {
            szStr[index_1] = str[i];
            index_1++;
            szFlag[index_1] = 'U';
            i++;
        }
        szStr[index_1] = '\0';
    }

}
void DiskMagment()//碎片整理
{
    int index_F = 0;
    int index_1 = 0;
    int i = 0;
    int j = 0;
    int k = 0;
    int Dvalue = 0;
    int nCurTotle = 0;
    int Isindex_F = 0;
    int Isindex_1 = 0;


    nCurTotle = TotalStr();
    for (i = 0; i < nCurTotle; i++)
    {
        index_F = 0;
        index_1 = 0;
        Isindex_F = 0;
        Isindex_1 = 0;
        for (j = 0; j < Capacity; j++)
        {
            if (szFlag[j] == 'F'&&Isindex_F==0)
            {
                index_F = j;
                Isindex_F = 1;
                
            }
            if (szFlag[j] == '1'&&Isindex_1 == 0 && Isindex_F == 1)
            {
                index_1 = j;
                Isindex_1 = 1;
            }
            
            if (Isindex_1 == 1 && Isindex_F==1)
            {   
                Dvalue = index_1 - index_F;
                for (k = index_1; k < Capacity; k++)
                {
                    szStr[k - Dvalue] = szStr[k];
                    szFlag[k - Dvalue] = szFlag[k];
                }
                break;
            }
        }
    }
    
}