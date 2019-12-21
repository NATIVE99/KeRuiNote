#include "stdafx.h"
#include "Control.h"


CControl::CControl()
{
	//初始化一些成员
	dwContinueStatus = DBG_CONTINUE;
	pShowErro = new CShowErro();
	pBreak = new CBreakPoint;
	
	memset(&mSi, 0, sizeof(STARTUPINFO));
	memset(&mPi, 0, sizeof(PROCESS_INFORMATION));
	mSi.cb= sizeof(STARTUPINFO);
	mIsSystem = true;
	 mIsMemory=false;
	 mIsInt3 = false;
	 mIsStep = false;
	 mIsEnterPoint = false;
	 mIsMemBreakDeal = false;
	 mIsScrip = false;
	 mIsTrace = false;
	 ModuleSize = {0};
	 mIsContinue = false;
	// mModuleSize = { 0 };
}



CControl::~CControl()
{
	delete pBreak;
	delete pShowErro;
}

bool CControl::Start(char * strPath)
{
	mFilePath = strPath;
	if (CreateProcessA(strPath, NULL, NULL, false, false, 
						DEBUG_PROCESS, NULL, NULL, &mSi, &mPi)) 
	{	
		MainLoop();

		return true;
	}
	else
	{
		pShowErro->ShowErroApi("CreateProcessA");
		return false;
	}

	
}

		
void CControl::MainLoop()
{

	while (true)
	{
		WaitForDebugEvent(&mEvent, INFINITE);

		switch (mEvent.dwDebugEventCode)
		{
		case EXCEPTION_DEBUG_EVENT:

			OnException();
			break;
		case CREATE_THREAD_DEBUG_EVENT:

			OnCreateThread();
			break;
		case CREATE_PROCESS_DEBUG_EVENT:
			OnCreateProcess();
			break;
		case EXIT_THREAD_DEBUG_EVENT:
			OnExitThread();
			break;
		case EXIT_PROCESS_DEBUG_EVENT:

			OnExitProcess();
			break;
		case LOAD_DLL_DEBUG_EVENT:

			OnLoadDll();
			break;

		case UNLOAD_DLL_DEBUG_EVENT:
			OnUnLoadDll();
			break;
		case OUTPUT_DEBUG_STRING_EVENT:
			OnOutputDebugString();
			break;
		}
		ContinueDebugEvent(mEvent.dwProcessId,
			mEvent.dwThreadId, dwContinueStatus);
		
			mIsContinue = false;
	}
}

