//���
#pragma once

#include <windows.h>
#include "zlib.h"
#include "zconf.h"
#pragma comment(lib,"zlibstat.lib")
//��ʾ����
#define CUT_WINDOWS 1
#define SEND_WINDOWS 2
//��ʾcmd
#define CUT_CMD 3
#define SEND_CMD 4
//��ʾ����
#define CUT_PROCESS 5
#define SEND_PROCESS 6

//��������
#define CUT_END 7
#define SEND_END 8
//��ʾ�ļ�·��
#define CUT_FILE 9
#define SEND_FILE 10
//�����ļ�
#define CUT_DOWNLOAD 11
#define SEND_DOWNLOAD 12
//������
#define CUT_HEART 13
#define SEND_HEART 14

#pragma pack(push,1)
struct tagHead
{
	BYTE bType;
    size_t nDateSize;//ѹ����Ĵ�С
    size_t nDate;//ԭ����С
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

//����ָ�������ݣ�����װ��packet
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
        //ѹ����Ĵ�С
        
        char* buf = new char[blen];
        //ѹ��
        compress((Bytef *)buf, &blen, (Bytef *)pBuf, nLength);
        pkt->nDateSize = blen;
        memcpy(pSendBuf + sizeof(tagHead), buf, blen);
    }
    else
    {
        pkt->nDateSize = 0;
    }

    //��Ҫѭ���ķ���
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

//����ָ�������ݣ�����װ��packet
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

        //��Ҫѭ���ķ���
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