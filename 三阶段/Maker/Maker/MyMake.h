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
	//����PE��ʽ
private:
	bool AnalyzePE(std::string strSrcPath);
	PIMAGE_DOS_HEADER m_pDosHdr = nullptr;
	PIMAGE_NT_HEADERS m_pNtHdr = nullptr;
	PIMAGE_SECTION_HEADER m_pSecHdrs = nullptr;

private: 
	struct CompressDataInfo
	{
		int m_nSrcSize = 0; //ѹ��ǰ���ݵĴ�С
		int m_nComSize = 0; //ѹ�����ݺ�Ĵ�С
		char* m_pComData = nullptr; //ѹ���������
	};
	CompressDataInfo* m_pCompressDatas = nullptr; //ѹ����Ľ�����
	bool CompressSectionsData();

	//��ѹ������
private:
	bool GetDecomcode();
	char* m_pDecompressCodeBuff = nullptr;
	int m_nDecompressCodeSize = 0;

private:
	bool GetDeCodeAndDataSec(); //������ѹ�������ѹ�����ݽ�
	char* m_pDeCodeAndDataSec = nullptr;
	int m_nsizeOfDeCodeAndDataSec = 0;

//private:
//	bool GetCompressDataSec(); //����ѹ�����ݽ�
//	char* m_pCompressSecData = nullptr;
//	int m_nSizeOfCompressSec = 0;

private:
	bool GetNewSectionHeaders(); //�����ڱ�
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

