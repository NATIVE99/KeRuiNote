#include "stdafx.h"
#include "DisAssmble.h"


CDisAssmble::CDisAssmble(char* str,DWORD dwPid)
{
	mFilepath = str;
	pFile = new CAnalyFile(mFilepath,dwPid);
	pFile->ReadFile();
	dwProcessId = dwPid;
}


CDisAssmble::~CDisAssmble()
{
}

ZYDIS_INLINE ZydisStatus ZydisStringAppendFormatC(ZydisString* string, const char* format, ...)
{
	if (!string || !string->buffer || !format)
	{
		return ZYDIS_STATUS_INVALID_PARAMETER;
	}

	va_list arglist;
	va_start(arglist, format);
	const int w = vsnprintf(string->buffer + string->length, string->capacity - string->length,
		format, arglist);
	if ((w < 0) || ((size_t)w > string->capacity - string->length))
	{
		va_end(arglist);
		return ZYDIS_STATUS_INSUFFICIENT_BUFFER_SIZE;
	}
	string->length += w;
	va_end(arglist);
	return ZYDIS_STATUS_SUCCESS;
}

/* ============================================================================================== */
/* Static data                                                                                    */
/* ============================================================================================== */

/**
* @brief   Static array with the condition-code strings.
*/
static const char* conditionCodeStrings[0x20] =
{
	/*00*/ "eq",
	/*01*/ "lt",
	/*02*/ "le",
	/*03*/ "unord",
	/*04*/ "neq",
	/*05*/ "nlt",
	/*06*/ "nle",
	/*07*/ "ord",
	/*08*/ "eq_uq",
	/*09*/ "nge",
	/*0A*/ "ngt",
	/*0B*/ "false",
	/*0C*/ "oq",
	/*0D*/ "ge",
	/*0E*/ "gt",
	/*0F*/ "true",
	/*10*/ "eq_os",
	/*11*/ "lt_oq",
	/*12*/ "le_oq",
	/*13*/ "unord_s",
	/*14*/ "neq_us",
	/*15*/ "nlt_uq",
	/*16*/ "nle_uq",
	/*17*/ "ord_s",
	/*18*/ "eq_us",
	/*19*/ "nge_uq",
	/*1A*/ "ngt_uq",
	/*1B*/ "false_os",
	/*1C*/ "neq_os",
	/*1D*/ "ge_oq",
	/*1E*/ "gt_oq",
	/*1F*/ "true_us"
};

/* ============================================================================================== */
/* Enums and Types                                                                                */
/* ============================================================================================== */

/**
* @brief   Custom user data struct.
*/
typedef struct ZydisCustomUserData_
{
	ZydisBool ommitImmediate;
} ZydisCustomUserData;

/* ============================================================================================== */
/* Hook callbacks                                                                                 */
/* ============================================================================================== */

ZydisFormatterFunc defaultPrintMnemonic;

static ZydisStatus ZydisFormatterPrintMnemonic(const ZydisFormatter* formatter,
	ZydisString* string, const ZydisDecodedInstruction* instruction, ZydisCustomUserData* userData)
{
	// We use the user-data to pass data to the @c ZydisFormatterFormatOperandImm function
	userData->ommitImmediate = ZYDIS_TRUE;

	// Rewrite the instruction-mnemonic for the given instructions
	if (instruction->operandCount &&
		instruction->operands[instruction->operandCount - 1].type == ZYDIS_OPERAND_TYPE_IMMEDIATE)
	{
		const ZydisU8 conditionCode =
			(ZydisU8)instruction->operands[instruction->operandCount - 1].imm.value.u;
		switch (instruction->mnemonic)
		{
		case ZYDIS_MNEMONIC_CMPPS:
			if (conditionCode < 0x08)
			{
				return ZydisStringAppendFormatC(
					string, "cmp%sps", conditionCodeStrings[conditionCode]);
			}
			break;
		case ZYDIS_MNEMONIC_CMPPD:
			if (conditionCode < 0x08)
			{
				return ZydisStringAppendFormatC(
					string, "cmp%spd", conditionCodeStrings[conditionCode]);
			}
			break;
		case ZYDIS_MNEMONIC_VCMPPS:
			if (conditionCode < 0x20)
			{
				return ZydisStringAppendFormatC(
					string, "vcmp%sps", conditionCodeStrings[conditionCode]);
			}
			break;
		case ZYDIS_MNEMONIC_VCMPPD:
			if (conditionCode < 0x20)
			{
				return ZydisStringAppendFormatC(
					string, "vcmp%spd", conditionCodeStrings[conditionCode]);
			}
			break;
		default:
			break;
		}
	}

	// We did not rewrite the instruction-mnemonic. Signal the @c ZydisFormatterFormatOperandImm
	// function not to omit the operand
	userData->ommitImmediate = ZYDIS_FALSE;

	// Default mnemonic printing
	return defaultPrintMnemonic(formatter, string, instruction, userData);
}