void CControl::Command()
{
	char Input[256] = { 0 };
	
	while(1)
	{
		if(mIsScrip)
		{	
			memset(Input, 0, 256);
			memcpy(Input, mCommandList.front(), 256);
			printf("Command:%s", mCommandList.front());
			mCommandList.pop_front();

			if (mCommandList.size() <= 1)
			{
				mIsScrip = false;
			}
	
		}
		else if(mIsTrace)
		{
			memset(Input, 0, 256);
			memcpy(Input, "t",1);
		
		}
		else
		{
			memset(Input, 0, 256);
			printf("Command:");
			gets_s(Input);
			char* szbuf = new char[256];
			memcpy(szbuf, Input, 256);
			RecordCommand(szbuf);
		}
		if (strcmp("q", Input) == 0)
		{
			exit(0);
		}
		else if(strcmp("g",Input)==0)
		{
			break;
		}
		else if(strncmp("u", Input,1)==0)
		{
			if(strlen(Input)>2)
			{
				int lpaddress= strtol(&Input[2], NULL, 16);
				ShowAsm((LPVOID)lpaddress);
			}
			else
			{
				ShowAsm();
			}
		}
		else if (strncmp("r", Input, 1) == 0)
		{

			ShowRegist();
		}
		else if (strncmp("dd", Input, 2) == 0)
		{
			if (strlen(Input) > 2)
			{
				int lpaddress = strtol(&Input[2], NULL, 16);
				ShowMemory((LPVOID)lpaddress);
			}
		}
		else if (strncmp("ls", Input, 2) == 0)
		{
			ImportScript();
		}
		else if (strncmp("es", Input, 2) == 0)
		{
			ExportScript();
		}
		else if (strncmp("e", Input, 1) == 0)
		{
			int lpaddress = strtol(&Input[1], NULL, 16);
			char Code = strtol(&Input[11], NULL, 16);

			//char Code = Input[11];
			AltMemoryValue((LPVOID)lpaddress, Code);
		}
		else if (strncmp("trace", Input, 5) == 0)
		{
			//有问题
			//如果EIP不走那个语句会停不下来
			char* str = NULL;
			int lpStartAddress = strtol(&Input[6], NULL, 16);
			int lpEndAddress= strtol(&Input[15], NULL, 16);
			if(strlen(Input)>24)
			{
				str = &Input[24];
			}
			else
			{
				str = NULL;
			}
			

			Trace((LPVOID)lpStartAddress,(LPVOID)lpEndAddress, str);
		}
		else if(strncmp("t", Input, 1) == 0)
		{
			mIsStep = true;
			pBreak->SetSingleStep(mEvent);
			break;
		}
		else if (strncmp("bpc", Input, 3) == 0)
		{
			int nOrd = strtol(&Input[4], NULL, 16);
			LPVOID lpAddress=pBreak->FindAddressInOrdinal(nOrd);
			HANDLE hThread = NULL;
			HANDLE hProcess = NULL;
			CONTEXT context = GetContext(hThread, hProcess);
			AltOldCodeEip(lpAddress,hThread,hProcess,context);
			pBreak->DeleteInt3ReptBreak(lpAddress);
		}
		else if (strncmp("bpl", Input, 3) == 0)
		{
			pBreak->ShowInt3Break();
		}
		else if(strncmp("bp", Input, 2)==0)
		{
			int lpaddress = strtol(&Input[3], NULL, 16);
			pBreak->SetInt3Break(mEvent, (LPVOID)lpaddress,false);
		}
		else if(strncmp("p", Input, 1) == 0)
		{
			//1.需要判断下一条指令是否为call
			//2.如果不是则设置单步，如果是则在call地址下断点，然后g

			int nRet=FindCall();
			if(nRet==-1)
			{
				mIsStep = true;
				pBreak->SetSingleStep(mEvent);
				break;
			}
			else
			{
				pBreak->SetInt3Break(mEvent, (LPVOID)nRet, true);
				
				break;
			}
			
		}
		else if (strncmp("bmpl", Input, 4) == 0)
		{
			pBreak->ShowPageMemBreakPoint();
		}
		else if (strncmp("bmc", Input, 3) == 0)
		{
			int Ord = strtol(&Input[4], NULL, 16);
			pBreak->DelMemBreakPoint(Ord, mEvent);
		}
		else if (strncmp("bml", Input, 3) == 0)
		{
			pBreak->ShowALLMemBreakPoint();
		}
		else if(strncmp("bm", Input, 2)==0)
		{
			
			//内存断点功能最难写
			int Startaddress = strtol(&Input[3], NULL, 16);
			int Endaddress = strtol(&Input[12], NULL, 16);
			int type = atoi(&Input[21]);
			pBreak->SetMemBreakPoint(mEvent, Startaddress, Endaddress, type);

		}
		else if (strncmp("bhl", Input, 3) == 0)
		{
			pBreak->ShowHardBreakPoint();
		}
		else if (strncmp("bhc", Input, 3) == 0)
		{
			int Ord = strtol(&Input[4], NULL, 16);
			pBreak->DelHardBreakPoint(mEvent,Ord);
		}
		else if (strncmp("bh", Input, 2) == 0)
		{
			int nRn = strtol(&Input[3], NULL, 16);
			int Startaddress = strtol(&Input[5], NULL, 16);
			char Type = Input[0xe];
			int nlength = atoi(&Input[16]);
			pBreak->SetHardBreakPoint(mEvent, nRn, (LPVOID)Startaddress, Type, nlength);
		}
		
	
	}
}


void CControl::ShowAsm()
{
	CONTEXT context;
	HANDLE hThread = NULL;
	HANDLE hProcess = NULL;
	context = GetContext(hThread, hProcess);

	ZydisU8 data[32] = { 0 };
	DWORD NumberOfBytesRead = 0;
	ReadProcessMemory(hProcess, (LPVOID)context.Eip, data, sizeof(data), &NumberOfBytesRead);


	ZydisDecoder decoder;
	ZydisDecoderInit(&decoder, ZYDIS_MACHINE_MODE_LONG_COMPAT_32, ZYDIS_ADDRESS_WIDTH_32);

	pDis->disassembleBuffer(&decoder, &data[0], sizeof(data), ZYDIS_FALSE, context.Eip);

	CloseHandle(hProcess);
	CloseHandle(hThread);
}

