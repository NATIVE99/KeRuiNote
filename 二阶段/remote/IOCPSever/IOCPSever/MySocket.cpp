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
//win平台socket启动需要的
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


//绑定和监听端口
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


//等连接
SOCKET MySocket::Accept()
{
    sockaddr_in addr = {0};
    int nSize = sizeof(sockaddr_in);
    addr.sin_family = AF_INET;
    SOCKET client=accept(m_sSever, (sockaddr*)&addr, &nSize);

    Session* ps = new Session(client, addr);

   //更新时间
    UpDateTime(ps);
    //加入用户表
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
//通过socket拿到session
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
    //获取时间
    QTime current_time = QTime::currentTime();
    ps->m_minute = current_time.minute();
    ps->m_hour = current_time.hour();
    ps->m_second = current_time.second();
}

/**************************************************************************/
//IOCP函数

//处理收到的消息
void ThreadPoolFunc(MySocket* pSocket)
{
    ULONG_PTR dwCompleteKey = 0;
    LPOVERLAPPED lpOl = NULL;
    BOOL bRet = false;
    DWORD NumberOfBytes = 0;//recv的话是收到的字节数，send的话发送的字节数
    while (1)
    {
        bRet=GetQueuedCompletionStatus(pSocket->m_hPort,//IOCP的端口
            &NumberOfBytes,//传输的字节数
            &dwCompleteKey,//传过来的socket
            &lpOl,
            INFINITE);

        if (!bRet)
        {
            break;
        }
        //转为MyOverlapped结构体
        MyOverlapped* pRecvOl = (MyOverlapped*)lpOl;

        if (pRecvOl->nType == IO_READ)
        {
            //处理收到的数据
            //解压缩
      
            Session* ps = pSocket->GetSession((SOCKET)dwCompleteKey);
            ps->arrDate.append(pRecvOl->pRecvBuf, NumberOfBytes);
  
            delete lpOl;

            //检查缓冲区是否满足一个包的大小，如果满足，则处理。
            while (ps->arrDate.size() >= sizeof(tagHead))
            {
                tagHead* ptagHead = (tagHead*)ps->arrDate.data();
                int n = ps->arrDate.size();
                //判断包的长度
                if (ps->arrDate.size() >= ptagHead->nDateSize + sizeof(tagHead))
                {
                    //说明已经完整的收到了一个包，可以开始处理了

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
                        //处理完后删除缓冲区的包
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
                        //表示关闭进程成功
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
                        //更新时间
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
            //处理clientsocket关闭
            qDebug() << "ss";
        }
       
    }
}




//客户端连接服务器
void ThreadAccept(MySocket* pSocket)
{
    while(1)
    {
        //有用户连接和端口连接
      SOCKET  client=pSocket->Accept();

      CreateIoCompletionPort((HANDLE)client, 
          pSocket->m_hPort, 
          (ULONG_PTR)client,//传递socket
          8);
      pSocket->PostRecv(client);
    
    }
}
//启动IOCP
void  MySocket::ModleIocp()
{
    //这个api有两个功能
    //1.创建iocp端口
    //2.把socket和端口绑定

    //首先创建端口
   m_hPort =CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, 8);

   //创建多个线程iocp有线程池管理线程
   for (int i = 0; i<8; i++)
   {
       std::thread(ThreadPoolFunc,this).detach();
   }


  //把端口和socket绑定
  CreateIoCompletionPort((HANDLE)m_sSever, 
      m_hPort, 
      (ULONG_PTR)m_sSever,//传递socket
      8);

  //开启一个线程专门accpet
  std::thread t(ThreadAccept, this);
  t.detach();
 
}

//向IOCP队列里面投递收包的消息，然后系统会自动处理
void MySocket::PostRecv(SOCKET sClient)
{
    //到这里表示关联成功了，投递一个recv请求
    DWORD dwReadedBytes = 0;
    //new的话传递给处理消息的线程时数据还在，要在处理数据的线程中delete
    MyOverlapped* pRecvOl = new MyOverlapped(IO_READ);

    WSABUF wsaBuf;
    wsaBuf.buf = pRecvOl->pRecvBuf;
    wsaBuf.len = 8192;

    DWORD flags = 0;

    int nRet = WSARecv(sClient,
        &wsaBuf,
        1,
        &dwReadedBytes,//收到的字节，一般没用，因为GetQueuedCompletionStatus的参数有收到的字节
        &flags,
        &pRecvOl->ol,//传递IOCP需要的包---重叠IO---OVERLAPPED，这里使用一个结构体带附加的数据
        NULL);
    if (nRet == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING) {
        return ;
    }
}

/***************************************************************************/