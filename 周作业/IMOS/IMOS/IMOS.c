#include "IMOS.h"
static char szFlag[FLAG_MAX] = { '0' };

void Increse(tagPersonBaseData* BaseData)
{
    int flag = 0;
    FILE *fp = fopen("data.bin", "r+");
    if (NULL == fp)
    {
        goto NEXT_FREE;
    }
    flag = Islength();
    fseek(fp, sizeof(tagPersonBaseData)*flag, SEEK_SET);
    fwrite(BaseData, sizeof(tagPersonBaseData), 1, fp);
    //fwrite(BaseData->m_strName, sizeof(BaseData->m_strName), 1, fp);
    //fwrite(BaseData->m_strPhone, sizeof(BaseData->m_strPhone), 1, fp);
    //fwrite(BaseData->m_strAddress, sizeof(BaseData->m_strAddress), 1, fp);
    
    szFlag[flag] = 'U';
NEXT_FREE :
    if (fp)
    {
        fclose(fp);
        fp = NULL;
    }
}

void ShowAll()
{   
    tagPersonBaseData data;
    int n = 0;
    int i = 0;
    int j = 1;
    static int  flag = 0;
    FILE *fp = fopen("data.bin", "r");
    if (NULL == fp)
    {
        goto NEXT_FREE;
    }
    flag = 1;
    while (n = fread(&data, sizeof(tagPersonBaseData),1,fp )>0)
    {
       
        if (flag)
        {
            printf("\tname\t\tphone\t          address\n");
            flag = 0;
        } 
        if (szFlag[i] == 'U')
        {
            printf("%d:\t%s\t\t%s\t%s\n", j, data.m_strName, data.m_strPhone, data.m_strAddress);
            j++;
        }
       
        i++;
    }

NEXT_FREE:
    if (fp)
    {
        fclose(fp);
        fp = NULL;
    }

}

void ShowFlag()
{
    int i = 0;
    for (i = 0; i < FLAG_MAX; i++)
    {
        printf("%c", szFlag[i]);
    }

}
void InitFlag()//初始化标志位如果没有Flag.txt文件则默认文件为空
{
    int i = 0;
    char ch = '0';
    int n = 0;
    FILE* fp = fopen("Flag.txt", "r");
    if (NULL == fp)
    {
        for (i = 0; i < FLAG_MAX; i++)
        {
            szFlag[i] = 'F';
        }
        goto NEXT_FREE;
    }
    while (!feof(fp))
    {
        ch=fgetc(fp);
        szFlag[n++] = ch;
    }
 
NEXT_FREE:
    if (fp)
    {
        fclose(fp);
        fp = NULL;
    }

}

void ConserveFlag()//保存结束之后的标志位
{
    int i = 0;
    FILE* fp = fopen("Flag.txt", "w");
    if (NULL == fp)
    {
       
        goto NEXT_FREE;
    }
    for (i = 0; i < FLAG_MAX; i++)
    {
        fputc(szFlag[i], fp);
    }

NEXT_FREE:
    if (fp)
    {
        fclose(fp);
        fp = NULL;
    }

}

int Islength()//寻找是否有空闲空间
{
    int i = 0;
    for (i = 0; i < FLAG_MAX; i++)
    {
        if (szFlag[i] == 'F')
        {
            return i;
        }
    }
    printf("空间不足");
    return -1;//没有空间
}

void Delete(int n)
{   
    int i = 0;
    int nCount = 0;
    for (i = 0; i < FLAG_MAX; i++)
    {

        if (szFlag[i] == 'U')
        {
            nCount++;
        }
        if (nCount == n)
        {
            break;
        }
    }
    szFlag[i] = 'F';

}
void Find_str_name(char* str)
{
    int nStr1Len = 0;
    int nStr2Len = 0;
    int ncount = 0;
    int j = 0;
    size_t i = 0;
    tagPersonBaseData data;
    int n = 0;
    FILE* fp = fopen("data.bin", "r");
    if (NULL == fp)
    {
        goto NEXT_FREE;
    }
    while (n = fread(&data, sizeof(tagPersonBaseData), 1, fp)>0)
    {

       
        if (szFlag[i] == 'U')
        {
            //printf("%s\t%s\t%s\n", data.m_strName, data.m_strPhone, data.m_strAddress);
           /* if (str[i] == data.m_strName[i])
            {
                ShowId(i); 
            }*/
            
            nStr1Len = strlen(data.m_strName);
            nStr2Len = strlen(str);
            for (j = 0; j<nStr1Len&& nStr1Len - j >= nStr2Len; j++)
            {
                ncount = 0;
                while (str[ncount ] != '\0'&&data.m_strName[ncount+j] != '\0')
                {
                    if (str[ncount ] != data.m_strName[ncount+j])
                    {
                        break;
                    }
                    ncount++;
                }
                if (ncount == nStr2Len)
                {
                    ShowId(i);
                    break;
                }
            }
        }

        i++;
    }

NEXT_FREE:
    if (fp)
    {
        fclose(fp);
        fp = NULL;
    }
}
void ShowId(int id)
{
    tagPersonBaseData data;
    FILE *fp = fopen("data.bin", "r");
    if (NULL == fp)
    {
        goto NEXT_FREE;
    }
    fseek(fp, sizeof(tagPersonBaseData)*id, SEEK_SET);
    fread(&data, sizeof(tagPersonBaseData), 1, fp);
    
     //printf("name\tphone\taddress\n");
 
        if (szFlag[id] == 'U')
        {
            printf("%s\t%s\t%s\n", data.m_strName, data.m_strPhone, data.m_strAddress);
   
        }

NEXT_FREE:
    if (fp)
    {
        fclose(fp);
        fp = NULL;
    }
}

