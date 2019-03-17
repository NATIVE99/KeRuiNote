#include "Process.h"
#include <stdio.h>

CProcess::CProcess(SOCKET s)
{
    m_sSever = s;
}


CProcess::~CProcess()
{
}

struct tagData
{
    int nPid;
    int nLength;
    char  data[];
};

void CProcess::GetProcess()
{

    HANDLE         hProcessSnap = NULL;
    BOOL           bRet = FALSE;
    PROCESSENTRY32 pe32 = { 0 };
    hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    pe32.dwSize = sizeof(PROCESSENTRY32);

    //PROCESSENTRY32


    if (Process32First(hProcessSnap, &pe32))
    {

        do
        {
            
            int n = wcslen(pe32.szExeFile)*2;
            tagData* pDate = (tagData*)new char[sizeof(tagData) + n];
            pDate->nPid = pe32.th32ProcessID;
            pDate->nLength = n;
            memcpy((char*)pDate + sizeof(tagData), pe32.szExeFile, n);

            cMyDate.Write((PBYTE)pDate, sizeof(tagData) + n);

            delete[] pDate;
         
      
        } while (Process32Next(hProcessSnap, &pe32));
    }

    CloseHandle(hProcessSnap);
}

void CProcess::SendProcess()
{
    SendData(m_sSever, SEND_PROCESS,(char*)cMyDate.GetBuffer(0), cMyDate.GetBufferLen());
}