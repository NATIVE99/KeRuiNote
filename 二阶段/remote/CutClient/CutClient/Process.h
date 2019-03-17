#pragma once

#include <windows.h>
#include <Tlhelp32.h>
#include "CBuffer.h"
#include "../../commd/commend.h"
class CProcess
{
public:
    CProcess(SOCKET s);
    ~CProcess();
    void GetProcess();
    void SendProcess();
private:
    CBuffer cMyDate;
    SOCKET m_sSever;
};

