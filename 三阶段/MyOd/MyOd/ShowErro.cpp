#include "stdafx.h"
#include "ShowErro.h"


CShowErro::CShowErro()
{
}


CShowErro::~CShowErro()
{
}

void CShowErro::ShowErroApi(const char * pre)
{

	//�쳣����  ϵͳ
		LPVOID lpMsgBuf;
		FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER |
			FORMAT_MESSAGE_FROM_SYSTEM |
			FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			GetLastError(),
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
			(LPTSTR)&lpMsgBuf,
			0,
			NULL
		);

		printf("%s:%s", pre, lpMsgBuf);


		LocalFree(lpMsgBuf);

}

void CShowErro::ShowErroArg()
{
	printf("�봫��Ҫ���ԵĲ���\r\n");
}
