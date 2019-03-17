//封包
#pragma once

#include <windows.h>
#include "zlib.h"
#include "zconf.h"
#pragma comment(lib,"zlibstat.lib")
//显示桌面
#define CUT_WINDOWS 1
#define SEND_WINDOWS 2
//显示cmd
#define CUT_CMD 3
#define SEND_CMD 4
//显示进程
#define CUT_PROCESS 5
#define SEND_PROCESS 6

//结束进程
#define CUT_END 7
#define SEND_END 8
//显示文件路径
#define CUT_FILE 9
#define SEND_FILE 10
//下载文件
#define CUT_DOWNLOAD 11
#define SEND_DOWNLOAD 12
//心跳包
#define CUT_HEART 13
#define SEND_HEART 14

#pragma pack(push,1)
struct tagHead
{
	BYTE bType;
    size_t nDateSize;//压缩后的大小
    size_t nDate;//原本大小
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
    uLong blen = compressBound(nLength);
    char* pSendBuf = NULL;
    if (blen>nLength)
    {
         pSendBuf = new char[sizeof(tagHead) + blen];
    }
    else
    {
         pSendBuf = new char[sizeof(tagHead) + nLength];
    }
  
    if (pSendBuf == NULL) {
        return false;
    }


    tagHead* pkt = (tagHead*)pSendBuf;
    pkt->bType = nType;
    pkt->nDate = nLength;

    if (nLength > 0) {
        //压缩后的大小
        
        char* buf = new char[blen];
        //压缩
        compress((Bytef *)buf, &blen, (Bytef *)pBuf, nLength);
        pkt->nDateSize = blen;
        memcpy(pSendBuf + sizeof(tagHead), buf, blen);
    }
    else
    {
        pkt->nDateSize = 0;
    }

    //需要循环的发送
    send(s,
        (char*)pSendBuf,
        pkt->nDateSize + sizeof(tagHead),
        0);

    if (pSendBuf != NULL) {
        delete[] pSendBuf;
        pSendBuf = NULL;
    }

    return true;
}

//发送指定的数据，并封装成packet
inline bool SendData2(SOCKET s, int nType, const char* pBuf, int nLength)
{

  
        char* pSendBuf = new char[sizeof(tagHead) + nLength];
        if (pSendBuf == NULL) {
            return false;
        }


        tagHead* pkt = (tagHead*)pSendBuf;
        pkt->bType = nType;
        pkt->nDate = nLength;

        if (nLength > 0) {

            memcpy(pSendBuf + sizeof(tagHead), pBuf, nLength);
        }

        //需要循环的发送
        send(s,
            (char*)pSendBuf,
            sizeof(tagHead) + pkt->nDate,
            0);

        if (pSendBuf != NULL) {
            delete[] pSendBuf;
            pSendBuf = NULL;
        }

        return true;
    
}