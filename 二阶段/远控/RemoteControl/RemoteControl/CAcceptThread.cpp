#include "CAcceptThread.h"


CAcceptThread::CAcceptThread(SeverScoket& socket):m_socket(socket)
{

}
CAcceptThread::~CAcceptThread()
{

}


void CAcceptThread::run()
{
    while(1)
    {
       bool bRet=m_socket.Accept();
       if(!bRet)
       {
           break;
       }    
    }
}
