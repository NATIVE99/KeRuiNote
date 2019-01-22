#pragma once
#include <QObject>
#define _WINSOCK_DEPRECATED_NO_WARNINGS 
#include <Winsock2.h>
#include "cmddialog.h"
#include <QSharedPointer>
#include <qmap.h>

#include "CRecvThread.h"

#pragma comment(lib, "ws2_32.lib")
struct CClientSession
{
    CClientSession(SOCKET s) {
        m_s = s;
        m_pRecvThread.reset(new CRecvThread(s));
    }


    SOCKET m_s;
    sockaddr_in m_addr;
    QSharedPointer<CRecvThread> m_pRecvThread;
    //dlg
    CmdDialog* m_pCmdDlg;


};
class SeverScoket : public QObject
{
    Q_OBJECT

public:
    SeverScoket(QObject *parent=0);
    ~SeverScoket();
public:
    void StartUp();
    void CreateSocketAndBind(int Port = 10086);
    bool Accept();

    void Send(SOCKET s, QString str);
    QString Recv(SOCKET s);

    void Close(SOCKET s);
    QSharedPointer<CClientSession> GetClient(SOCKET s);
signals:
    void sendClinentInfo(CClientSession* pCs);
private:
    SOCKET m_SeverSocket;
    sockaddr_in m_SeverAddr;
    QMap<SOCKET, QSharedPointer<CClientSession>> m_Clients;
};
