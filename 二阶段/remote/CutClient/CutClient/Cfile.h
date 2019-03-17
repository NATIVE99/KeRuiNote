#pragma once
#include <fstream> 
#include <windows.h>
#include "CBuffer.h"
#include "../../commd/commend.h"
class Cfile
{
public:
    Cfile(SOCKET s);
    ~Cfile();
    void EnumFile(char* lpFileName);
    void EnumDevir();
    void SendfileName();
    void SendFile(char* lpFileName);
private:
    CBuffer mybuf;
    SOCKET  m_sSever;
};

