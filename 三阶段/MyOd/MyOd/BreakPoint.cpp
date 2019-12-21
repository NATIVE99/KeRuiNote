#include "stdafx.h"
#include "BreakPoint.h"


CBreakPoint::CBreakPoint()
{
	pShow = new CShowErro;
}


CBreakPoint::~CBreakPoint()
{
}

void CBreakPoint::SetSingleStep(DEBUG_EVENT& Event)
{
	CONTEXT context;
	context.ContextFlags = CONTEXT_CONTROL;
	HANDLE hThread=OpenThread(THREAD_ALL_ACCESS, false, Event.dwThreadId);
	BOOL bRet=GetThreadContext(hThread, &context);
	if(!bRet)
	{
		pShow->ShowErroApi("CBreakPoint:GetThreadContext");
	}
	//把TF位置1，下单步断点
	context.EFlags |= 0x100;
	SetThreadContext(hThread, &context);
}

void CBreakPoint::SetInt3Break(DEBUG_EVENT & Event, LPVOID Address, bool LsOnce)
{
	char OldCode = '0';
	char ExcptCode = 0xcc;
	DWORD NumberOfBytesRead=0;
	DWORD NumberOfBytesWrite = 0;
	HANDLE hProcess = OpenProcess(THREAD_ALL_ACCESS, FALSE, Event.dwProcessId);
	//读出原来得字节
	ReadProcessMemory(hProcess, Address, &OldCode, sizeof(OldCode), &NumberOfBytesRead);
	//把CC写入要下断点得地址
	int bRet=WriteProcessMemory(hProcess,Address, &ExcptCode, sizeof(ExcptCode), &NumberOfBytesWrite);
	if(bRet==0)
	{
		pShow->ShowErroApi("CBreakPoint:SetInt3Break:WriteProcessMemory");
	}

	if(!LsOnce)
	{
		//如果是重复断点
		mReptIn3BreakMap.insert(std::pair<LPVOID, char>(Address, OldCode));
		
	}
	else
	{
		//如果不是
		mOnceIn3BreakMap.insert(std::pair<LPVOID, char>(Address, OldCode));
	}

	CloseHandle(hProcess);
}

char CBreakPoint::FindReptInt3Break(LPVOID address)
{

	for(auto i: mReptIn3BreakMap)
	{
		if(i.first== address)
		{
			return i.second;
		}
	}

	return 0xcc;
}

char CBreakPoint::FindOnceInt3Break(LPVOID address)
{
	for (auto i : mOnceIn3BreakMap)
	{
		if (i.first == address)
		{
			return i.second;
		}
	}

	return 0xcc;
}

bool CBreakPoint::DeleteInt3OnceBreak(LPVOID address)
{

	mOnceIn3BreakMap.erase(address);
	return true;
}

bool CBreakPoint::DeleteInt3ReptBreak(LPVOID address)
{
	mReptIn3BreakMap.erase(address);
	return true;
}

LPVOID CBreakPoint::FindAddressInOrdinal(int Ord)
{
	int n = 0;
	for (auto i : mReptIn3BreakMap)
	{
		if(Ord==n)
		{
			return i.first;
		}
	}
	
	return NULL;
}

void CBreakPoint::ShowInt3Break()
{
	printf("---------------------一般断点列表---------------------\r\n");
	printf("序号  地址     类型\r\n");
	int n = 0;
	for (auto i : mReptIn3BreakMap)
	{
		printf("%d   %08X  永久断点\r\n", n,i.first);
		n++;
	}

	for (auto i :mOnceIn3BreakMap)
	{
		printf("%d   %08X  一次性断点\r\n", n, i.first);
		n++;
	}
	printf("------------------------------------------------------\r\n");
}

tagMemoryBreakPointMsg * CBreakPoint::GetMemoryBreakPoint(LPVOID address)
{
	int nStart =((int)address) & 0xFFFFF000;

	for (auto i : mMemBreakMap)
	{
		if (i.first == (LPVOID)nStart)
		{
			for(auto k:*i.second)
			{
				if(k->StartAddress<= (DWORD)address&& (DWORD)address<= k->EndAddress)
				{
					return k;
				}
			}
		}
	}
	return nullptr;
}

