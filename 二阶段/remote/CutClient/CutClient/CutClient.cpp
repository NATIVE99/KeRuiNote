// CutClient.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"

#include "MySocket.h"


int _tmain(int argc, _TCHAR* argv[])
{
    MySocket mysocket;
    mysocket.Start();
    mysocket.Connect();
	return 0;
}


