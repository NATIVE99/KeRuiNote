// MyOd.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include  "Control.h"


int main(int argv,char* argc[])
{
	CControl *pCtrl=new CControl;
	CShowErro* pShowErro=NULL;
	//�ж��Ƿ��е���·������
	if(argv>1)
	{
		pCtrl->Start(argc[1]);
	
	}
	else
	{
		pShowErro = new CShowErro;
		pShowErro->ShowErroArg();
	}
    return 0;
}