void Find_str_ph(char* str)
{

    int nStr1Len = 0;
    int nStr2Len = 0;
    int ncount = 0;
    int j = 0;
    size_t i = 0;
    tagPersonBaseData data;
    int n = 0;
    FILE* fp = fopen("data.bin", "r");
    if (NULL == fp)
    {
        goto NEXT_FREE;
    }
    while (n = fread(&data, sizeof(tagPersonBaseData), 1, fp)>0)
    {


        if (szFlag[i] == 'U')
        {
            //printf("%s\t%s\t%s\n", data.m_strName, data.m_strPhone, data.m_strAddress);
            /* if (str[i] == data.m_strName[i])
            {
            ShowId(i);
            }*/

            nStr1Len = strlen(data.m_strPhone);
            nStr2Len = strlen(str);
            for (j = 0; j<nStr1Len&& nStr1Len - j >= nStr2Len; j++)
            {
                ncount = 0;
                while (str[ncount] != '\0'&&data.m_strPhone[ncount + j] != '\0')
                {
                    if (str[ncount] != data.m_strPhone[ncount + j])
                    {
                        break;
                    }
                    ncount++;
                }
                if (ncount == nStr2Len)
                {
                    ShowId(i);
                    break;
                }
            }
        }

        i++;
    }

NEXT_FREE:
    if (fp)
    {
        fclose(fp);
        fp = NULL;
    }


}