/* ---------------------------------------------------------------------------------------------- */

ZydisFormatterOperandFunc defaultFormatOperandImm;

static ZydisStatus ZydisFormatterFormatOperandImm(const ZydisFormatter* formatter,
	ZydisString* string, const ZydisDecodedInstruction* instruction,
	const ZydisDecodedOperand* operand, ZydisCustomUserData* userData)
{
	// The @c ZydisFormatterFormatMnemonic sinals us to omit the immediate (condition-code)
	// operand, because it got replaced by the alias-mnemonic
	if (userData->ommitImmediate)
	{
		return ZYDIS_STATUS_SKIP_OPERAND;
	}

	// Default immediate formatting
	return defaultFormatOperandImm(formatter, string, instruction, operand, userData);
}


void CDisAssmble::disassembleBuffer(ZydisDecoder* decoder, ZydisU8* data, ZydisUSize length,
	ZydisBool installHooks, ZydisU64 instruct)
{
	ZydisFormatter formatter;
	ZydisFormatterInit(&formatter, ZYDIS_FORMATTER_STYLE_INTEL);
	ZydisFormatterSetProperty(&formatter, ZYDIS_FORMATTER_PROP_FORCE_MEMSEG, ZYDIS_TRUE);
	ZydisFormatterSetProperty(&formatter, ZYDIS_FORMATTER_PROP_FORCE_MEMSIZE, ZYDIS_TRUE);

	if (installHooks)
	{
		defaultPrintMnemonic = (ZydisFormatterFunc)&ZydisFormatterPrintMnemonic;
		ZydisFormatterSetHook(&formatter, ZYDIS_FORMATTER_HOOK_PRINT_MNEMONIC,
			(const void**)&defaultPrintMnemonic);
		defaultFormatOperandImm = (ZydisFormatterOperandFunc)&ZydisFormatterFormatOperandImm;
		ZydisFormatterSetHook(&formatter, ZYDIS_FORMATTER_HOOK_FORMAT_OPERAND_IMM,
			(const void**)&defaultFormatOperandImm);
	}

	ZydisU64 instructionPointer = instruct;

	ZydisDecodedInstruction instruction;
	ZydisCustomUserData userData;
	char buffer[256];
	while (ZYDIS_SUCCESS(
		ZydisDecoderDecodeBuffer(decoder, data, length, instructionPointer, &instruction)))
	{
	
		
		printf("%08X    ", instruction.instrAddress);
		//打印机器码
		int i = 0;
		for(i;i<instruction.length;i++)
		{
			printf("%02X ", *(data+i));
		}
		
		//对齐
		for(i;i<8;i++)
		{
			printf("   ");
		}
		length -= instruction.length;
		instructionPointer += instruction.length;
		data += instruction.length;
		ZydisFormatterFormatInstructionEx(
			&formatter, &instruction, &buffer[0], sizeof(buffer), &userData);

		SetColor(buffer, instruction.length);
		
	}
}


