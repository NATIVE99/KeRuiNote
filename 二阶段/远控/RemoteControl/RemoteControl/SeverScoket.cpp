#include "SeverScoket.h"
#include "RemoteControl.h"
extern RemoteControl* g_pWindow;
SeverScoket::SeverScoket(QObject *parent)
    : QObject(parent)
{
}

SeverScoket::~SeverScoket()
{

    closesocket(m_SeverSocket);


    //释放所有socket
    QMap<SOCKET, QSharedPointer<CClientSession>>::iterator it = m_Clients.begin();
    for (; it != m_Clients.end(); it++) 
    {

        closesocket(it.key());
    }
    m_Clients.clear();

}
//初始化ws2_32.dll的版本
void SeverScoket::StartUp()
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

void SeverScoket::CreateSocketAndBind(int Port )
{
    //sockaddr_in
    m_SeverAddr.sin_family = AF_INET;
    m_SeverAddr.sin_port = htons(Port);
    m_SeverAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
    int nLength = sizeof(m_SeverAddr);
    // cerate socket
    m_SeverSocket = socket(AF_INET, SOCK_STREAM, 0);
    //check
    if (m_SeverSocket == INVALID_SOCKET)
    {
        OutputDebugStringA("INVALID_SOCKET");
        return;
    }

    //bind
    int nRetB = bind(m_SeverSocket, (struct sockaddr*)& m_SeverAddr, nLength);
    //check
    if (nRetB == SOCKET_ERROR)
    {
        OutputDebugStringA("nRetB== SOCKET_ERROR|| nRetL==SOCKET_ERROR");
        return;
    }
    //listen
    listen(m_SeverSocket, SOMAXCONN);
}


bool SeverScoket::Accept()
{
    sockaddr_in RecvAddr = { 0 };
    RecvAddr.sin_family = AF_INET;
    int nLength = sizeof(RecvAddr);
   SOCKET SocketClient=accept(m_SeverSocket, (struct sockaddr*) &RecvAddr, &nLength);
   if(SocketClient== SOCKET_ERROR)
   {
       return false;
   }
   QSharedPointer<CClientSession> pclient(new CClientSession(SocketClient));
   pclient->m_addr = RecvAddr;
   m_Clients.insert(SocketClient, pclient);

   QObject::connect(pclient->m_pRecvThread.data(),
       SIGNAL(sig_CreateDialog(int, int)),
       g_pWindow,
       SLOT(slot_CreateDialog(int, int)));

   QObject::connect(pclient->m_pRecvThread.data(),
       SIGNAL(sig_RecvData(int, int, QByteArray)),
       g_pWindow,
       SLOT(slot_RecvData(int, int, QByteArray)));

   emit sendClinentInfo(pclient.data());
   pclient->m_pRecvThread->start();
   return true;
}

void SeverScoket::Send(SOCKET s, QString str)
{

}
QString SeverScoket::Recv(SOCKET s)
{
    return "ss";
}

void SeverScoket::Close(SOCKET s)
{
 
    closesocket(s);
    m_Clients.remove(s);
}

QSharedPointer<CClientSession>  SeverScoket::GetClient(SOCKET s)
{
    return  m_Clients.find(s).value();
}