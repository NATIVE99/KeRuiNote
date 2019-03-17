#include "MySocket.h"

#include "zlib.h"
#include "zconf.h"
#pragma comment(lib,"zlibstat.lib")
#include <qdebug.h>
extern CRITICAL_SECTION CriticalSection;
MySocket::MySocket()
{
   
}


MySocket::~MySocket()
{
    closesocket(m_sSever);
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


//�󶨺ͼ����˿�
void MySocket::BindAndListen()
{
    m_sSever = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in addr;
    addr.sin_port = htons(87654);
    addr.sin_family = AF_INET;
    addr.sin_addr.S_un.S_addr = inet_addr("0.0.0.0");

    int  nRet=bind(m_sSever, (sockaddr*)&addr, sizeof(addr));
    if(nRet== SOCKET_ERROR)
    {
        return;
    }

    listen(m_sSever, SOMAXCONN);


}


//������
SOCKET MySocket::Accept()
{
    sockaddr_in addr = {0};
    int nSize = sizeof(sockaddr_in);
    addr.sin_family = AF_INET;
    SOCKET client=accept(m_sSever, (sockaddr*)&addr, &nSize);

    Session* ps = new Session(client, addr);

   //����ʱ��
    UpDateTime(ps);
    //�����û���
    m_client.insert(std::pair<SOCKET, Session*>(client, ps));

    emit sig_sendsession(ps);
    return client;
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

void MySocket::Send(SOCKET s, char szbuf[], int nLength)
{

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
//ͨ��socket�õ�session
Session* MySocket::GetSession(SOCKET s)
{

    for(auto &i:m_client)
    {
        if(i.first==s)
        {
            return i.second;
        }
    }
}

void MySocket::Heartbeat()
{
   
    if(!m_client.empty())
    {
        for(auto &i:m_client)
        {
            SendData2(i.first, CUT_HEART, NULL, 0);
        }
    }
}

void MySocket::UpDateTime(Session* ps)
{
    //��ȡʱ��
    QTime current_time = QTime::currentTime();
    ps->m_minute = current_time.minute();
    ps->m_hour = current_time.hour();
    ps->m_second = current_time.second();
}

/**************************************************************************/
//IOCP����

//�����յ�����Ϣ
void ThreadPoolFunc(MySocket* pSocket)
{
    ULONG_PTR dwCompleteKey = 0;
    LPOVERLAPPED lpOl = NULL;
    BOOL bRet = false;
    DWORD NumberOfBytes = 0;//recv�Ļ����յ����ֽ�����send�Ļ����͵��ֽ���
    while (1)
    {
        bRet=GetQueuedCompletionStatus(pSocket->m_hPort,//IOCP�Ķ˿�
            &NumberOfBytes,//������ֽ���
            &dwCompleteKey,//��������socket
            &lpOl,
            INFINITE);

        if (!bRet)
        {
            break;
        }
        //תΪMyOverlapped�ṹ��
        MyOverlapped* pRecvOl = (MyOverlapped*)lpOl;

        if (pRecvOl->nType == IO_READ)
        {
            //�����յ�������
            //��ѹ��
      
            Session* ps = pSocket->GetSession((SOCKET)dwCompleteKey);
            ps->arrDate.append(pRecvOl->pRecvBuf, NumberOfBytes);
  
            delete lpOl;

            //��黺�����Ƿ�����һ�����Ĵ�С��������㣬����
            while (ps->arrDate.size() >= sizeof(tagHead))
            {
                tagHead* ptagHead = (tagHead*)ps->arrDate.data();
                int n = ps->arrDate.size();
                //�жϰ��ĳ���
                if (ps->arrDate.size() >= ptagHead->nDateSize + sizeof(tagHead))
                {
                    //˵���Ѿ��������յ���һ���������Կ�ʼ������

                    char* pDate = new char[ptagHead->nDate];
                    uLong tlen = ptagHead->nDate;
                   int n=uncompress((Bytef *)pDate, &tlen, (Bytef *)(ps->arrDate.data() + sizeof(tagHead)), ptagHead->nDateSize);
          
                    //memcpy(pDate, ps->arrDate.data() + sizeof(tagHead), ptagHead->nDateSize);
 
                    switch (ptagHead->bType)
                    {
                    case SEND_WINDOWS:
                    {
                
                        QByteArray arr(pDate, ptagHead->nDate);
                        emit pSocket->sig_sendWindows(arr);
                        //�������ɾ���������İ�
                        ps->arrDate.remove(0, ptagHead->nDateSize + sizeof(tagHead));
                        delete[] pDate;
                        SendData(ps->m_sock, CUT_WINDOWS, NULL, 0);
                       
                    }
                    break;
                    case SEND_CMD:
                    {
                        QByteArray arr(pDate, ptagHead->nDate);
                        emit pSocket->sig_sendCmd(arr);
                        ps->arrDate.remove(0, ptagHead->nDateSize + sizeof(tagHead));
                        delete[] pDate;
                    }
                    break;
                    case SEND_PROCESS:
                    {
                        QByteArray arr(pDate, ptagHead->nDate);
                        emit pSocket->sig_sendProcess(arr);
                        ps->arrDate.remove(0, ptagHead->nDateSize + sizeof(tagHead));
                        delete[] pDate;

                    }
                    break;
                    case SEND_END:
                    {
                        //��ʾ�رս��̳ɹ�
                        QByteArray arr(pDate, ptagHead->nDate);
                        emit pSocket->sig_deleteProcess(arr);
                        ps->arrDate.remove(0, ptagHead->nDateSize + sizeof(tagHead));
                        delete[] pDate;
                    }
                    break;
                    case SEND_FILE:
                    {
                        QByteArray arr(pDate, ptagHead->nDate);
                        emit pSocket->sig_sendFile(arr);
                        ps->arrDate.remove(0, ptagHead->nDateSize + sizeof(tagHead));
                        delete[] pDate;
                    }
                    break;
                    case SEND_DOWNLOAD:
                    {
                        QByteArray arr(pDate, ptagHead->nDate);
                        emit pSocket->sig_sendDownload(arr);
                        ps->arrDate.remove(0, ptagHead->nDateSize + sizeof(tagHead));
                        delete[] pDate;
                    }
                    break;
                    case SEND_HEART:
                    {
                        //����ʱ��
                        QByteArray arr(pDate, ptagHead->nDate);
                        pSocket->UpDateTime(ps);
                        emit  pSocket->sig_sendUpate(ps);
                        ps->arrDate.remove(0, ptagHead->nDateSize + sizeof(tagHead));
                        delete[] pDate;
                    }
                    break;
                    }

                }
                else
                {
                    break;
                }

            }
            pSocket->PostRecv((SOCKET)dwCompleteKey);
        }
        else if(pRecvOl->nType == IO_CLOSE)
        {
            //����clientsocket�ر�
            qDebug() << "ss";
        }
       
    }
}




//�ͻ������ӷ�����
void ThreadAccept(MySocket* pSocket)
{
    while(1)
    {
        //���û����ӺͶ˿�����
      SOCKET  client=pSocket->Accept();

      CreateIoCompletionPort((HANDLE)client, 
          pSocket->m_hPort, 
          (ULONG_PTR)client,//����socket
          8);
      pSocket->PostRecv(client);
    
    }
}
//����IOCP
void  MySocket::ModleIocp()
{
    //���api����������
    //1.����iocp�˿�
    //2.��socket�Ͷ˿ڰ�

    //���ȴ����˿�
   m_hPort =CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, 8);

   //��������߳�iocp���̳߳ع����߳�
   for (int i = 0; i<8; i++)
   {
       std::thread(ThreadPoolFunc,this).detach();
   }


  //�Ѷ˿ں�socket��
  CreateIoCompletionPort((HANDLE)m_sSever, 
      m_hPort, 
      (ULONG_PTR)m_sSever,//����socket
      8);

  //����һ���߳�ר��accpet
  std::thread t(ThreadAccept, this);
  t.detach();
 
}

//��IOCP��������Ͷ���հ�����Ϣ��Ȼ��ϵͳ���Զ�����
void MySocket::PostRecv(SOCKET sClient)
{
    //�������ʾ�����ɹ��ˣ�Ͷ��һ��recv����
    DWORD dwReadedBytes = 0;
    //new�Ļ����ݸ�������Ϣ���߳�ʱ���ݻ��ڣ�Ҫ�ڴ������ݵ��߳���delete
    MyOverlapped* pRecvOl = new MyOverlapped(IO_READ);

    WSABUF wsaBuf;
    wsaBuf.buf = pRecvOl->pRecvBuf;
    wsaBuf.len = 8192;

    DWORD flags = 0;

    int nRet = WSARecv(sClient,
        &wsaBuf,
        1,
        &dwReadedBytes,//�յ����ֽڣ�һ��û�ã���ΪGetQueuedCompletionStatus�Ĳ������յ����ֽ�
        &flags,
        &pRecvOl->ol,//����IOCP��Ҫ�İ�---�ص�IO---OVERLAPPED������ʹ��һ���ṹ������ӵ�����
        NULL);
    if (nRet == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING) {
        return ;
    }
}

/***************************************************************************/