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
	//显示多行的
	void disassembleBuffer(ZydisDecoder* decoder, ZydisU8* data, ZydisUSize length,
		ZydisBool installHooks, ZydisU64 instruct);

	//显示一行的
	char* disassembleBuffer1(ZydisDecoder* decoder, ZydisU8* data, ZydisUSize length,
		ZydisBool installHooks, ZydisU64 instruct);

	//寻找call指令

	DWORD disassembleBufferCall(ZydisDecoder* decoder, ZydisU8* data, ZydisUSize length,
		ZydisBool installHooks, ZydisU64 instruct);

	//寻找jmp指令
	DWORD FindJmp(LPVOID lpAddress);
	DWORD disassembleBufferJMP(ZydisDecoder* decoder, ZydisU8* data, ZydisUSize length,
		ZydisBool installHooks, ZydisU64 instruct);

	//寻找call指令地址call ds:[0x000000]
	DWORD FindCallAddress(LPVOID lpAddress);

	//设置颜色
	void SetColor(char* buffer, int OpencodeLength);

private:
	char* mFilepath;
	CAnalyFile* pFile;
	DWORD dwProcessId;
};

