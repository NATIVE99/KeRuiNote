#include "IMOS.h"

int main()
{
    int nSelect = 1;
    int n = 0;
    int nChoose = 0;
    char str[1024] = {0};
    int nIndex = 0;
    tagPersonBaseData tagPBD = {0};
   // printf("%d", sizeof(tagPBD));
    InitFlag();
    while (nSelect)
    {
        printf("          ��ϵ�˹���ϵͳ  \12");
        printf("*********************************\12");
        printf("*[1]increase      [2]delete     *\12");
        printf("*[3]ShowAll       [4]alter      *\12");
        printf("*[5]find_id       [6]DiskMagment*\12");
        printf("*[7]Statistical   [8]ShowFlag   *\12");
        printf("*[9]find_str      [0]quit       *\12");
        printf("*********************************\12");
        printf("��ѡ��> ");
        scanf_s("%d", &nSelect);
      
        switch (nSelect)
        {
        case 1:
            getchar();
            printf("name:");
            gets(&tagPBD.m_strName);

            printf("phone:");
            gets(&tagPBD.m_strPhone);

            printf("address:");
            gets(&tagPBD.m_strAddress);
            Increse(&tagPBD);

            break;
        case 2:
            printf("������Ҫɾ����ϵ�˵�id��");
            scanf_s("%d", &n);
            Delete(n);
            break;
        case 3:
            ShowAll();
            break;
        case 4:
           printf("������Ҫxiugai��ϵ�˵�id��");
           scanf_s("%d", &n);
            getchar();
            
            printf("name:");
            gets(&tagPBD.m_strName);

            printf("phone:");
            gets(&tagPBD.m_strPhone);

            printf("address:");
            gets(&tagPBD.m_strAddress);
            Alter(n, &tagPBD);
            break;
        case 5:
            printf("��������ҵڼ�����ϵ�ˣ�");
            scanf_s("%d", &n);
            nIndex=find_id(n);
            ShowId(nIndex);
            break;
        case 6:
            DiskMagment();
            break;
        case 7:
            Statistical();
            break;
        case 8:
           //ShowFlag
            ShowFlag();
            break;
        case 9:
            printf("          ��ѯ��ҳ��  \12");
            printf("*********************************\12");
            printf("*[1]Find_str_name [2]Find_str_Ph*\12");
            printf("*[3]Find_str_adress             *\12");
            printf("*********************************\12");
            printf("��������Ҫ���ҷ�ʽ��");
            scanf("%d", &nChoose);
            printf("��������Ҫ���ҵ��ַ�����");
            scanf("%s", str);
            switch (nChoose)
            {
             case 1:
                 Find_str_name(str);
                 break;
             case 2:
                 Find_str_ph(str);
                 break;
             case 3:
                 Find_str_adress(str);
                 break;
            }
           // printf("��������Ҫ���ҵ��ַ�����");
           // scanf("%s", str);
            //nIndex=Find_str(str);
            //ShowId(nIndex);
            break;
        default:
            break;
        }
    
    }
    ConserveFlag();
   

    return 0;
}