#include <windows.h>
#include <compressapi.h>
//#define TEST 
HMODULE GetKernel32Base();
HMODULE GetImageBase();
BOOL ImportProc(HINSTANCE hInstance, PIMAGE_IMPORT_DESCRIPTOR lpImport);
int __cdecl mymemcmp(const void * buf1, const void * buf2, size_t count);


FARPROC WINAPI MyGetProcAddress(
	__in  HMODULE hModule,
	__in  LPCSTR lpProcName);

typedef HMODULE

(WINAPI *pfnLoadLibrary)(
	_In_ LPCSTR lpLibFileName
);

typedef
BOOL
(WINAPI* pfnDeCreateCompressor)(
	_In_ DWORD Algorithm,
	_In_opt_ PCOMPRESS_ALLOCATION_ROUTINES AllocationRoutines,
	_Out_ PCOMPRESSOR_HANDLE CompressorHandle
	);

typedef
BOOL
(WINAPI* pfnDecompress)(
	_In_ DECOMPRESSOR_HANDLE DecompressorHandle,
	_In_reads_bytes_opt_(CompressedDataSize) LPCVOID CompressedData,
	_In_ SIZE_T CompressedDataSize,
	_Out_writes_bytes_opt_(UncompressedBufferSize) PVOID UncompressedBuffer,
	_In_ SIZE_T UncompressedBufferSize,
	_Out_opt_ PSIZE_T UncompressedDataSize
	);

#ifdef TEST
typedef int
(WINAPI*pfnMessageBoxA)
(
	_In_opt_ HWND hWnd,
	_In_opt_ LPCSTR lpText,
	_In_opt_ LPCSTR lpCaption,
	_In_ UINT uType);
#endif
int shellcode()
{
#ifdef TEST
	HMODULE hKernel32 = GetKernel32Base();
	//获取loadlibarary
	char szLoadLibraryA[] = { 'L','o','a','d','L','i','b','r','a','r','y','A', '\0' };
	pfnLoadLibrary MyLoadlibrary = (pfnLoadLibrary)MyGetProcAddress(hKernel32, szLoadLibraryA);
	char szUser32[] = { 'u','s','e','r','3','2', '\0' };
	char szMessageBoxA[] = { 'M','e','s','s','a','g','e','B','o','x','A', '\0' };

	HMODULE hUser32 = MyLoadlibrary(szUser32);
	pfnMessageBoxA MyMessageBox = (pfnMessageBoxA)MyGetProcAddress(hUser32, szMessageBoxA);
	MyMessageBox(0, 0, 0, 0);

#endif // TEST
	HMODULE hKernel32 = GetKernel32Base();
	//获取loadlibarary
	char szLoadLibraryA[] = { 'L','o','a','d','L','i','b','r','a','r','y','A', '\0' };
	pfnLoadLibrary MyLoadlibrary = (pfnLoadLibrary)MyGetProcAddress(hKernel32, szLoadLibraryA);
	
	//获取解压缩api
	char szCabinet[] = { 'C','a','b','i','n','e','t', '\0' };
	char szCreateCompressor[] = { 'C','r','e','a','t','e','D','e','c','o','m','p','r','e','s','s','o','r', '\0' };
	char szDecompress[] = { 'D','e','c','o','m','p','r','e','s','s', '\0' };


	HMODULE hModCabinet = MyLoadlibrary(szCabinet);
	pfnDeCreateCompressor MyCreateDecompressor = (pfnDeCreateCompressor)MyGetProcAddress(hModCabinet, szCreateCompressor);
	pfnDecompress MyDecompress = (pfnDecompress)MyGetProcAddress(hModCabinet, szDecompress);

	//解压缩
	//1.读取pe头信息，定位到压缩代码节
	HMODULE hImageBase = GetImageBase();
	PIMAGE_DOS_HEADER pDosHdr = (PIMAGE_DOS_HEADER)hImageBase;
	PIMAGE_NT_HEADERS pNtHdr = (PIMAGE_NT_HEADERS)((char*)hImageBase + pDosHdr->e_lfanew);
	PIMAGE_SECTION_HEADER pSecHdrs = (PIMAGE_SECTION_HEADER)
		((char*)&pNtHdr->OptionalHeader + pNtHdr->FileHeader.SizeOfOptionalHeader);

	//压缩代码节内存位置
	DWORD dwOffset = pSecHdrs[1].VirtualAddress + (DWORD)hImageBase;

	//2.解压数据

	int* pDecompressCodeSize = NULL;//解压缩代码大小+4
	pDecompressCodeSize = (int*)dwOffset;

	char* pCode = NULL;//解压缩代码位置
	pCode = (char*)(dwOffset + 4);

	//压缩数据头部信息
	struct DataInfo
	{
		int m_nVirtualSize; //解压缩后数据的大小
		int m_nVirtualAddress; //解压缩后数据的地址
		int m_nCompressSize; //压缩数据的大小
		int m_nEntryPoint;//OEP
		int m_nSectionNum;
	};
	
	DataInfo* pDataSec = (DataInfo*)(dwOffset + *pDecompressCodeSize);
	int nSectionNum = pDataSec->m_nSectionNum;
	int nOep = pDataSec->m_nEntryPoint + (DWORD)hImageBase;
	//DataInfo* pData = new DataInfo[nSectionNum];


	COMPRESSOR_HANDLE hDecompressor = NULL;
	BOOL bSuccess = MyCreateDecompressor(
		COMPRESS_ALGORITHM_XPRESS_HUFF, //  Compression Algorithm
		NULL,                           //  Optional allocation routine
		&hDecompressor);                 //  Handle

	int nOffset = dwOffset + *pDecompressCodeSize;//第一个节头部信息位置
	char* pComCode = NULL;
	pDataSec = NULL;
	for(int i=0;i<nSectionNum;i++)
	{
		pDataSec = (DataInfo*)nOffset;
		pComCode = (char*)(nOffset + sizeof(DataInfo));

		//解压缩
		DWORD dwDecompressedBufferSize = 0;
		bSuccess = MyDecompress(
			hDecompressor,                //  Compressor Handle
			pComCode,            //  Compressed data
			pDataSec->m_nCompressSize,               //  Compressed data size
			(LPVOID)(pDataSec->m_nVirtualAddress+ (DWORD)hImageBase),                        //  Buffer set to NULL
			pDataSec->m_nVirtualSize,                           //  Buffer size set to 0
			&dwDecompressedBufferSize);    //  Decompressed Data size

		nOffset = nOffset + sizeof(DataInfo) + pDataSec->m_nCompressSize;	
	}
	
	//修IAT表
	PIMAGE_DATA_DIRECTORY pDataDirectory = &pNtHdr->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT];
	IMAGE_IMPORT_DESCRIPTOR *lpImport =
		(IMAGE_IMPORT_DESCRIPTOR*)(pDataDirectory->VirtualAddress + (DWORD)hImageBase);
	ImportProc(hImageBase, lpImport);

	//修重定位

	//修TLS


	
	__asm jmp nOep;

	return 0;
}


