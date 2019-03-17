#include "Session.h"

Session::Session(SOCKET s, sockaddr_in addr)
{
    m_sock = s;
    m_addr = addr;
    pScreen = NULL;
    pCmd = NULL;
    pProcess = NULL;
    pFile = NULL;
}


Session::~Session()
{
}
