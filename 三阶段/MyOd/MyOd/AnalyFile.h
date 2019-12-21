#pragma once
#include "ShowErro.h"
#include <windows.h>
#include <map>
#include <Tlhelp32.h>

class CAnalyFile
{
public:
	CAnalyFile(char* str,DWORD dwPid);
	~CAnalyFile();

	void ReadFile();
	DWORD RVA2FA(DWORD nRva);
	char* FindFunName(LPVOID lpAddress);
	BOOL GetProcessModule();
	LPVOID FindMoudelAddress(char* str);
private:
	char* mFilePath;
	std::map<LPVOID, char*> mFunName;
	std::map<char*, LPVOID> mModel;
	CShowErro* pShowErro;
	PIMAGE_NT_HEADERS32 pNt;
	PIMAGE_SECTION_HEADER mpSection;
	DWORD mPid;

};