char* CDisAssmble::disassembleBuffer1(ZydisDecoder* decoder, ZydisU8* data, ZydisUSize length,
	ZydisBool installHooks, ZydisU64 instruct)
{
	ZydisFormatter formatter;
	ZydisFormatterInit(&formatter, ZYDIS_FORMATTER_STYLE_INTEL);
	ZydisFormatterSetProperty(&formatter, ZYDIS_FORMATTER_PROP_FORCE_MEMSEG, ZYDIS_TRUE);
	ZydisFormatterSetProperty(&formatter, ZYDIS_FORMATTER_PROP_FORCE_MEMSIZE, ZYDIS_TRUE);

	if (installHooks)
	{
		defaultPrintMnemonic = (ZydisFormatterFunc)&ZydisFormatterPrintMnemonic;
		ZydisFormatterSetHook(&formatter, ZYDIS_FORMATTER_HOOK_PRINT_MNEMONIC,
			(const void**)&defaultPrintMnemonic);
		defaultFormatOperandImm = (ZydisFormatterOperandFunc)&ZydisFormatterFormatOperandImm;
		ZydisFormatterSetHook(&formatter, ZYDIS_FORMATTER_HOOK_FORMAT_OPERAND_IMM,
			(const void**)&defaultFormatOperandImm);
	}

	ZydisU64 instructionPointer = instruct;

	ZydisDecodedInstruction instruction;
	ZydisCustomUserData userData;
	char buffer[256];
	
	ZydisDecoderDecodeBuffer(decoder, data, length, instructionPointer, &instruction);
	
		
		printf("%08X    ", instruction.instrAddress);
		//打印机器码
		int i = 0;
		for (i; i < instruction.length; i++)
		{
			printf("%02X ", *(data + i));
		}
		//对齐
		for (i; i < 8; i++)
		{
			printf("   ");
		}

		data += instruction.length;
		length -= instruction.length;
		instructionPointer += instruction.length;

		ZydisFormatterFormatInstructionEx(
			&formatter, &instruction, &buffer[0], sizeof(buffer), &userData);
		char* pStr = new char[32];
		memcpy(pStr, buffer, 32);

		SetColor(buffer,instruction.length);
		
		return pStr;
	
}

//DWORD CDisAssmble::disassembleBufferCall(ZydisDecoder * decoder, ZydisU8 * data, ZydisUSize length, ZydisBool installHooks, ZydisU64 instruct)
//{
//
//	ZydisFormatter formatter;
//	ZydisFormatterInit(&formatter, ZYDIS_FORMATTER_STYLE_INTEL);
//	ZydisFormatterSetProperty(&formatter, ZYDIS_FORMATTER_PROP_FORCE_MEMSEG, ZYDIS_TRUE);
//	ZydisFormatterSetProperty(&formatter, ZYDIS_FORMATTER_PROP_FORCE_MEMSIZE, ZYDIS_TRUE);
//
//	if (installHooks)
//	{
//		defaultPrintMnemonic = (ZydisFormatterFunc)&ZydisFormatterPrintMnemonic;
//		ZydisFormatterSetHook(&formatter, ZYDIS_FORMATTER_HOOK_PRINT_MNEMONIC,
//			(const void**)&defaultPrintMnemonic);
//		defaultFormatOperandImm = (ZydisFormatterOperandFunc)&ZydisFormatterFormatOperandImm;
//		ZydisFormatterSetHook(&formatter, ZYDIS_FORMATTER_HOOK_FORMAT_OPERAND_IMM,
//			(const void**)&defaultFormatOperandImm);
//	}
//
//	ZydisU64 instructionPointer = instruct;
//
//	ZydisDecodedInstruction instruction;
//	ZydisCustomUserData userData;
//	char buffer[256];
//
//	int i = 0;
//	while (ZYDIS_SUCCESS(
//		ZydisDecoderDecodeBuffer(decoder, data, length, instructionPointer, &instruction)))
//	{
//		data += instruction.length;
//		length -= instruction.length;
//		instructionPointer += instruction.length;
//	
//		
//	
//		ZydisFormatterFormatInstructionEx(
//			&formatter, &instruction, &buffer[0], sizeof(buffer), &userData);
//
//		if (strncmp("call", &buffer[0], 4) == 0)
//		{
//			
//			return instructionPointer;
//		}
//
//		i++;
//		if(i>=2)
//		{
//			break;
//		}
//	}
//
//	return -1;
//	
//}

