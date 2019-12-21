#pragma once

#include <windows.h>
#include <Psapi.h>
#include <Zydis/Zydis.h>
#include <inttypes.h>
#include "AnalyFile.h"

#pragma  comment(lib, "Psapi.lib")
#pragma  comment(lib, "Zydis.lib")
class CDisAssmble
{
public:
	CDisAssmble(char* str, DWORD dwPid);
	~CDisAssmble();
	//��ʾ���е�
	void disassembleBuffer(ZydisDecoder* decoder, ZydisU8* data, ZydisUSize length,
		ZydisBool installHooks, ZydisU64 instruct);

	//��ʾһ�е�
	char* disassembleBuffer1(ZydisDecoder* decoder, ZydisU8* data, ZydisUSize length,
		ZydisBool installHooks, ZydisU64 instruct);

	//Ѱ��callָ��

	DWORD disassembleBufferCall(ZydisDecoder* decoder, ZydisU8* data, ZydisUSize length,
		ZydisBool installHooks, ZydisU64 instruct);

	//Ѱ��jmpָ��
	DWORD FindJmp(LPVOID lpAddress);
	DWORD disassembleBufferJMP(ZydisDecoder* decoder, ZydisU8* data, ZydisUSize length,
		ZydisBool installHooks, ZydisU64 instruct);

	//Ѱ��callָ���ַcall ds:[0x000000]
	DWORD FindCallAddress(LPVOID lpAddress);

	//������ɫ
	void SetColor(char* buffer, int OpencodeLength);

private:
	char* mFilepath;
	CAnalyFile* pFile;
	DWORD dwProcessId;
};