void CControl::ShowAsm(LPVOID lpAddress)
{

	HANDLE hProcess = OpenProcess(THREAD_ALL_ACCESS, FALSE, mEvent.dwProcessId);

	ZydisU8 data[32] = { 0 };
	DWORD NumberOfBytesRead = 0;
	ReadProcessMemory(hProcess, (LPVOID)lpAddress, data, 32, &NumberOfBytesRead);

	//显示的时候修复成原来的指令，不让他显示CC
	for(int i=0;i<32;i++)
	{
		if(data[i] == 0xcc)
		{
			char n=pBreak->FindReptInt3Break((LPVOID)((DWORD)lpAddress + i));
			if(n!=0xcc)
			{
				data[i] = n;
			}
		}
	}
	ZydisDecoder decoder;
	ZydisDecoderInit(&decoder, ZYDIS_MACHINE_MODE_LONG_COMPAT_32, ZYDIS_ADDRESS_WIDTH_32);
	pDis->disassembleBuffer(&decoder, &data[0], sizeof(data), ZYDIS_FALSE, (ZydisU32)lpAddress);
	CloseHandle(hProcess);
}

void CControl::ShowAsm(int nNum)
{
	CONTEXT context;
	HANDLE hThread = NULL;
	HANDLE hProcess = NULL;
	context = GetContext(hThread, hProcess);
	//ALT
	ZydisU8 data[32] = { 0 };
	//
	DWORD NumberOfBytesRead = 0;
	ReadProcessMemory(hProcess, (LPVOID)context.Eip, data, sizeof(data), &NumberOfBytesRead);


	ZydisDecoder decoder;
	ZydisDecoderInit(&decoder, ZYDIS_MACHINE_MODE_LONG_COMPAT_32, ZYDIS_ADDRESS_WIDTH_32);
	DWORD dwEip;
	char* str=pDis->disassembleBuffer1(&decoder, &data[0], sizeof(data), ZYDIS_FALSE, context.Eip);

	if(mIsTrace)
	{	
		if(context.Eip==mdwEnd)
		{
			fclose(mFile);
			//pModuleSize = NULL;
			mIsTrace = false;
			mFile = NULL;
		}
		if (mFile != NULL)
		{	//追到模块的地址才要记录下来
			if(ModuleSize.dwStart<=context.Eip&&context.Eip<=(ModuleSize.dwStart+ ModuleSize.dwSize))
			{
				fprintf(mFile, "%08X %s\n", context.Eip,str);
			}	
		}

		delete[] str;
	}

	//ShowRegist(context);

	CloseHandle(hProcess);
	CloseHandle(hThread);
}

void CControl::ShowRegist()
{

	CONTEXT context;
	HANDLE hThread = NULL;
	HANDLE hProcess = NULL;
	context = GetContext(hThread, hProcess);

	
	printf("\r\n");
	printf("EAX = %08X   EBX = %08X  ECX = %08X  EDX = %08X  ESI = %08X\r\n", context.Eax, context.Ebx, context.Ecx, context.Edx, context.Esi);
	printf("EDI = %08X   ESP = %08X  EBP = %08X  EIP =%08X\r\n", context.Edi, context.Esp, context.Ebp, context.Eip);
	printf("CS = %04X        DS = %04X       ES = %04X       SS = %04X       FS = %04X\r\n", context.SegCs, context.SegDs, context.SegEs, context.SegSs, context.SegFs);

	tagEflag Eflag = *(tagEflag*)&context.EFlags;
	printf("CF:%d PF:%d AF:%d ZF:%d SF:%d TF:%d IF:%d DF:%d OF:%d\r\n", Eflag.CF, Eflag.PF, Eflag.AF, Eflag.ZF, Eflag.SF, Eflag.TF, Eflag.IF, Eflag.DF, Eflag.OF);

	CloseHandle(hProcess);
	CloseHandle(hThread);
}