DWORD CDisAssmble::disassembleBufferCall(ZydisDecoder * decoder, ZydisU8 * data, ZydisUSize length, ZydisBool installHooks, ZydisU64 instruct)
{

	ZydisFormatter formatter;
	ZydisFormatterInit(&formatter, ZYDIS_FORMATTER_STYLE_INTEL);
	ZydisFormatterSetProperty(&formatter, ZYDIS_FORMATTER_PROP_FORCE_MEMSEG, ZYDIS_TRUE);
	ZydisFormatterSetProperty(&formatter, ZYDIS_FORMATTER_PROP_FORCE_MEMSIZE, ZYDIS_TRUE);

	if (installHooks)
	{
		defaultPrintMnemonic = (ZydisFormatterFunc)&ZydisFormatterPrintMnemonic;
		ZydisFormatterSetHook(&formatter, ZYDIS_FORMATTER_HOOK_PRINT_MNEMONIC,
			(const void**)&defaultPrintMnemonic);
		defaultFormatOperandImm = (ZydisFormatterOperandFunc)&ZydisFormatterFormatOperandImm;
		ZydisFormatterSetHook(&formatter, ZYDIS_FORMATTER_HOOK_FORMAT_OPERAND_IMM,
			(const void**)&defaultFormatOperandImm);
	}

	ZydisU64 instructionPointer = instruct;

	ZydisDecodedInstruction instruction;
	ZydisCustomUserData userData;
	char buffer[256];

	int i = 0;

	ZydisDecoderDecodeBuffer(decoder, data, length, instructionPointer, &instruction);
	
		data += instruction.length;
		length -= instruction.length;
		instructionPointer += instruction.length;
	
		
	
		ZydisFormatterFormatInstructionEx(
			&formatter, &instruction, &buffer[0], sizeof(buffer), &userData);

		if (strncmp("call", &buffer[0], 4) == 0)
		{
			
			return instructionPointer;
		}

	return -1;
	
}
DWORD CDisAssmble::FindJmp(LPVOID lpAddress)
{
	HANDLE hProcess = OpenProcess(THREAD_ALL_ACCESS, FALSE, dwProcessId);

	ZydisU8 data[32] = { 0 };
	DWORD NumberOfBytesRead = 0;
	ReadProcessMemory(hProcess, (LPVOID)lpAddress, data, 32, &NumberOfBytesRead);

	ZydisDecoder decoder;
	ZydisDecoderInit(&decoder, ZYDIS_MACHINE_MODE_LONG_COMPAT_32, ZYDIS_ADDRESS_WIDTH_32);
	DWORD dwAddress=disassembleBufferJMP(&decoder, &data[0], sizeof(data), ZYDIS_FALSE, (ZydisU32)lpAddress);

	CloseHandle(hProcess);
	return dwAddress;
}


DWORD CDisAssmble::disassembleBufferJMP(ZydisDecoder* decoder, ZydisU8* data, ZydisUSize length, ZydisBool installHooks, ZydisU64 instruct)
{

	ZydisFormatter formatter;
	ZydisFormatterInit(&formatter, ZYDIS_FORMATTER_STYLE_INTEL);
	ZydisFormatterSetProperty(&formatter, ZYDIS_FORMATTER_PROP_FORCE_MEMSEG, ZYDIS_TRUE);
	ZydisFormatterSetProperty(&formatter, ZYDIS_FORMATTER_PROP_FORCE_MEMSIZE, ZYDIS_TRUE);

	if (installHooks)
	{
		defaultPrintMnemonic = (ZydisFormatterFunc)&ZydisFormatterPrintMnemonic;
		ZydisFormatterSetHook(&formatter, ZYDIS_FORMATTER_HOOK_PRINT_MNEMONIC,
			(const void**)&defaultPrintMnemonic);
		defaultFormatOperandImm = (ZydisFormatterOperandFunc)&ZydisFormatterFormatOperandImm;
		ZydisFormatterSetHook(&formatter, ZYDIS_FORMATTER_HOOK_FORMAT_OPERAND_IMM,
			(const void**)&defaultFormatOperandImm);
	}

	ZydisU64 instructionPointer = instruct;

	ZydisDecodedInstruction instruction;
	ZydisCustomUserData userData;
	char buffer[256];

	int i = 0;
	
	ZydisDecoderDecodeBuffer(decoder, data, length, instructionPointer, &instruction);

		data += instruction.length;
		length -= instruction.length;
		instructionPointer += instruction.length;

		ZydisFormatterFormatInstructionEx(
			&formatter, &instruction, &buffer[0], sizeof(buffer), &userData);

		if (strncmp("jmp", &buffer[0], 3) == 0)
		{
			char* pstr = strstr(buffer, "ptr");
			if (pstr != NULL)
			{
				char* str = "]";
				DWORD NumberOfBytesRead = 0;
				char szbuf[4] = { 0 };
				DWORD lpaddress = strtol(&buffer[18], &str, 16);
				HANDLE hProcess = OpenProcess(THREAD_ALL_ACCESS, FALSE, dwProcessId);
				ReadProcessMemory(hProcess, (LPVOID)lpaddress, (LPVOID)szbuf, 4, &NumberOfBytesRead);
				LPVOID lpFunAddress = (LPVOID)*(DWORD*)szbuf;
				CloseHandle(hProcess);
				return (DWORD)lpFunAddress;
			}
	    }

	return -1;
}

