#include "StrMS.h"

int main()
{
    char szStr_1[Capacity] = {0};
    int result = 0;
    int nSelect = 1;
    int nId = 0;
    int nIndex = 0;
    InitFlag();
    
    while (nSelect)
    {
        printf("          �ַ�������ϵͳ  \12");
        printf("*********************************\12");
        printf("*[1]increase      [2]delete     *\12");
        printf("*[3]ShowAll       [4]alter      *\12");
        printf("*[5]find_id       [6]DiskMagment*\12");
        printf("*[7]Statistical   [8]ShowFlag   *\12");
        printf("*[9]find_str      [0]quit       *\12");
        printf("*********************************\12");
        printf("��ѡ��> ");
        scanf("%d", &nSelect);
        switch (nSelect)
        {
            case 1:
                printf("��������Ҫ������ַ�����");
                scanf("%s", szStr_1);
                Increase(szStr_1);
                    //ShowFlag();
                break;
            case 2:
                Delete();
                break;
            case 3:
                ShowAll();
                break;
            case 4:
                printf("��������Ҫ������ַ�����");
                scanf("%s", szStr_1);
                printf("��������Ҫ�����id��");
                scanf("%d", &nId);
                Alter(nId,szStr_1);
                break;
            case 5:
                printf("��������Ҫ���ҵ�id��");
                scanf("%d", &nId);
                nIndex = Find_id(nId);
                ShowId(nIndex);
                break;
            case 6:
                DiskMagment();
                //ShowFlag();
                break;
            case 7:
                Statistical();
                break;
            case 8:
                ShowFlag_1();
                break;
            case 9:
                printf("��������Ҫ���ҵ��ַ�����");
                scanf("%s", szStr_1);
                nIndex=Find_str(szStr_1);
                ShowId(nIndex);
            default:
                break;
        }
    }

    return 0;
}