std::list<tagMemoryBreakPointMsg*>* CBreakPoint::GetMemoryBreakPointlist(LPVOID address)
{
	for (auto i : mMemBreakMap)
	{
		if (i.first == address)
		{
			return i.second;
		}
	}
	return nullptr;
}

void CBreakPoint::SetHardBreakPoint(DEBUG_EVENT& Event,int DRN,LPVOID address, char nType, int nLength)
{
	//执行断点不能设置长度
	if(nType=='e')
	{
		nLength = 0;
	}

	HANDLE hThread = OpenThread(THREAD_ALL_ACCESS, FALSE, Event.dwThreadId);
	CONTEXT context;
	context.ContextFlags = CONTEXT_ALL;
	GetThreadContext(hThread, &context);
	DBG_REG7 DR7;
	
	memcpy((char*)&DR7, (char*)&context.Dr7, 4);

	/*	00：执行断点
		01：数据写入断点
		10：I / 0读写断点
		11：读写断点，读取指令不算*/

	int Type;
	switch (nType)
	{
	case 'e':
		Type = 0b00;
		break;
	case 'r':
		Type = 0b11;
		break;
	case 'w':
		Type = 0b01;
		break;


	}


	switch (DRN)
	{
	case 0:
		context.Dr0 = (DWORD)address;
		DR7.L0 = 1;
		DR7.LEN0 = nLength;
		DR7.RW0 = Type;
		break;
	case 1:
		context.Dr1 = (DWORD)address;
		DR7.L1 = 1;
		DR7.LEN1 = nLength;
		DR7.RW1 = Type;
		
		break;
	case 2:
		context.Dr2 = (DWORD)address;
		DR7.L2 = 1;
		DR7.LEN2 = nLength;
		DR7.RW2 = Type;
		break;
	case 3:
		context.Dr3 = (DWORD)address;
		DR7.L3 = 1;
		DR7.LEN3 = nLength;
		DR7.RW3 = Type;
		break;
	}

	memcpy((char*)&context.Dr7, (char*)&DR7, sizeof(DBG_REG7));
	

	SetThreadContext(hThread, &context);
	mHardBreakPoint[DRN].lpAddress = address;
	mHardBreakPoint[DRN].nType = nType;
	mHardBreakPoint[DRN].nLength = nLength;
	CloseHandle(hThread);


}

bool CBreakPoint::IsHardBreakPoint(DEBUG_EVENT& Event)
{
	HANDLE hThread = OpenThread(THREAD_ALL_ACCESS, FALSE, Event.dwThreadId);
	CONTEXT context;
	context.ContextFlags = CONTEXT_ALL;
	GetThreadContext(hThread, &context);

	DBG_REG6 DR6;

	memcpy((char*)&DR6, (char*)&context.Dr6, sizeof(DBG_REG6));
	CloseHandle(hThread);
	if(DR6.B0||DR6.B1||DR6.B2||DR6.B3)
	{
		return true;
	}
	return false;
}

void CBreakPoint::ShowHardBreakPoint()
{
	printf("---------------------硬件断点列表---------------------\r\n");
	printf("序号   地址    类型\r\n");
	for(int i=0;i<4;i++)
	{
		if(mHardBreakPoint[i].lpAddress!=NULL)
		{
			if (mHardBreakPoint[i].nType == 'e')
			{
				printf("%d    %08X  e\r\n", i, mHardBreakPoint[i].lpAddress);
			}
			else if (mHardBreakPoint[i].nType =='r')
			{
				printf("%d    %08X  rw\r\n", i, mHardBreakPoint[i].lpAddress);
			}
			else if (mHardBreakPoint[i].nType == 'w')
			{
				printf("%d    %08X  w\r\n", i, mHardBreakPoint[i].lpAddress);
			}
		}
		else
		{
			printf("%d    %08X  没使用\r\n", i, mHardBreakPoint[i].lpAddress);
		}
		
		
	}
	
	printf("------------------------------------------------------\r\n");
}