//获取Kernel32基址
HMODULE GetKernel32Base()
{
	HMODULE hKer32 = NULL;
	__asm
	{
		mov eax, fs:[0x18]; //teb
		mov eax, [eax + 0x30]; //peb
		mov eax, [eax + 0x0C]; //_PEB_LDR_DATA
		mov eax, [eax + 0x0C]; //_LIST_ENTRY, 主模块
		mov eax, [eax]; //ntdll
		mov eax, [eax]; //kernel32
		mov eax, dword ptr[eax + 0x18]; //kernel32基址
		mov hKer32, eax
	}

	return hKer32;
}

HMODULE GetImageBase()
{
	HMODULE hImageBase = NULL;
	__asm
	{
		mov eax, fs:[0x18]; //teb
		mov eax, [eax + 0x30]; //peb
		mov eax, [eax + 0x08]; //ImageBaseAddress
		mov hImageBase, eax
	}

	return hImageBase;
}
FARPROC WINAPI MyGetProcAddress(
	__in  HMODULE hModule,
	__in  LPCSTR lpProcName
)
{
	//通过hModule找到导出表的位置
	PIMAGE_DOS_HEADER pDOSHeader = (PIMAGE_DOS_HEADER)hModule;
	PIMAGE_NT_HEADERS pNTHeader = (PIMAGE_NT_HEADERS)((DWORD)hModule + pDOSHeader->e_lfanew);
	DWORD dwExportTableAddr = pNTHeader->OptionalHeader.DataDirectory[0].VirtualAddress + (DWORD)hModule;
	PIMAGE_EXPORT_DIRECTORY pExportTable = (PIMAGE_EXPORT_DIRECTORY)dwExportTableAddr;

	FARPROC farRet = 0;
	//因为地址空间低64K不能访问.所以名字小于64K的为序号导出
	if ((DWORD)lpProcName <= 0xFFFF)
	{
		DWORD dwNumberOfFunctions = (WORD)lpProcName - pExportTable->Base;

		//大于NumberOfFunctions
		if (dwNumberOfFunctions + 1 > pExportTable->NumberOfFunctions)
		{
			farRet = 0;
		}
		else
		{
			farRet = (FARPROC)(((DWORD*)(pExportTable->AddressOfFunctions + DWORD(hModule)))[dwNumberOfFunctions] + DWORD(hModule));
		}
	}
	else
	{
		//名称导出

		//获得lpProcName的字符串长度
		DWORD dwlpProcNameCount = 0;
		char* pName = (char*)lpProcName;
		while (*pName != '\0')
		{
			dwlpProcNameCount++;
			pName++;
		}

		//遍历名称表, 找到名称表的下标
		size_t i = 0;
		for (; i < pExportTable->NumberOfNames; i++)
		{
			//获得第i项导出名称长度
			char* pExportName = (char*)(*((DWORD*)(pExportTable->AddressOfNames + DWORD(hModule)) + i) + DWORD(hModule));
			char* pExportNameCMP = pExportName;
			DWORD dwExportNameCount = 0;
			while (*pExportNameCMP != '\0')
			{
				dwExportNameCount++;
				pExportNameCMP++;
			}

			//如果长度相等, 比较每个字符串
			if (dwlpProcNameCount == dwExportNameCount)
			{
				size_t j = 0;
				for (; j < dwExportNameCount; j++)
				{
					if (lpProcName[j] != pExportName[j])
					{
						break;
					}
				}
				if (j == dwExportNameCount)
				{
					//找到了对应函数
					//找到序数表对应下标
					WORD wNameOrdinals = *(WORD*)((pExportTable->AddressOfNameOrdinals + DWORD(hModule)) + i * sizeof(WORD));


					//在地址表中找到函数地址
					farRet = (FARPROC)(((DWORD*)(pExportTable->AddressOfFunctions + DWORD(hModule)))[wNameOrdinals] + DWORD(hModule));
					break;
				}
			}
		}
		if (i == pExportTable->NumberOfNames)
		{
			farRet = 0;
		}
	}
	return farRet;
}

