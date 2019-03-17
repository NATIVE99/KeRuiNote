// CutClient.cpp : 定义控制台应用程序的入口点。
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


