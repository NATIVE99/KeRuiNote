// MyOd.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include  "Control.h"


int main(int argv,char* argc[])
{
	CControl *pCtrl=new CControl;
	CShowErro* pShowErro=NULL;
	//判断是否有调试路径参数
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

