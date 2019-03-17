//封包
#pragma once
#include <windows.h>


#define CUT_WINDOWS 1
#define SEND_WINDOWS 2
#pragma pack(push,1)
struct tagHead
{
	BYTE bType;
    size_t nDateSize;//压缩后的大小
    size_t nDate;//
    char  data[];
};
#pragma pack(pop)

enum IOTYPE {
    IO_READ=0,
    IO_WRITE,
    IO_CLOSE,
    IO_ACCEPT,
};

struct MyOverlapped {
    MyOverlapped(IOTYPE type) {
        nType = type;
        memset(&ol, 0, sizeof(OVERLAPPED));
    }
    OVERLAPPED ol;
    IOTYPE nType;
    char pRecvBuf[8192];
};

//发送指定的数据，并封装成packet
inline bool SendData(SOCKET s, int nType, const char* pBuf, int nLength)
{
    char* pSendBuf = new char[sizeof(tagHead) + nLength];
    if (pSendBuf == NULL) {
        return false;
    }


    tagHead* pkt = (tagHead*)pSendBuf;
    pkt->bType = nType;
    pkt->nDateSize = nLength;

    if (nLength > 0) {

        memcpy(pSendBuf + sizeof(tagHead), pBuf, nLength);
    }

    //需要循环的发送
    send(s,
        (char*)pSendBuf,
        sizeof(tagHead) + pkt->nDateSize,
        0);

    if (pSendBuf != NULL) {
        delete[] pSendBuf;
        pSendBuf = NULL;
    }

    return true;
}