void Find_str_adress(char* str)
{
    int nStr1Len = 0;
    int nStr2Len = 0;
    int ncount = 0;
    int j = 0;
    size_t i = 0;
    tagPersonBaseData data;
    int n = 0;
    FILE* fp = fopen("data.bin", "r");
    if (NULL == fp)
    {
        goto NEXT_FREE;
    }
    while (n = fread(&data, sizeof(tagPersonBaseData), 1, fp)>0)
    {


        if (szFlag[i] == 'U')
        {
            //printf("%s\t%s\t%s\n", data.m_strName, data.m_strPhone, data.m_strAddress);
            /* if (str[i] == data.m_strName[i])
            {
            ShowId(i);
            }*/

            nStr1Len = strlen(data.m_strAddress);
            nStr2Len = strlen(str);
            for (j = 0; j<nStr1Len&& nStr1Len - j >= nStr2Len; j++)
            {
                ncount = 0;
                while (str[ncount] != '\0'&&data.m_strAddress[ncount + j] != '\0')
                {
                    if (str[ncount] != data.m_strAddress[ncount + j])
                    {
                        break;
                    }
                    ncount++;
                }
                if (ncount == nStr2Len)
                {
                    ShowId(i);
                    break;
                }
            }
        }

        i++;
    }

NEXT_FREE:
    if (fp)
    {
        fclose(fp);
        fp = NULL;
    }

    
}
int find_id(int n)
{
    FILE* fp = fopen("data.bin", "r");
    int nIndex = 0;
    int i = 0;
    int nMax = 0;
    int nCount = 0;
    nMax = TotelU();
    if (NULL == fp&&nMax<n)
    {
        goto NEXT_FREE;
    }
    for (i = 0; i < FLAG_MAX; i++)
    {
        
        if (szFlag[i] == 'U')
        {
            nCount++;
        }
        if (nCount == n)
        {
            break;
        }
    }
    return i;


NEXT_FREE:
    if (fp)
    {
        fclose(fp);
        fp = NULL;
    }

}
int TotelU()
{
    int i = 0;
    int nCount = 0;
    for (i = 0; i < FLAG_MAX; i++)
    {
        if (szFlag[i] == 'U')
        {
            nCount++;
        }
    }
    return nCount;
}
void Alter(int id, tagPersonBaseData* BaseData)
{
    int i = 0;
    int nCount = 0;
    tagPersonBaseData data;
    FILE* fp = fopen("data.bin", "r+");
    if (NULL == fp)
    {
        goto NEXT_FREE;
    }
    for (i = 0; i < FLAG_MAX; i++)
    {

        if (szFlag[i] == 'U')
        {
            nCount++;
        }
        if (nCount == id)
        {
            break;
        }
    }
        fseek(fp, sizeof(tagPersonBaseData)*(i-1), SEEK_SET);
        memset(fp, sizeof(tagPersonBaseData), 0);
        fwrite(BaseData, sizeof(tagPersonBaseData), 1, fp);

NEXT_FREE:
        if (fp)
        {
            fclose(fp);
            fp = NULL;
        }
}
void DiskMagment()
{
    int index_F = 0;
    int index_U = 0;
    int i = 0;
    int j = 0;
    int k = 0;
    int Isindex_F = 0;
    int Isindex_U = 0;
    int nCurTotle = 0;
    int Dvalue = 0;
    tagPersonBaseData data;
    FILE* fp = fopen("data.bin", "r+");
    if (NULL == fp)
    {
        goto NEXT_FREE;
    }

    nCurTotle = TotelU();
    for (i = 0; i < nCurTotle; i++)
    {
        index_F = 0;
        index_U = 0;
        Isindex_F = 0;
        Isindex_U = 0;
        
        
        for (j = 0; j < FLAG_MAX; j++)
        {
            if (szFlag[j] == 'F'&&Isindex_F == 0)
            {
                index_F = j;
                Isindex_F = 1;
            }
            if (szFlag[j] == 'U'&&Isindex_U == 0 && Isindex_F == 1)
            {
                index_U = j;
                Isindex_U = 1;
            }

            if (Isindex_U == 1 && Isindex_F == 1)
            {
                Dvalue = index_U - index_F;
                for (k = index_U; k <FLAG_MAX; k++)
                {   
                    fseek(fp, sizeof(tagPersonBaseData)*k, SEEK_SET);
                    fread(&data, sizeof(tagPersonBaseData), 1, fp);
                    fseek(fp, sizeof(tagPersonBaseData)*(k - Dvalue), SEEK_SET);
                    fwrite(&data, sizeof(tagPersonBaseData), 1, fp);
                    //szStr[k - Dvalue] = szStr[k];
                    szFlag[k - Dvalue] = szFlag[k];
                }
                break;
            }
        }
    }

NEXT_FREE:
    if (fp)
    {
        fclose(fp);
        fp = NULL;
    }
}
void Statistical()//未完成
{
    FILE* fp = fopen("data.bin", "r");
    tagPersonBaseData data;
    //char *arry[FLAG_MAX] = { NULL };
    int n = 0;
    double nMax = 0;
    int i = 0;
    int j = 0;
    int k = 0;
    int l = 0;
    int flag = 0;
   tagStatistical tag[1024] = { 0 };
   
    nMax = TotelU();

    if (NULL == fp)
    {
        goto NEXT_FREE;
    }

    
    for (i = 0; i < FLAG_MAX; i++)
    {
        flag = 0;
        if (szFlag[i] == 'U')
        {
            fseek(fp, sizeof(tagPersonBaseData)*i, SEEK_SET);
            fread(&data, sizeof(tagPersonBaseData), 1, fp);
            //strcpy(tag[k].m_strName, data.m_strName);
            //tag[k].m_nCount = 1;
            //k++;
            for (j = 0; j < k; j++)
            {
                if (strcmp(tag[j].m_strName, data.m_strName) == 0)
                {
                    tag[j].m_nCount++;
                    flag = 1;
                    break;
                }
               
            }
            if (flag!=1)
            {
           
                strcpy(tag[k].m_strName, data.m_strName);
                tag[k].m_nCount = 1;
                k++;
           
            }
            

        }
    }

    for (l = 0; l < k; l++)
    {
        printf("%s: %d   %g%%", 
               tag[l].m_strName, tag[l].m_nCount, tag[l].m_nCount/nMax*100);
        printf("\r\n");
    }
       
    

    
NEXT_FREE:
    if (fp)
    {
        fclose(fp);
        fp = NULL;
    }
}