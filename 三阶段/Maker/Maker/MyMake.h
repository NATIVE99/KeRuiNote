#pragma once
#include <string>
#include <windows.h>
#include <iostream>
#include <compressapi.h>
#pragma comment(lib, "Cabinet.lib")
class CMyMake
{
public:
	CMyMake();
	~CMyMake();
public:
	bool Make(std::string strSrcPath, std::string strDstPath);
	//解析PE格式
private:
	bool AnalyzePE(std::string strSrcPath);
	PIMAGE_DOS_HEADER m_pDosHdr = nullptr;
	PIMAGE_NT_HEADERS m_pNtHdr = nullptr;
	PIMAGE_SECTION_HEADER m_pSecHdrs = nullptr;

private: 
	struct CompressDataInfo
	{
		int m_nSrcSize = 0; //压缩前数据的大小
		int m_nComSize = 0; //压缩数据后的大小
		char* m_pComData = nullptr; //压缩后的数据
	};
	CompressDataInfo* m_pCompressDatas = nullptr; //压缩后的节数据
	bool CompressSectionsData();

	//解压缩代码
private:
	bool GetDecomcode();
	char* m_pDecompressCodeBuff = nullptr;
	int m_nDecompressCodeSize = 0;

private:
	bool GetDeCodeAndDataSec(); //构建解压缩代码和压缩数据节
	char* m_pDeCodeAndDataSec = nullptr;
	int m_nsizeOfDeCodeAndDataSec = 0;

//private:
//	bool GetCompressDataSec(); //构建压缩数据节
//	char* m_pCompressSecData = nullptr;
//	int m_nSizeOfCompressSec = 0;

private:
	bool GetNewSectionHeaders(); //构建节表
	PIMAGE_SECTION_HEADER m_pNewSecHdrs = nullptr;
	int m_nNumberOfNewSecs = 0;

private:
	bool GetNewPeHdr();
	char* m_pNewPeHdr = nullptr;
	int m_nSizeOfNewPeHdr = 0;

private:
	bool WriteNewPeFile(std::string strDstPath);

private:
int GetAlinValue(int nValue, int nAlign);
};

