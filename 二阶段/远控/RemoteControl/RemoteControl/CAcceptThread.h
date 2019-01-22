#pragma once
#include <Qthread>
#include "SeverScoket.h"
class CAcceptThread : public QThread
{
    Q_OBJECT

public:
    CAcceptThread(SeverScoket& socket);
    ~CAcceptThread();
public:
    
    void run();
    public slots:

private:
    SeverScoket& m_socket;
};