DWORD CDisAssmble::FindCallAddress(LPVOID lpAddress)
{
	HANDLE hProcess = OpenProcess(THREAD_ALL_ACCESS, FALSE, dwProcessId);

	char data[4] = { 0 };
	DWORD NumberOfBytesRead = 0;
	ReadProcessMemory(hProcess, (LPVOID)lpAddress, data, 4, &NumberOfBytesRead);


	//DWORD dwAddress = disassembleBufferJMP(&decoder, &data[0], sizeof(data), ZYDIS_FALSE, (ZydisU32)lpAddress);

	CloseHandle(hProcess);
	return *(DWORD*)data;
}

void CDisAssmble::SetColor(char* buffer, int OpencodeLength)
{
	
	HANDLE  consolehwnd = GetStdHandle(STD_OUTPUT_HANDLE);
	char szbuf[256] = { 'j','m','p','.' };
	//判断是不是call
	if (strncmp("call", &buffer[0], 4) == 0)
	{
		//取call后面的地址
		DWORD lpaddress = 0;
		char* pstr2 = strstr(buffer, "ptr");
		if(pstr2==NULL)
		{
			 lpaddress = strtol(&buffer[5], NULL, 16);

			 //判断这个地址是不是jmp的
			 DWORD dwAddres = FindJmp((LPVOID)lpaddress);
			 //打印call
			 SetConsoleTextAttribute(consolehwnd, BACKGROUND_GREEN);
			 char* pstr = strtok(buffer, " ");
			 printf("%s", pstr);

			 //找到jmp后地址
			 char* strFunName = pFile->FindFunName((LPVOID)dwAddres);
			 if (strFunName != NULL)
			 {
				 //打印地址名
				 SetConsoleTextAttribute(consolehwnd, FOREGROUND_RED);
				 strcat(szbuf, strFunName);
				 printf(" %s\n", szbuf);
				 SetConsoleTextAttribute(consolehwnd, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
			 }
			 else
			 {
				 SetConsoleTextAttribute(consolehwnd, FOREGROUND_RED);
				 printf(" %s\n", &buffer[5]);
				 SetConsoleTextAttribute(consolehwnd, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

			 }
		}
		else
		{
			/*lpaddress = strtol(&buffer[5], NULL, 16);*/
			char* str = "]";
			DWORD NumberOfBytesRead = 0;
			char szbuf[4] = { 0 };
			lpaddress = strtol(&buffer[19], &str, 16);
			DWORD dwAddres = FindCallAddress((LPVOID)lpaddress);
			char* strFunName = pFile->FindFunName((LPVOID)dwAddres);
			//打印call
			SetConsoleTextAttribute(consolehwnd, BACKGROUND_GREEN);
			char* pstr = strtok(buffer, " ");
			printf("%s", pstr);

			SetConsoleTextAttribute(consolehwnd, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
			printf(" ");

			if (strFunName != NULL)
			{
				//打印地址名
				SetConsoleTextAttribute(consolehwnd, FOREGROUND_RED);
				//strcat(szbuf, strFunName);
				printf("%s\n", strFunName);
				SetConsoleTextAttribute(consolehwnd, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
			}
			else
			{
				SetConsoleTextAttribute(consolehwnd, FOREGROUND_RED);
				printf(" %s\n", &buffer[5]);
				SetConsoleTextAttribute(consolehwnd, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

			}
		}
		
			
	}
	else if (strncmp("push", &buffer[0], 4) == 0)
	{
		SetConsoleTextAttribute(consolehwnd, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
		char* pstr = strtok(buffer, " ");
		printf("%s", pstr);
		if (OpencodeLength > 3)
		{
			SetConsoleTextAttribute(consolehwnd, FOREGROUND_RED);
			printf(" %s\n", &buffer[5]);
			SetConsoleTextAttribute(consolehwnd, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		}
		else
		{
			SetConsoleTextAttribute(consolehwnd, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
			printf(" %s\n", &buffer[5]);
		}
	}
	else if(strncmp("jmp", &buffer[0], 3) == 0)
	{
		char* pstr = strstr(buffer, "ptr");

		SetConsoleTextAttribute(consolehwnd, FOREGROUND_INTENSITY | FOREGROUND_RED| FOREGROUND_GREEN);
		char* pstr1 = strtok(buffer, " ");
		printf("%s", pstr1);

		
		if (pstr != NULL)
		{
			char* str = "]";
			DWORD NumberOfBytesRead = 0;
			char szbuf[4] = { 0 };
			DWORD lpaddress = strtol(&buffer[18], &str, 16);
			HANDLE hProcess = OpenProcess(THREAD_ALL_ACCESS, FALSE, dwProcessId);
			ReadProcessMemory(hProcess, (LPVOID)lpaddress, (LPVOID)szbuf, 4, &NumberOfBytesRead);
			LPVOID lpFunAddress = (LPVOID)*(DWORD*)szbuf;
			CloseHandle(hProcess);

			//找到jmp后地址
			char* strFunName = pFile->FindFunName(lpFunAddress);
			if (strFunName != NULL)
			{
				//打印地址名
				SetConsoleTextAttribute(consolehwnd, FOREGROUND_RED);
				
				printf(" %s\n", strFunName);
				SetConsoleTextAttribute(consolehwnd, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
			}
			else
			{
				SetConsoleTextAttribute(consolehwnd, FOREGROUND_RED);
				printf(" %s\n", &buffer[4]);
				SetConsoleTextAttribute(consolehwnd, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

			}
		}
		else
		{
			SetConsoleTextAttribute(consolehwnd, FOREGROUND_RED);
			printf(" %s\n", &buffer[4]);
			SetConsoleTextAttribute(consolehwnd, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		}

	}
	else if (strncmp("mov", &buffer[0], 3) == 0)
	{
		char* pstr = strstr(buffer, "ptr");
		if(pstr==NULL)
		{
			printf("%s\n", &buffer[0]);
		}
		else
		{
			char* str = "]";
			DWORD NumberOfBytesRead = 0;
			char szbuf[4] = { 0 };
			DWORD lpaddress = strtol(&buffer[23], &str, 16);
			HANDLE hProcess = OpenProcess(THREAD_ALL_ACCESS, FALSE, dwProcessId);
			ReadProcessMemory(hProcess, (LPVOID)lpaddress, (LPVOID)szbuf, 4, &NumberOfBytesRead);
			LPVOID lpFunAddress = (LPVOID)*(DWORD*)szbuf;
			CloseHandle(hProcess);
			//找到jmp后地址
			char* strFunName = pFile->FindFunName(lpFunAddress);
			if (strFunName != NULL)
			{
				//打印地址名
				SetConsoleTextAttribute(consolehwnd, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
				char* pstr1 = strtok(buffer, ",");
				printf("%s", pstr1);
				SetConsoleTextAttribute(consolehwnd, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);		
				printf(",");
				SetConsoleTextAttribute(consolehwnd, FOREGROUND_RED);
				printf(" %s\n", strFunName);
				SetConsoleTextAttribute(consolehwnd, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
			}
			else
			{
				printf("%s\n", &buffer[0]);
			}

		}
	}
	else
	{
		printf("%s\n", &buffer[0]);
	}
}

