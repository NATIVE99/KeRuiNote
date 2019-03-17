#pragma once

#include <Winsock2.h>
#include <map>
#include "Cmd.h"
#include "Process.h"
#include "Cfile.h"
#pragma comment(lib,"Ws2_32.lib")
class MySocket
{
public:
    MySocket();
    ~MySocket();
    void Start();
    bool Connect();
    bool Recv(SOCKET s, char szbuf[], int nLenth);

    bool RecvData(SOCKET s, char* pBuf, int nLength);

    
    void InitCmd();
    void InitPro();
    void InitFile();
public:
    SOCKET m_sSever;
    int nIndex;
    Cmd* pCmd;
    CProcess* pPro;
    Cfile*    pFile;
    
};