void CControl::ShowMemory(LPVOID lpAddress)
{
	CONTEXT context;
	HANDLE hThread = NULL;
	HANDLE hProcess = NULL;
	context = GetContext(hThread, hProcess);

	ZydisU8 data[128] = { 0 };
	//
	DWORD NumberOfBytesRead = 0;
	ReadProcessMemory(hProcess, (LPVOID)lpAddress, data, sizeof(data), &NumberOfBytesRead);

	int nAddress = (DWORD)lpAddress & 0XFFFFFFF0;
	int n =(DWORD)lpAddress- nAddress;
	int nNumSpace = n;
	int nEnd = 0;
	int nBeg = 0;
	for(int i=0;i<8;i++)
	{
		printf("%08X ", nAddress);
		nAddress = nAddress + 16;
		nBeg = nEnd;
		for(int j=0;j<16;j++)
		{
			if(n>0)
			{
					printf("   ");
					n--;
					continue;
			}
			if (j==7)
			{
				printf("%02X", data[16 * i + j- nNumSpace]);
				printf("--");
				continue;
			}
			nEnd = 16 * i + j - nNumSpace;
			printf("%02X ", data[nEnd]);
			
		}
		printf("  ");
		if(i==0)
		{
			for(int k=0;k<nNumSpace+1;k++)
			{
				printf(" ");
			}
		}
		for(int k=nBeg;k<nEnd;k++)
		{
			if(data[k]>0x20&& data[k]<0x7e)
			{
				printf("%c", data[k]);
			}
			else
			{
				printf(".");
			}
			
		}
		printf("\r\n");
	}


	CloseHandle(hProcess);
	CloseHandle(hThread);
}

DWORD CControl::OnException()
{
	switch (mEvent.u.Exception.ExceptionRecord.ExceptionCode)
	{
	case EXCEPTION_ACCESS_VIOLATION:
		// First chance: Pass this on to the system. 
		// Last chance: Display an appropriate error. 
		
		OnExceptionAccessViolation();

		break;
	case EXCEPTION_BREAKPOINT:
		// First chance: Display the current 
		// instruction and register values. 
		OnExceptionBreakpoint();
		break;
	case EXCEPTION_DATATYPE_MISALIGNMENT:
		// First chance: Pass this on to the system. 
		// Last chance: Display an appropriate error. 
		break;
	case EXCEPTION_SINGLE_STEP:
		// First chance: Update the display of the 
		// current instruction and register values. 

		OnExceptionSingleStep();
		break;
	case DBG_CONTROL_C:
		// First chance: Pass this on to the system. 
		// Last chance: Display an appropriate error. 

		// Handle other exceptions.

		break;
	}
	if(mIsContinue)
	{
		dwContinueStatus = DBG_CONTINUE;
	}
	else
	{
		dwContinueStatus = DBG_EXCEPTION_NOT_HANDLED;
	}
	
	return 0;
}

DWORD CControl::OnUnLoadDll()
{	
	printf("UnloadDll lpBaseOfDll:%p \n", mEvent.u.UnloadDll.lpBaseOfDll);
	dwContinueStatus = DBG_CONTINUE;
	return 0;
}

DWORD CControl::OnOutputDebugString()
{
		if (mEvent.u.DebugString.fUnicode == 1) {
			wprintf(L"OutputStringDebugEvnet length:%d %s\n",
				mEvent.u.DebugString.nDebugStringLength,
				mEvent.u.DebugString.lpDebugStringData);
		}
		else {
			printf("OutputStringDebugEvnet length:%d %s\n",
				mEvent.u.DebugString.nDebugStringLength,
				mEvent.u.DebugString.lpDebugStringData);
		}
	dwContinueStatus = DBG_CONTINUE;
	return 0;
}

DWORD CControl::OnLoadDll()
{
	LOAD_DLL_DEBUG_INFO info = mEvent.u.LoadDll;
	

	wchar_t path[MAX_PATH];
	SIZE_T NumberOfBytesRead;

	path[0] = '\0';
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, mEvent.dwProcessId);
	if (ReadProcessMemory(hProcess,
		info.lpImageName,
		path,
		sizeof(int*),
		&NumberOfBytesRead))
	{
		ReadProcessMemory(hProcess,
			*(int**)path,
			path,
			sizeof(path),
			&NumberOfBytesRead);
	}

	if(info.fUnicode)
	{
		if(path[0]==NULL)
		{
			wprintf(L"LoadDll lpBaseOfDll:%p name:C:\\Windows\\SysWOW64\\ntdll.dll\n", info.lpBaseOfDll);

		}
		else
		{
			wprintf(L"LoadDll lpBaseOfDll:%p name:%s\n", info.lpBaseOfDll, path);

		}
	}
	else
	{
		printf("LoadDll lpBaseOfDll:%p name:%s\n", info.lpBaseOfDll, path);
	}

	
	CloseHandle(hProcess);
	dwContinueStatus = DBG_CONTINUE;
	return 0;
}

