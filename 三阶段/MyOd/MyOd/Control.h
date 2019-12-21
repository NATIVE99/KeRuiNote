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
	
	//��ѭ��
private:
	void MainLoop();
private:
	//��������
	void Command();
	
	//��ʾ��ࡢ�Ĵ������ڴ�Ⱥ���
private:
	void ShowAsm();
	void ShowAsm(LPVOID lpAddress);
	void ShowAsm(int nNum);
	void ShowRegist();
	void ShowMemory(LPVOID lpAddress);
	
	//Event������
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

	//�쳣�ϵ㴦����
private:
	void OnExceptionBreakpoint();
	void OnExceptionSingleStep();
	void OnExceptionAccessViolation();
	
	//�ϲ�������ظ��ϵ�
private:
	void AltOldCodeEip(LPVOID lpAddress, HANDLE hThread, HANDLE hProcess, CONTEXT& context);
	void AltOldCode(LPVOID address);
	
	//Ѱ��eip�ǲ���call����
private:
	int FindCall();

	//�ű�����
private:
	void RecordCommand(char* szbuf);
	void ExportScript();
	void ImportScript();
	
	//trace����
private:
	void Trace(LPVOID start, LPVOID end, char* strName);
	BOOL GetProcessModule();
private:
	//�ļ�·��
	char* mFilePath;
	//�Ƿ����쳣
	DWORD dwContinueStatus;

	

	//CreateProcess��Ҫ�Ĳ���
	STARTUPINFO mSi ;
	PROCESS_INFORMATION mPi ;
	DEBUG_EVENT mEvent;
	//���� ����� �ϵ���
	CShowErro* pShowErro;
	CDisAssmble* pDis;
	CBreakPoint* pBreak;
	
	//��Ҫ�޸ĵ�EIP���ϲ�������ظ��ϵ㣬��֮ǰ�޸����ٸ�Ϊ0XCC
	LPVOID mlpAltEip;

	//״̬��־
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
	//��¼ģ��
	std::map<char*, tagModuleMs> mMoudel;
	FILE * mFile;
	DWORD mdwEnd;
	DWORD mdwStart;
	tagModuleMs ModuleSize;

};

