#pragma once
#include "DisAssmble.h"
#include "BreakPoint.h"
#include <iostream>
#include <string>
#include <Tlhelp32.h>
typedef struct _tagModuleMs
{
	DWORD dwStart;
	DWORD dwSize;

}tagModuleMs;
typedef struct _tagEflag
{
	DWORD CF : 1;
	DWORD	 : 1;
	DWORD PF : 1;
	DWORD    : 1;
    DWORD AF : 1;
	DWORD    : 1;
	DWORD ZF : 1;
	DWORD SF : 1;
	DWORD TF : 1;
	DWORD IF : 1;
	DWORD DF : 1;
	DWORD OF : 1;
	DWORD    : 20;
}tagEflag;
class CControl
{
public:
	CControl();
	~CControl();
	
	bool Start(char* strPath);
	
	//主循环
private:
	void MainLoop();
private:
	//输入命令
	void Command();
	
	//显示汇编、寄存器、内存等函数
private:
	void ShowAsm();
	void ShowAsm(LPVOID lpAddress);
	void ShowAsm(int nNum);
	void ShowRegist();
	void ShowMemory(LPVOID lpAddress);
	
	//Event处理函数
private:
	DWORD OnException();
	DWORD OnUnLoadDll();
	DWORD OnOutputDebugString();
	DWORD OnLoadDll();
	DWORD OnCreateProcess();
	DWORD OnCreateThread();
	DWORD OnExitProcess();
	DWORD OnExitThread();

private:
	CONTEXT GetContext(HANDLE& hThread, HANDLE& hProcess);
	void	AltMemoryValue(LPVOID lpAddress,char Code);

	//异常断点处理函数
private:
	void OnExceptionBreakpoint();
	void OnExceptionSingleStep();
	void OnExceptionAccessViolation();
	
	//断步配合下重复断点
private:
	void AltOldCodeEip(LPVOID lpAddress, HANDLE hThread, HANDLE hProcess, CONTEXT& context);
	void AltOldCode(LPVOID address);
	
	//寻找eip是不是call函数
private:
	int FindCall();

	//脚本功能
private:
	void RecordCommand(char* szbuf);
	void ExportScript();
	void ImportScript();
	
	//trace功能
private:
	void Trace(LPVOID start, LPVOID end, char* strName);
	BOOL GetProcessModule();
private:
	//文件路径
	char* mFilePath;
	//是否处理异常
	DWORD dwContinueStatus;

	

	//CreateProcess需要的参数
	STARTUPINFO mSi ;
	PROCESS_INFORMATION mPi ;
	DEBUG_EVENT mEvent;
	//错误 反汇编 断点类
	CShowErro* pShowErro;
	CDisAssmble* pDis;
	CBreakPoint* pBreak;
	
	//需要修改的EIP，断步配合下重复断点，把之前修复的再改为0XCC
	LPVOID mlpAltEip;

	//状态标志
	bool mIsMemory;
	bool mIsInt3;
	bool mIsStep;
	bool mIsSystem;
	bool mIsEnterPoint;
	bool mIsMemBreakDeal;
	bool mIsScrip;
	bool mIsTrace;
	bool mIsContinue;

	DWORD mdwflag;
	DWORD  mdwAddress;

	std::list<char*> mRecordList;
	std::list<char*> mCommandList;

	/*************************/
	//trace
	//记录模块
	std::map<char*, tagModuleMs> mMoudel;
	FILE * mFile;
	DWORD mdwEnd;
	DWORD mdwStart;
	tagModuleMs ModuleSize;

};

