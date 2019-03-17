#pragma once
#include "../../commd/commend.h"
#include <QByteArray>
#include "screendialog.h"
#include "cmddialog.h"
#include "processdialog.h"
#include "filedialog.h"
class Session
{
public:
    Session(SOCKET s,sockaddr_in addr);
    ~Session();
public:
    SOCKET m_sock;
    sockaddr_in m_addr;

    int m_hour; //��ǰ��Сʱ
    int m_minute; //��ǰ�ķ�
    int m_second;//��ǰ����
    //dialog
    ScreenDialog* pScreen;
    CmdDialog* pCmd;
    ProcessDialog* pProcess;
    FileDialog*   pFile;
    //date
    QByteArray arrDate;
    //ÿ���ͻ��˶�Ҫ����һ�������Ļ���������Ϊ�Ǳ��ص���������͵��û�ã�ֱ��send������
    //QByteArray SendDate;
};