DWORD CControl::OnCreateProcess()
{

	
	printf("CreateProcessDebugEvnet pid:%d tid:%d Base=0x%p ImageName:%s StartAddress=0x%p\n",
		mEvent.dwProcessId,
		mEvent.dwThreadId,
		mEvent.u.CreateProcessInfo.lpBaseOfImage,
		mEvent.u.CreateProcessInfo.lpImageName,
		mEvent.u.CreateProcessInfo.lpStartAddress);

	//设置入口断点
	pBreak->SetInt3Break(mEvent, mEvent.u.CreateProcessInfo.lpStartAddress, true);
	mIsEnterPoint = true;
    dwContinueStatus = DBG_CONTINUE;
	return 0;
}

DWORD CControl::OnCreateThread()
{
	 dwContinueStatus = DBG_CONTINUE;
	printf("CreateThreadDebugEvnet pid:%d tid:%d hThread:%p StartAddress:%p\n",
		mEvent.dwProcessId,
		mEvent.dwThreadId,
		mEvent.u.CreateThread.hThread,
		mEvent.u.CreateThread.lpStartAddress);
	return 0;
}

DWORD CControl::OnExitProcess()
{

	 dwContinueStatus = DBG_CONTINUE;
	printf("ExitProcessDebugEvnet pid:%d ExitCode:%d\n",
		mEvent.dwProcessId,
		mEvent.u.ExitProcess.dwExitCode);

	//结束调试,主进程结束
	if (mEvent.dwProcessId == mPi.dwProcessId)
		exit(0);

	return 0;
}

DWORD CControl::OnExitThread()
{
	 dwContinueStatus = DBG_CONTINUE;
	printf("ExitThreadDebugEvnet tid:%d ExitCode:%d\n",
		mEvent.dwThreadId,
		mEvent.u.ExitThread.dwExitCode);
	return 0;
}

CONTEXT CControl::GetContext(HANDLE& hThread, HANDLE& hProcess)
{
	 hThread = OpenThread(THREAD_ALL_ACCESS, FALSE, mEvent.dwThreadId);
	 hProcess = OpenProcess(THREAD_ALL_ACCESS, FALSE, mEvent.dwProcessId);
	CONTEXT context;
	context.ContextFlags = CONTEXT_FULL;
	GetThreadContext(hThread, &context);

	return context;
}

void CControl::AltMemoryValue(LPVOID lpAddress,char Code)
{
	DWORD NumberOfBytesWrite = 0;
	HANDLE hProcess = OpenProcess(THREAD_ALL_ACCESS, FALSE, mEvent.dwProcessId);
	int bRet = WriteProcessMemory(hProcess, lpAddress, &Code, sizeof(char), &NumberOfBytesWrite);
	//check it
	CloseHandle(hProcess);
}

void CControl::OnExceptionBreakpoint()
{

	
	//设置mIsSystem用来忽略系统断点
	if (!mIsSystem)
	{
		/******************************************************************/
		//int 3
		//AltOldCodeEip();

		HANDLE hThread = NULL;
		HANDLE hProcess = NULL;	
		CONTEXT context = GetContext(hThread, hProcess);

		char cFind=pBreak->FindReptInt3Break((LPVOID)(context.Eip - 1));
		char cFind2 = pBreak->FindOnceInt3Break((LPVOID)(context.Eip - 1));
		char cCmp = 0XCC;
		if(cFind== cCmp&&cFind2== cCmp)
		{
			mIsContinue = false;
		}
		else
		{
			if(cFind2!= cCmp)
			{
				mIsEnterPoint = true;
			}
			LPVOID lpEip = (LPVOID)(context.Eip - 1);
			AltOldCodeEip(lpEip, hThread, hProcess, context);
			//断步配合
			
			pBreak->SetSingleStep(mEvent);
			
			ShowAsm(1);
			ShowRegist();
			mIsInt3 = true;
			mIsContinue = true;
		}
		
	   /******************************************************************/
	}
	else
	{
		pDis = new CDisAssmble(mFilePath,mEvent.dwProcessId);
		mIsSystem = false;
	}
}

