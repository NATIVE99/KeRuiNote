#include "StrMS.h"
static char    szStr[Capacity] = {0};
static char    szFlag[Capacity] = {0};

void Increase(char* str)//�����ַ���
{   
    int nLength = 0;
    int i = 0;
    int j = 0;
    nLength = strlen(str);
    i = IsLength(nLength);
    szFlag[i] = '1';
    if (i < 0)
    {
        printf("�ռ䲻��\r\n");
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
void ShowAll()//��ʾ�����ַ���
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
void Record(int i)//��¼ szStr����λ�ñ�ʹ�õ����
{
    szFlag[i] = 'U';//��ʾ��ռ��
}
void InitFlag()//��ʼ��szStr��λ��ΪF״̬
{
    int i = 0;

    for (i = 0; i < Capacity; i++)
    {
        szFlag[i] = 'F';//F��ʾ����
    }
}

void ShowFlag()//����ʹ�ú��� �۲�ʹ�����
{
    int i = 0;

    for (i = 0; i < Capacity; i++)
    {
        printf("%c", szFlag[i]);
    }
    printf("\r\n");
}

void ShowFlag_1()//�ⲿ���ùۿ�UF���ݴ洢���
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
int IsLength(int strlength)//����Ƿ����㹻�Ŀռ�����ַ���������з����±꣬���û�з���-1��
{
    int nCount = 0;//��¼�ж��ٸ�������F
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

void Delete()//�������
{
    memset(szStr, 0, Capacity);
    InitFlag();
}

void Statistical()//ͳ���ַ�����ĸ
{
    char szAlphabet[52] = { 0 };
    int szRecord[52] = { 0 };
    int i = 0;
    int j = 0;
    int nCount = 0;;
    for (i = 0; i < 26; i++)//��ʼ��Сд����ĸ
    {
        szAlphabet[i] = 'a' + i;
        //printf("%c ", arry_alphabet[i]);
    }
    for (i = 26; i < 52; i++)//��ʼ����д��ĸ
    {
        szAlphabet[i] = 39 + i;
        //printf("%c ", arry_alphabet[i]);
    }
    for (i = 0; i < Capacity; i++)//��szStr�����е�λ�ñȽ�
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
void ShowStatistical(int* szRecord)//��ʾͳ�ƽ��
{
    int i = 0;
    double nSum = 0;
    for (i = 0; i < 52; i++)//��ʾ������ĸ����
    {
        if (szRecord[i] != 0)
        {
            nSum = nSum + szRecord[i]; 
        }
    }
    printf("�ܹ���%d�� ", (int)nSum);

    for (i = 0; i < 26; i++)//��ʾСд��ĸ
    {
        if (szRecord[i] != 0)
        {
            printf("%c��%d��ռ%g%% ", i + 97, szRecord[i], szRecord[i]/nSum*100);
        }
    }
    for (i = 26; i < 52; i++)
    {
        if (szRecord[i] != 0)//��ʾ��д��ĸ
        {
            printf("%c��%d��ռ%g%% ", i + 39, szRecord[i], szRecord[i] / nSum * 100);
        }
    }
    
    printf("\r\n");
   
}

int Find_str(char* str)//֧��ģ�������ַ���������鵽�����ַ����±꣬û�ҵ�����-1
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
int TotalStr()//�����ж��ٸ��ַ���
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

int Find_id(int id)//��id�����ַ���
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
void ShowId(int index)//��ʾid���ҵ��ַ���
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
void Alter(int n, char* str)//�޸��ַ���
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
void DiskMagment()//��Ƭ����
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