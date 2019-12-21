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
	//     断点命中标志位，如果位于DR0~3的某个断点被命中，则进行异常处理前，对应
	// 的B0~3就会被置为1。
	*/
	unsigned B0 : 1;  // Dr0断点触发置位
	unsigned B1 : 1;  // Dr1断点触发置位
	unsigned B2 : 1;  // Dr2断点触发置位
	unsigned B3 : 1;  // Dr3断点触发置位
					  /*
					  // 保留字段
					  */
	unsigned Reserve1 : 9;
	/*
	// 其它状态字段
	*/
	unsigned BD : 1;  // 调制寄存器本身触发断点后，此位被置为1
	unsigned BS : 1;  // 单步异常被触发，需要与寄存器EFLAGS的TF联合使用
	unsigned BT : 1;  // 此位与TSS的T标志联合使用，用于接收CPU任务切换异常
					  /*
					  // 保留字段
					  */
	unsigned Reserve2 : 16;
}DBG_REG6, *PDBG_REG6;
typedef struct _DBG_REG7
{
	/*
	// 局部断点(L0~3)与全局断点(G0~3)的标记位
	*/
	unsigned L0 : 1;  // 对Dr0保存的地址启用 局部断点
	unsigned G0 : 1;  // 对Dr0保存的地址启用 全局断点
	unsigned L1 : 1;  // 对Dr1保存的地址启用 局部断点
	unsigned G1 : 1;  // 对Dr1保存的地址启用 全局断点
	unsigned L2 : 1;  // 对Dr2保存的地址启用 局部断点
	unsigned G2 : 1;  // 对Dr2保存的地址启用 全局断点
	unsigned L3 : 1;  // 对Dr3保存的地址启用 局部断点
	unsigned G3 : 1;  // 对Dr3保存的地址启用 全局断点
					  /*
					  // 【以弃用】用于降低CPU频率，以方便准确检测断点异常
					  */
	unsigned LE : 1;
	unsigned GE : 1;
	/*
	// 保留字段
	*/
	unsigned Reserve1 : 3;
	/*
	// 保护调试寄存器标志位，如果此位为1，则有指令修改条是寄存器时会触发异常
	*/
	unsigned GD : 1;
	/*
	// 保留字段
	*/
	unsigned Reserve2 : 2;
	/*
	// 保存Dr0~Dr3地址所指向位置的断点类型(RW0~3)与断点长度(LEN0~3)，状态描述如下：
	*/
	unsigned RW0 : 2;  // 设定Dr0指向地址的断点类型
	unsigned LEN0 : 2;  // 设定Dr0指向地址的断点长度
	unsigned RW1 : 2;  // 设定Dr1指向地址的断点类型
	unsigned LEN1 : 2;  // 设定Dr1指向地址的断点长度
	unsigned RW2 : 2;  // 设定Dr2指向地址的断点类型
	unsigned LEN2 : 2;  // 设定Dr2指向地址的断点长度
	unsigned RW3 : 2;  // 设定Dr3指向地址的断点类型
	unsigned LEN3 : 2;  // 设定Dr3指向地址的断点长度
}DBG_REG7, *PDBG_REG7;

class CBreakPoint
{
public:
	CBreakPoint();
	~CBreakPoint();
	
	//1.单步断点
	//设置TF位为1
	void SetSingleStep(DEBUG_EVENT& Event);

	//2.设置Int3断点
	void SetInt3Break(DEBUG_EVENT& Event, LPVOID Address,bool LsOnce=false);
	char FindReptInt3Break(LPVOID address);
	char FindOnceInt3Break(LPVOID address);
	bool DeleteInt3OnceBreak(LPVOID address);
	bool DeleteInt3ReptBreak(LPVOID address);
	//用序号找地址,没用找到反回空，找到反回地址
	LPVOID  FindAddressInOrdinal(int Ord);
	void ShowInt3Break();
	//3.设置内存断点
	//内存断点可以设一个字节的，二个字节的，4个字节的
	//设两个字节的会遇到跨页的问题，并且一个页里面可以有多个断点

public:
	//设置内存读写断点
	void SetMemBreakPoint(DEBUG_EVENT& Event,DWORD StartAddress, DWORD EndAddress,DWORD cType);
	void ShowALLMemBreakPoint();
	void ShowPageMemBreakPoint();
	void DelMemBreakPoint(int Ord, DEBUG_EVENT& Event);
	tagMemoryBreakPointMsg* GetMemoryBreakPoint(LPVOID address);
	std::list<tagMemoryBreakPointMsg*>* GetMemoryBreakPointlist(LPVOID address);

public:
	//设置硬件断点
	void SetHardBreakPoint(DEBUG_EVENT& Event,int DRN, LPVOID address, char nType,int nLength=1);
	bool IsHardBreakPoint(DEBUG_EVENT& Event);
	void ShowHardBreakPoint();
	void DelHardBreakPoint(DEBUG_EVENT& Event,int Ord);

private:
	bool FindPage(LPVOID address);

private:
	CShowErro* pShow;
	std::map<LPVOID, char> mReptIn3BreakMap;//重复断点记录
	std::map<LPVOID, char> mOnceIn3BreakMap;//单个断点记录

	//记录分页-多断点
	std::map<LPVOID, std::list<tagMemoryBreakPointMsg*>*> mMemBreakMap;


	//硬件断点数组
	tagHardBreak mHardBreakPoint[4];

	
};