void CControl::OnExceptionSingleStep()
{
	//是否是INT3的断步配合
	if(mIsInt3)
	{
		mIsInt3 = false;
	
		//ShowRegist();
		//ShowAsm(1);
				
		if (!mIsEnterPoint)
		{
			AltOldCode(mlpAltEip);
			mIsEnterPoint = false;
		}
		Command();
	}
	
	if(mIsStep)
	{
		mIsStep = false;
		ShowAsm(1);
		ShowRegist();
		Command();
	}

	//是否是内存的断步配合
	if(mIsMemory)
	{
		mIsMemory = false;

		DWORD OldProtect;
		int nStart = mdwAddress & 0xFFFFF000;
		HANDLE hProcess = OpenProcess(THREAD_ALL_ACCESS, FALSE, mEvent.dwProcessId);
		BOOL bRet = VirtualProtectEx(hProcess, (LPVOID)nStart, 0x1000, PAGE_NOACCESS, &OldProtect);

		CloseHandle(hProcess);

		if(mIsMemBreakDeal)
		{
			//ShowRegist();
			//ShowAsm(1);
			Command();
			mIsMemBreakDeal = false;
		}
	}

	//判断是否是硬件断点

	if(pBreak->IsHardBreakPoint(mEvent))
	{
		ShowAsm(1);
		ShowRegist();
		Command();
		
	}

	mIsContinue = true;
	
}

void CControl::OnExceptionAccessViolation()
{
	//ExceptionInformation[0]包含了表示引发访问违规操作类型的读写标志。如果该标志为0，
	//表示线程试图读一个不可访问地址处的数据；如果该标志是1，表示线程试图写数据到一个不可访问的地址。
	//数组的第二个元素ExceptionInformation[1]指定了不可访问的地址
	 mdwflag = mEvent.u.Exception.ExceptionRecord.ExceptionInformation[0];

	//出异常访问的地址
	 mdwAddress = mEvent.u.Exception.ExceptionRecord.ExceptionInformation[1];

	 tagMemoryBreakPointMsg * pTag=pBreak->GetMemoryBreakPoint((LPVOID)mdwAddress);
	 
	 //我们需要处理的
	 if (pTag != nullptr)
	 {
		 mIsContinue = true;
		 if(mdwflag==0&& pTag->Attribute== READ|| mdwflag == 1 && pTag->Attribute == WRITE)
		{
			ShowAsm(1);
			ShowRegist();
			mIsMemBreakDeal = true; 
		}
	 }
	 else
	 {
		 //我们不需要处理的
		 mIsContinue = false;
	 }
	int nStart = mdwAddress & 0xFFFFF000;
	DWORD OldProtect;
	DWORD Protect;
	std::list<tagMemoryBreakPointMsg*>* pList = pBreak->GetMemoryBreakPointlist((LPVOID)nStart);
	if(pList!=nullptr)
	{
		for(auto i: *pList)
		{
			Protect = i->OldProtect;
			break;
		}
		HANDLE hProcess = OpenProcess(THREAD_ALL_ACCESS, FALSE, mEvent.dwProcessId);
		BOOL bRet = VirtualProtectEx(hProcess, (LPVOID)nStart, 0x1000, Protect, &OldProtect);
		pBreak->SetSingleStep(mEvent);
		mIsMemory = true;
		CloseHandle(hProcess);
	}
}

void CControl::AltOldCodeEip(LPVOID lpAddress, HANDLE hThread ,HANDLE hProcess,CONTEXT& context)
{
		DWORD NumberOfBytesWrite = 0;
		context.Eip = (DWORD)lpAddress;
		char Old = pBreak->FindReptInt3Break(lpAddress);
		char cNoFind = 0xcc;
		if (Old != cNoFind)
		{
			int bRet = WriteProcessMemory(hProcess, lpAddress, &Old, sizeof(Old), &NumberOfBytesWrite);
			if (bRet == 0)
			{
				pShowErro->ShowErroApi(" CControl::OnExceptionBreakpoint：WriteProcessMemory");
			}
		}
		else
		{
			Old = pBreak->FindOnceInt3Break(lpAddress);
			int bRet = WriteProcessMemory(hProcess, lpAddress, &Old, sizeof(Old), &NumberOfBytesWrite);
			if (bRet == 0)
			{
				pShowErro->ShowErroApi(" CControl::OnExceptionBreakpoint：WriteProcessMemory");
			}
	
			pBreak->DeleteInt3OnceBreak(lpAddress);
		}
			SetThreadContext(hThread, &context);
	
		mlpAltEip = lpAddress;
		CloseHandle(hProcess);
		CloseHandle(hThread);
}

