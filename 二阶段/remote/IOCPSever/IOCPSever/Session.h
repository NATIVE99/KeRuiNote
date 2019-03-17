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

    int m_hour; //当前的小时
    int m_minute; //当前的分
    int m_second;//当前的秒
    //dialog
    ScreenDialog* pScreen;
    CmdDialog* pCmd;
    ProcessDialog* pProcess;
    FileDialog*   pFile;
    //date
    QByteArray arrDate;
    //每个客户端都要配有一个发包的缓冲区，因为是本地的所以这里偷懒没用，直接send发包了
    //QByteArray SendDate;
};

