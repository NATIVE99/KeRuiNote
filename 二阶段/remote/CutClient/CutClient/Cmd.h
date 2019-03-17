#pragma once
#include <windows.h>
#include <thread>
#include "../../commd/commend.h"
class Cmd
{
public:
    Cmd();
    ~Cmd();
    void RecvCommnd(char* szbuf, int nLenth);
    void Send();
    void CreateCmd(SOCKET s);
private:
    HANDLE hMyWrite;
    HANDLE hCmdRead;

    HANDLE hMyRead;
    HANDLE hCmdWrite;
    SOCKET m_Sever;
};