void CControl::AltOldCode(LPVOID address)
{

	DWORD NumberOfBytesWrite = 0;
	HANDLE hProcess = OpenProcess(THREAD_ALL_ACCESS, FALSE, mEvent.dwProcessId);
	char cNoFind = 0xcc;

		int bRet = WriteProcessMemory(hProcess, address, &cNoFind, sizeof(cNoFind), &NumberOfBytesWrite);
		if (bRet == 0)
		{
			pShowErro->ShowErroApi(" CControl::OnExceptionBreakpoint：WriteProcessMemory");
		}	
	CloseHandle(hProcess);
}

int CControl::FindCall()
{

	int address=0;
	CONTEXT context;
	HANDLE hThread = NULL;
	HANDLE hProcess = NULL;

	context = GetContext(hThread, hProcess);

	ZydisU8 data[32] = { 0 };
	DWORD NumberOfBytesRead = 0;
	ReadProcessMemory(hProcess, (LPVOID)context.Eip, data, sizeof(data), &NumberOfBytesRead);


	ZydisDecoder decoder;
	ZydisDecoderInit(&decoder, ZYDIS_MACHINE_MODE_LONG_COMPAT_32, ZYDIS_ADDRESS_WIDTH_32);

	address=pDis->disassembleBufferCall(&decoder, &data[0], sizeof(data), ZYDIS_FALSE, context.Eip);

	CloseHandle(hProcess);
	CloseHandle(hThread);
	
	return address;
}

void CControl::RecordCommand(char* szbuf)
{
	mRecordList.push_back(szbuf);
}

void CControl::ExportScript()
{
	FILE * pFile;
	pFile = fopen("Script.txt", "w+");
	if(pFile!=NULL)
	{
		for (auto i : mRecordList)
		{
			fprintf(pFile, "%s\n", i);
			
		}
	}
	fclose(pFile);
	
}

void CControl::ImportScript()
{
	FILE * pFile;
	pFile = fopen("Script.txt", "r+");
	
	while (!feof(pFile))								
	{
		char* szbuf = new char[256];
		fgets(szbuf, 256, pFile);	
		mCommandList.push_back(szbuf);
	}
	fclose(pFile);
	mIsScrip = true;

}

void CControl::Trace(LPVOID start, LPVOID end, char* strName)
{
	GetProcessModule();
	mIsTrace = true;
	mFile = fopen("trace.txt", "w+");
	mdwEnd =(DWORD)end;
	mdwStart = (DWORD)start;
	
	if(strName==NULL)
	{
		ModuleSize.dwSize = 0XFFFFFFFF;
		ModuleSize.dwStart = 0;
		return;
	}
	
	for(auto i:mMoudel)
	{
		int n=strcmp(strName, i.first);
		if(n==0)
		{
			ModuleSize = i.second;
			break;
		}
	}

	
}


BOOL CControl::GetProcessModule()
{


	HANDLE        hModuleSnap = NULL;
	MODULEENTRY32 me32 = { 0 };

	hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, mEvent.dwProcessId);
	if (hModuleSnap == INVALID_HANDLE_VALUE)
		return (FALSE);

	me32.dwSize = sizeof(MODULEENTRY32);

	if (Module32First(hModuleSnap, &me32))
	{
		do
		{
			char* pName = new char[24];
			tagModuleMs tagMod = { (DWORD)me32.modBaseAddr,me32.modBaseSize };

			memcpy(pName, me32.szModule, 24);
			_strlwr(pName);
			mMoudel.insert(std::pair <char*, tagModuleMs>(pName, tagMod));

		} while (Module32Next(hModuleSnap, &me32));

	}							
	CloseHandle(hModuleSnap);
	return true;
}