void CBreakPoint::DelHardBreakPoint(DEBUG_EVENT& Event,int Ord)
{
	HANDLE hThread = OpenThread(THREAD_ALL_ACCESS, FALSE, Event.dwThreadId);
	CONTEXT context;
	context.ContextFlags = CONTEXT_ALL;
	GetThreadContext(hThread, &context);
	DBG_REG7 DR7;

	memcpy((char*)&DR7, (char*)&context.Dr7, 4);
	switch (Ord)
	{
	case 0:
		context.Dr0 = (DWORD)0;
		DR7.L0 = 0;
		DR7.LEN0 = 0;
		DR7.RW0 = 0;
		break;
	case 1:
		context.Dr1 = (DWORD)0;
		DR7.L1 = 0;
		DR7.LEN1 = 0;
		DR7.RW1 = 0;

		break;
	case 2:
		context.Dr2 = (DWORD)0;
		DR7.L2 = 0;
		DR7.LEN2 = 0;
		DR7.RW2 = 0;
		break;
	case 3:
		context.Dr3 = (DWORD)0;
		DR7.L3 = 0;
		DR7.LEN3 = 0;
		DR7.RW3 = 0;
		break;
	}

	memcpy((char*)&context.Dr7, (char*)&DR7, sizeof(DBG_REG7));

	SetThreadContext(hThread, &context);
	
	mHardBreakPoint[Ord].lpAddress = 0;
	mHardBreakPoint[Ord].nType = 0;
	mHardBreakPoint[Ord].nLength = 0;
}

bool CBreakPoint::FindPage(LPVOID address)
{
	
	for (auto i : mMemBreakMap)
	{
		if (i.first == address)
		{
			return true;
		}
	}
	return false;
}


void CBreakPoint::SetMemBreakPoint(DEBUG_EVENT& Event, DWORD StartAddress, DWORD EndAddress, DWORD cType)
{
	//1.判断下的断点内存页是否有这个属性,能不能下断点先忽略
	/***********************************************************/




	/***********************************************************/

	//2.算StartAddress和EndAddress跨页没(我这里默认最大支持两个页，太多了也不实用，99%这个就够用)
	int nStart = StartAddress & 0xFFFFF000;
	int nSnd = EndAddress & 0xFFFFF000;
	DWORD OldProtect;
	if (nStart == nSnd)
	{
		//内存断点表里寻找是否修改过页属性
		bool bIsFind = FindPage((LPVOID)nStart);
		if (!bIsFind)
		{
			HANDLE hProcess = OpenProcess(THREAD_ALL_ACCESS, FALSE, Event.dwProcessId);
			BOOL bRet = VirtualProtectEx(hProcess, (LPVOID)nStart, 0x1000, PAGE_NOACCESS, &OldProtect);
			std::list<tagMemoryBreakPointMsg*>* pList = new std::list<tagMemoryBreakPointMsg*>;
			tagMemoryBreakPointMsg* pTag = new tagMemoryBreakPointMsg;
			pTag->StartAddress = StartAddress;
			pTag->EndAddress = EndAddress;
			pTag->Attribute = cType;
			pTag->OldProtect = OldProtect;
			pList->push_back(pTag);
			mMemBreakMap.insert(std::pair < LPVOID, std::list<tagMemoryBreakPointMsg*>*>((LPVOID)nStart, pList));
			CloseHandle(hProcess);
		}
		else
		{
			for (auto i : mMemBreakMap)
			{
				if (i.first == (LPVOID)nStart)
				{
					tagMemoryBreakPointMsg* pTag = new tagMemoryBreakPointMsg;
					pTag->StartAddress = StartAddress;
					pTag->EndAddress = EndAddress;
					pTag->Attribute = cType;
					pTag->OldProtect =i.second->front()->OldProtect;
					i.second->push_back(pTag);
				}
			}
		}
	}
	else
	{
		//跨页
		//内存断点表里寻找是否修改过页属性
		bool bIsFind = FindPage((LPVOID)nStart);
		if (!bIsFind)
		{
			HANDLE hProcess = OpenProcess(THREAD_ALL_ACCESS, FALSE, Event.dwProcessId);
			BOOL bRet = VirtualProtectEx(hProcess, (LPVOID)nStart, 0x1000, PAGE_NOACCESS, &OldProtect);
			std::list<tagMemoryBreakPointMsg*>* pList = new std::list<tagMemoryBreakPointMsg*>;
			tagMemoryBreakPointMsg* pTag = new tagMemoryBreakPointMsg;
			pTag->StartAddress = StartAddress;
			pTag->EndAddress = EndAddress;
			pTag->Attribute = cType;
			pTag->OldProtect = OldProtect;
			pList->push_back(pTag);
			mMemBreakMap.insert(std::pair < LPVOID, std::list<tagMemoryBreakPointMsg*>*>((LPVOID)nStart, pList));
			CloseHandle(hProcess);
		}
		else
		{
			for (auto i : mMemBreakMap)
			{
				if (i.first == (LPVOID)nStart)
				{
					tagMemoryBreakPointMsg* pTag = new tagMemoryBreakPointMsg;
					pTag->StartAddress = StartAddress;
					pTag->EndAddress = EndAddress;
					pTag->Attribute = cType;
					pTag->OldProtect = i.second->front()->OldProtect;
					i.second->push_back(pTag);
				}
			}
		}

		bIsFind=FindPage((LPVOID)nSnd);

		if (!bIsFind)
		{
			HANDLE hProcess = OpenProcess(THREAD_ALL_ACCESS, FALSE, Event.dwProcessId);
			BOOL bRet = VirtualProtectEx(hProcess, (LPVOID)nStart, 0x1000, PAGE_NOACCESS, &OldProtect);
			std::list<tagMemoryBreakPointMsg*>* pList = new std::list<tagMemoryBreakPointMsg*>;
			tagMemoryBreakPointMsg* pTag = new tagMemoryBreakPointMsg;
			pTag->StartAddress = StartAddress;
			pTag->EndAddress = EndAddress;
			pTag->Attribute = cType;
			pTag->OldProtect = OldProtect;
			pList->push_back(pTag);
			mMemBreakMap.insert(std::pair < LPVOID, std::list<tagMemoryBreakPointMsg*>*>((LPVOID)nStart, pList));
			CloseHandle(hProcess);
		}
		else
		{
			for (auto i : mMemBreakMap)
			{
				if (i.first == (LPVOID)nStart)
				{
					tagMemoryBreakPointMsg* pTag = new tagMemoryBreakPointMsg;
					pTag->StartAddress = StartAddress;
					pTag->EndAddress = EndAddress;
					pTag->Attribute = cType;
					pTag->OldProtect = i.second->front()->OldProtect;
					i.second->push_back(pTag);
				}
			}
		}

	}
}

