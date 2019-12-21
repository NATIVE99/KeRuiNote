#include "stdafx.h"
#include "AnalyFile.h"



CAnalyFile::CAnalyFile(char* str, DWORD dwPid)
{
	mFilePath = str;
	pShowErro = new CShowErro;
	mPid = dwPid;
}

CAnalyFile::~CAnalyFile()
{
	
}

void CAnalyFile::ReadFile()
{

	GetProcessModule();
	HANDLE hFile;

	hFile = CreateFileA(mFilePath,           // open MYFILE.TXT 
			GENERIC_READ,              // open for reading 
			FILE_SHARE_READ,           // share for reading 
			NULL,                      // no security 
			OPEN_EXISTING,             // existing file only 
			FILE_ATTRIBUTE_NORMAL,     // normal file 
			NULL);                     // no attr. template 
	if (hFile == INVALID_HANDLE_VALUE)
	{
		pShowErro->ShowErroApi("CAnalyFile::ReadFile:CreateFileMapping");
	}

	HANDLE hMapFile;

	hMapFile = CreateFileMapping(hFile,    // Current file handle. 
		NULL,                              // Default security. 
		PAGE_READONLY,                    // Read/write permission. 
		0,                                 // Max. object size. 
		0,                                 // Size of hFile. 
		"MyFileMappingObject");            // Name of mapping object. 

	if (hMapFile == NULL)
	{
		pShowErro->ShowErroApi("CAnalyFile::ReadFile:CreateFileMapping");
	}

	LPVOID lpMapAddress;
	lpMapAddress = MapViewOfFile(hMapFile, // Handle to mapping object. 
		FILE_MAP_READ,               // Read/write permission 
		0,                                 // Max. object size. 
		0,                                 // Size of hFile. 
		0);                                // Map entire file. 

	if (lpMapAddress == NULL)
	{
		pShowErro->ShowErroApi("CAnalyFile::ReadFile:MapViewOfFile");
	}

	PIMAGE_DOS_HEADER pDos= (PIMAGE_DOS_HEADER)lpMapAddress;

	 pNt = (PIMAGE_NT_HEADERS32)(pDos->e_lfanew + (DWORD)lpMapAddress);
	 mpSection = (PIMAGE_SECTION_HEADER)(sizeof(IMAGE_FILE_HEADER)+(DWORD)pNt+pNt->FileHeader.SizeOfOptionalHeader+4);

	IMAGE_OPTIONAL_HEADER32 OpFile = pNt->OptionalHeader;
	LPVOID lpImport=(LPVOID)OpFile.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress;
	lpImport =(LPVOID)(RVA2FA((DWORD)lpImport)+ (DWORD)lpMapAddress);
	//lpImport = (LPVOID)((DWORD)lpImport + (DWORD)lpMapAddress);

	PIMAGE_IMPORT_DESCRIPTOR pImport = (PIMAGE_IMPORT_DESCRIPTOR)lpImport;
	char* szDllNmae;
	char szbuf[32] ={0};
	while(pImport->OriginalFirstThunk!=NULL)
	{
		//szDllNmae = (char*)(pImport->Name + (DWORD)lpMapAddress);

		szDllNmae = (char*)RVA2FA(pImport->Name) + (DWORD)lpMapAddress;
		memcpy(szbuf, szDllNmae, 32);
		if (pImport->OriginalFirstThunk != NULL)
		{
			LPVOID lpThunk32 = (LPVOID)(RVA2FA(pImport->OriginalFirstThunk) + (DWORD)lpMapAddress);
			PIMAGE_THUNK_DATA32 pThunk32=(PIMAGE_THUNK_DATA32)lpThunk32;
			//这里获取本进程的，所以有错误
			HMODULE hModule = GetModuleHandle(szDllNmae);
			if(hModule==NULL)
			{
				hModule=LoadLibrary(szDllNmae);
			}
			_strlwr(szbuf);
			HMODULE hModuleEx=(HMODULE)FindMoudelAddress(szbuf);
			while(pThunk32->u1.AddressOfData!=NULL)
			{
				if(!IMAGE_SNAP_BY_ORDINAL32(pThunk32->u1.Ordinal))
				{					
					PIMAGE_IMPORT_BY_NAME pName = (PIMAGE_IMPORT_BY_NAME)(RVA2FA(pThunk32->u1.AddressOfData) + (DWORD)lpMapAddress);
					LPVOID lpFunAddress=GetProcAddress(hModule, pName->Name);
					DWORD dwOffset = (DWORD)lpFunAddress - (DWORD)hModule;
					DWORD dwFunAddressEx = (DWORD)dwOffset + (DWORD)hModuleEx;
					mFunName.insert(std::pair < LPVOID, char*>((LPVOID)dwFunAddressEx, pName->Name));
				}

				pThunk32 = pThunk32 + 1;
			}
		}
		//
		pImport = pImport + 1;
	}
	

}

DWORD CAnalyFile::RVA2FA(DWORD nRva)
{

	int nPosSe = -1;
	//判断在哪个节里面
	for (int i = 0; i < pNt->FileHeader.NumberOfSections; i++)
	{
		int nStart = (int)(mpSection + i)->VirtualAddress;
		int nEnd = (int)((mpSection + i)->VirtualAddress + (int)((mpSection + i)->Misc.VirtualSize));
		if (nRva >= nStart&& nRva <= nEnd)
		{
			nPosSe = i;
			break;
		}
	}
	DWORD FOA = 0;
	if (nPosSe != -1)
	{
		//减去ImageBase减去VirtualOffset得到RVA相对节的偏移再加上文件的节的偏移得到FOA
		FOA = nRva - (mpSection + nPosSe)->VirtualAddress + (mpSection + nPosSe)->PointerToRawData;
	}

	return FOA;

}

char* CAnalyFile::FindFunName(LPVOID lpAddress)
{
	for(auto i:mFunName)
	{
		if(i.first== lpAddress)
		{
			return i.second;
		}
	}

	return NULL;
}

BOOL CAnalyFile::GetProcessModule()
{


	HANDLE        hModuleSnap = NULL;
	MODULEENTRY32 me32 = { 0 };

	hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, mPid);
	if (hModuleSnap == INVALID_HANDLE_VALUE)
		return (FALSE);

	me32.dwSize = sizeof(MODULEENTRY32);

	if (Module32First(hModuleSnap, &me32))
	{
		do
		{
			char* pName = new char[24];
	
			memcpy(pName, me32.szModule, 24);
			_strlwr(pName);
			mModel.insert(std::pair <char*, LPVOID>(pName, me32.modBaseAddr));

		} while (Module32Next(hModuleSnap, &me32));

	}

	CloseHandle(hModuleSnap);

	return true;


}

LPVOID CAnalyFile::FindMoudelAddress(char* str)
{
	for(auto i:mModel)
	{
		int n = strcmp(str, i.first);
		if (n == 0)
		{
			return i.second;
			break;
		}
	}

	return 0;
}
