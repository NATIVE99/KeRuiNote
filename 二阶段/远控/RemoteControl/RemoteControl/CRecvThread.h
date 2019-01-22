#pragma once

#include <QThread>
#include <windows.h>
#include "../../comment/macro.h"
class CRecvThread : public QThread
{
    Q_OBJECT

public:
    CRecvThread(SOCKET s);
    ~CRecvThread();
signals:
  
    public slots:
    void slot_SendData(QString strInfo);
    void slot_CreateDialog(int s, int nDialogType);

public:
    void run();
private:
    SOCKET m_RecvSocket;
};
