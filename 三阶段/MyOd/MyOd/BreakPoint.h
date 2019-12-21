#pragma once
#include "ShowErro.h"
#include <map>
#include <list>
#define WRITE 1
#define READ 0

typedef struct tagMemoryBreakPointMsg
{
	DWORD StartAddress;
	DWORD EndAddress;
	DWORD Attribute;
	DWORD OldProtect;

}tagMemoryBreakPointMsg;


typedef struct _tagHardBreak
{
	LPVOID lpAddress;
	int	   nType;
	int nLength;
}tagHardBreak;

typedef struct _DBG_REG6
{
	/*
	//     �ϵ����б�־λ�����λ��DR0~3��ĳ���ϵ㱻���У�������쳣����ǰ����Ӧ
	// ��B0~3�ͻᱻ��Ϊ1��
	*/
	unsigned B0 : 1;  // Dr0�ϵ㴥����λ
	unsigned B1 : 1;  // Dr1�ϵ㴥����λ
	unsigned B2 : 1;  // Dr2�ϵ㴥����λ
	unsigned B3 : 1;  // Dr3�ϵ㴥����λ
					  /*
					  // �����ֶ�
					  */
	unsigned Reserve1 : 9;
	/*
	// ����״̬�ֶ�
	*/
	unsigned BD : 1;  // ���ƼĴ����������ϵ�󣬴�λ����Ϊ1
	unsigned BS : 1;  // �����쳣����������Ҫ��Ĵ���EFLAGS��TF����ʹ��
	unsigned BT : 1;  // ��λ��TSS��T��־����ʹ�ã����ڽ���CPU�����л��쳣
					  /*
					  // �����ֶ�
					  */
	unsigned Reserve2 : 16;
}DBG_REG6, *PDBG_REG6;
typedef struct _DBG_REG7
{
	/*
	// �ֲ��ϵ�(L0~3)��ȫ�ֶϵ�(G0~3)�ı��λ
	*/
	unsigned L0 : 1;  // ��Dr0����ĵ�ַ���� �ֲ��ϵ�
	unsigned G0 : 1;  // ��Dr0����ĵ�ַ���� ȫ�ֶϵ�
	unsigned L1 : 1;  // ��Dr1����ĵ�ַ���� �ֲ��ϵ�
	unsigned G1 : 1;  // ��Dr1����ĵ�ַ���� ȫ�ֶϵ�
	unsigned L2 : 1;  // ��Dr2����ĵ�ַ���� �ֲ��ϵ�
	unsigned G2 : 1;  // ��Dr2����ĵ�ַ���� ȫ�ֶϵ�
	unsigned L3 : 1;  // ��Dr3����ĵ�ַ���� �ֲ��ϵ�
	unsigned G3 : 1;  // ��Dr3����ĵ�ַ���� ȫ�ֶϵ�
					  /*
					  // �������á����ڽ���CPUƵ�ʣ��Է���׼ȷ���ϵ��쳣
					  */
	unsigned LE : 1;
	unsigned GE : 1;
	/*
	// �����ֶ�
	*/
	unsigned Reserve1 : 3;
	/*
	// �������ԼĴ�����־λ�������λΪ1������ָ���޸����ǼĴ���ʱ�ᴥ���쳣
	*/
	unsigned GD : 1;
	/*
	// �����ֶ�
	*/
	unsigned Reserve2 : 2;
	/*
	// ����Dr0~Dr3��ַ��ָ��λ�õĶϵ�����(RW0~3)��ϵ㳤��(LEN0~3)��״̬�������£�
	*/
	unsigned RW0 : 2;  // �趨Dr0ָ���ַ�Ķϵ�����
	unsigned LEN0 : 2;  // �趨Dr0ָ���ַ�Ķϵ㳤��
	unsigned RW1 : 2;  // �趨Dr1ָ���ַ�Ķϵ�����
	unsigned LEN1 : 2;  // �趨Dr1ָ���ַ�Ķϵ㳤��
	unsigned RW2 : 2;  // �趨Dr2ָ���ַ�Ķϵ�����
	unsigned LEN2 : 2;  // �趨Dr2ָ���ַ�Ķϵ㳤��
	unsigned RW3 : 2;  // �趨Dr3ָ���ַ�Ķϵ�����
	unsigned LEN3 : 2;  // �趨Dr3ָ���ַ�Ķϵ㳤��
}DBG_REG7, *PDBG_REG7;

class CBreakPoint
{
public:
	CBreakPoint();
	~CBreakPoint();
	
	//1.�����ϵ�
	//����TFλΪ1
	void SetSingleStep(DEBUG_EVENT& Event);

	//2.����Int3�ϵ�
	void SetInt3Break(DEBUG_EVENT& Event, LPVOID Address,bool LsOnce=false);
	char FindReptInt3Break(LPVOID address);
	char FindOnceInt3Break(LPVOID address);
	bool DeleteInt3OnceBreak(LPVOID address);
	bool DeleteInt3ReptBreak(LPVOID address);
	//������ҵ�ַ,û���ҵ����ؿգ��ҵ����ص�ַ
	LPVOID  FindAddressInOrdinal(int Ord);
	void ShowInt3Break();
	//3.�����ڴ�ϵ�
	//�ڴ�ϵ������һ���ֽڵģ������ֽڵģ�4���ֽڵ�
	//�������ֽڵĻ�������ҳ�����⣬����һ��ҳ��������ж���ϵ�

public:
	//�����ڴ��д�ϵ�
	void SetMemBreakPoint(DEBUG_EVENT& Event,DWORD StartAddress, DWORD EndAddress,DWORD cType);
	void ShowALLMemBreakPoint();
	void ShowPageMemBreakPoint();
	void DelMemBreakPoint(int Ord, DEBUG_EVENT& Event);
	tagMemoryBreakPointMsg* GetMemoryBreakPoint(LPVOID address);
	std::list<tagMemoryBreakPointMsg*>* GetMemoryBreakPointlist(LPVOID address);

public:
	//����Ӳ���ϵ�
	void SetHardBreakPoint(DEBUG_EVENT& Event,int DRN, LPVOID address, char nType,int nLength=1);
	bool IsHardBreakPoint(DEBUG_EVENT& Event);
	void ShowHardBreakPoint();
	void DelHardBreakPoint(DEBUG_EVENT& Event,int Ord);

private:
	bool FindPage(LPVOID address);

private:
	CShowErro* pShow;
	std::map<LPVOID, char> mReptIn3BreakMap;//�ظ��ϵ��¼
	std::map<LPVOID, char> mOnceIn3BreakMap;//�����ϵ��¼

	//��¼��ҳ-��ϵ�
	std::map<LPVOID, std::list<tagMemoryBreakPointMsg*>*> mMemBreakMap;


	//Ӳ���ϵ�����
	tagHardBreak mHardBreakPoint[4];

	
};

