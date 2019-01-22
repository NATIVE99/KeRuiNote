#include "CRecvThread.h"

CRecvThread::CRecvThread(SOCKET s):m_RecvSocket(s)
  
{
}

CRecvThread::~CRecvThread()
{
}
void CRecvThread::slot_SendData(QString strInfo)
{
    int nRet = 0;

    nRet = send(m_RecvSocket,
        strInfo.toLocal8Bit().toStdString().c_str(),
        strInfo.toLocal8Bit().toStdString().length(),
        0);
}
void CRecvThread::run()
{

    tagPacket pkt;
    char* pData = NULL;

    while (1)
    {
        //收取包头
        bool bRet = RecvData(m_RecvSocket,
            (char*)&pkt,
            sizeof(tagPacket));
        if (!bRet)
        {
            break;
        }
        if (pkt.length > 0)
        {
            //收取包的附加数据
            pData = new char[pkt.length];
            bool bRet = RecvData(m_RecvSocket,
                (char*)pData,
                pkt.length);
            if (!bRet)
            {
                return;
            }

        }

        //表示已经成功的收到了一个命令包，开始解包分析包
        switch (pkt.code)
        {
        case CLIENT_CMD_REPLY:
        {
            //通知界面创建Dialog
            emit sig_CreateDialog(m_RecvSocket, CMD_CREATE);
        }
        break;

        case CLIENT_CMD_DATA:
        {
            //显示数据
            emit sig_RecvData(m_RecvSocket, CMD_CREATE, QByteArray(pData, pkt.length));
        }
        break;
        }
        //RecvData(m_RecvSocket,)
        if (pData != NULL) {
            delete[] pData;
            pData = NULL;
        }
    }

}