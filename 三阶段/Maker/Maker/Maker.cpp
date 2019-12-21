// Maker.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "MyMake.h"

int main(int argc,char** argv)
{
	if(argc>1)
	{
		CMyMake* pCMyMake = new CMyMake;
		std::string strSrcPath = argv[1];

		int nIndex= strSrcPath.rfind('.');
		std::string strTmp = strSrcPath.substr(0, nIndex);
		std::string strDstPath = strTmp + "_pack.exe";

		pCMyMake->Make(strSrcPath, strDstPath);
	}
	else
	{
		std::cout << "请输入文件路径" << std::endl;
	}

	
    return 0;
}