BOOL ImportProc(HINSTANCE hInstance, PIMAGE_IMPORT_DESCRIPTOR lpImport)
{

	

	HMODULE hKernel32 = GetKernel32Base();
	//获取loadlibarary
	char szLoadLibraryA[] = { 'L','o','a','d','L','i','b','r','a','r','y','A', '\0' };
	pfnLoadLibrary MyLoadlibrary = (pfnLoadLibrary)MyGetProcAddress(hKernel32, szLoadLibraryA);


	IMAGE_IMPORT_DESCRIPTOR ZeroImport = { 0 };
	while (mymemcmp(&ZeroImport, lpImport, sizeof ZeroImport) != 0)
	{
		IMAGE_THUNK_DATA *pIAT = (IMAGE_THUNK_DATA *)lpImport->FirstThunk;
		pIAT = (IMAGE_THUNK_DATA*)((int)pIAT + (int)hInstance);
		if (pIAT->u1.Function == NULL)
		{
			lpImport++;
			continue;
		}

		IMAGE_THUNK_DATA *pThunk = (IMAGE_THUNK_DATA *)lpImport->OriginalFirstThunk;
		if (pThunk == NULL)
		{
			pThunk = (IMAGE_THUNK_DATA *)lpImport->FirstThunk;
		}
		pThunk = (IMAGE_THUNK_DATA*)((int)pThunk + (int)hInstance);

		LPSTR lpLibName = (LPSTR)lpImport->Name;
		lpLibName = (LPSTR)((int)lpLibName + (int)hInstance);
		HMODULE hMod = MyLoadlibrary(lpLibName);
		// check it

		int i = 0;
		while (pThunk->u1.Ordinal != NULL)
		{
			LPSTR lpApiName = NULL;
			if (IMAGE_SNAP_BY_ORDINAL32(pThunk->u1.Ordinal)) // is name
			{
				lpApiName = (LPSTR)IMAGE_ORDINAL32(pThunk->u1.Ordinal);
			}
			else // is ordinal
			{
				IMAGE_IMPORT_BY_NAME *pName = (IMAGE_IMPORT_BY_NAME*)pThunk->u1.AddressOfData;
				pName = (IMAGE_IMPORT_BY_NAME*)((int)pName + (int)hInstance);
				lpApiName = (LPSTR)pName->Name;
			}

			DWORD pApi = (DWORD)MyGetProcAddress(hMod, lpApiName);
			// check it

			pIAT[i].u1.Ordinal = pApi;
			pThunk++;
			i++;
		}


		lpImport++;
	}


	return FALSE;
}

int __cdecl mymemcmp(
	const void * buf1,
	const void * buf2,
	size_t count
)
{
	if (!count)
		return(0);

	while (--count && *(char *)buf1 == *(char *)buf2) {
		buf1 = (char *)buf1 + 1;
		buf2 = (char *)buf2 + 1;
	}

	return(*((unsigned char *)buf1) - *((unsigned char *)buf2));
}
