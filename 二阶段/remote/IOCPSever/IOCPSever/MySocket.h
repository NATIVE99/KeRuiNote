#pragma once
#include <Winsock2.h>
#include "Session.h"
#include <map>
#include <thread>
#include <sstream>
#include <QObject>
#include <QTime>
#pragma comment(lib,"Ws2_32.lib")
class MySocket :public QObject
{
    Q_OBJECT
public:
    MySocket();
    ~MySocket();
    void Start();
    void BindAndListen();
    SOCKET Accept();
    bool Recv(SOCKET s, char szbuf[], int nLenth);
    void Send(SOCKET s, char szbuf[], int nLength);

    bool RecvData(SOCKET s, char* pBuf, int nLength);

    void ModleIocp();
    void PostRecv(SOCKET sClient);

    Session* GetSession(SOCKET s);
    void Heartbeat();
    void UpDateTime(Session* ps);
public:
    SOCKET m_sSever;
    HANDLE m_hPort;
    std::map<SOCKET, Session*> m_client;

signals:
    void sig_sendDownload(QByteArray arr);
    void sig_sendFile(QByteArray arr);
    void sig_sendProcess(QByteArray arr);
    void sig_deleteProcess(QByteArray arr);
    void sig_sendCmd(QByteArray arr);
    void sig_sendWindows(QByteArray arr);
    void sig_sendsession(Session* ps);
    void sig_sendUpate(Session* ps);
public slots:


};

