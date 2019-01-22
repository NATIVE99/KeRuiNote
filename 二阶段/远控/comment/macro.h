#pragma once
#include <windows.h>

enum CmdType
{
    //server cmd ����
    SERVER_CMD = 1,
    SERVER_CMD_DATA,

    //client cmd ����
    CLIENT_CMD_REPLY,
    CLIENT_CMD_DATA,

};

enum DIALOG_TYPE {
    CMD_CREATE,
    SCREEN_CREATE

};

#pragma pack(push, 1)
struct tagPacket {
    unsigned char code;
    size_t length; //����ĸ������ݳ���
};
#pragma pack(pop)

inline bool RecvData(SOCKET s, char* pBuf, int nLength)
{

    int nCurSize = 0;
    int nTotalSize = 0;

    while(nTotalSize<nLength)
    {
        
        nCurSize=recv(s, &pBuf[nCurSize], nLength - nTotalSize, 0);
        if (nCurSize == 0 || nCurSize == SOCKET_ERROR) {
            //tcp���ӹر��ˣ����ŶϿ�
            return false;
        }
        else {
            nTotalSize += nCurSize;
        }
    }
}

//����ָ�������ݣ�����װ��packet
inline bool SendData(SOCKET s, int nType, const char* pBuf, int nLength)
{
    char* pSendBuf = new char[sizeof(tagPacket) + nLength];
    if (pSendBuf == NULL) {
        return false;
    }


    tagPacket* pkt = (tagPacket*)pSendBuf;
    pkt->code = nType;
    pkt->length = nLength;

    if (nLength > 0) {

        memcpy(pSendBuf + sizeof(tagPacket), pBuf, nLength);
    }

    //��Ҫѭ���ķ���
    send(s,
        (char*)pSendBuf,
        sizeof(tagPacket) + pkt->length,
        0);

    if (pSendBuf != NULL) {
        delete[] pSendBuf;
        pSendBuf = NULL;
    }

    return true;
}