void CBreakPoint::ShowALLMemBreakPoint()
{
	
	printf("---------------------内存断点ALL列表---------------------\r\n");
	printf("序号  起始地址   结束地址   类型\r\n");
	int n = 0;
		for (auto i : mMemBreakMap)
		{
			for (auto j : *i.second)
			{
				if(j->Attribute==WRITE)
				{
					printf("%d     %08X   %08X   写\r\n", n++, j->StartAddress, j->EndAddress);
				}
				else
				{
					printf("%d     %08X   %08X   访问\r\n", n++, j->StartAddress, j->EndAddress);
				}
				
			}		
		}
		printf("------------------------------------------------------\r\n");
}

void CBreakPoint::ShowPageMemBreakPoint()
{
	printf("---------------------内存断点PAGE列表---------------------\r\n");
	printf("分页地址   断点个数    旧属性     新属性\r\n");
	if(mMemBreakMap.size()>0)
	{
		for (auto i : mMemBreakMap)
		{
			printf("%08X   %d          %08X   %08X\r\n", i.first, i.second->size(), i.second->front()->OldProtect, PAGE_NOACCESS);
		}
		
	}
	printf("------------------------------------------------------\r\n");
	
}

void CBreakPoint::DelMemBreakPoint(int Ord, DEBUG_EVENT& Event)
{
	int n = 0;
	tagMemoryBreakPointMsg* pTag = NULL;
	DWORD OldProtect = 0;
	for (auto i : mMemBreakMap)
	{
		for (auto j : *i.second)
		{
			
			if(Ord==n)
			{
				//当前页没断点了修改页属性
				if(i.second->size()==1)
				{
					
					HANDLE hProcess = OpenProcess(THREAD_ALL_ACCESS, FALSE, Event.dwProcessId);
					BOOL bRet = VirtualProtectEx(hProcess, (LPVOID)i.first, 0x1000, j->OldProtect, &OldProtect);
					CloseHandle(hProcess);
				}
				//tagMemoryBreakPointMsg* pTag = j;
				i.second->remove(j);
				return;	
			}
		}
		
	}



}
