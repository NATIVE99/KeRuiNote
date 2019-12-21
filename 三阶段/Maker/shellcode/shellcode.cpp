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
	//��ȡloadlibarary
	char szLoadLibraryA[] = { 'L','o','a','d','L','i','b','r','a','r','y','A', '\0' };
	pfnLoadLibrary MyLoadlibrary = (pfnLoadLibrary)MyGetProcAddress(hKernel32, szLoadLibraryA);
	char szUser32[] = { 'u','s','e','r','3','2', '\0' };
	char szMessageBoxA[] = { 'M','e','s','s','a','g','e','B','o','x','A', '\0' };

	HMODULE hUser32 = MyLoadlibrary(szUser32);
	pfnMessageBoxA MyMessageBox = (pfnMessageBoxA)MyGetProcAddress(hUser32, szMessageBoxA);
	MyMessageBox(0, 0, 0, 0);

#endif // TEST
	HMODULE hKernel32 = GetKernel32Base();
	//��ȡloadlibarary
	char szLoadLibraryA[] = { 'L','o','a','d','L','i','b','r','a','r','y','A', '\0' };
	pfnLoadLibrary MyLoadlibrary = (pfnLoadLibrary)MyGetProcAddress(hKernel32, szLoadLibraryA);
	
	//��ȡ��ѹ��api
	char szCabinet[] = { 'C','a','b','i','n','e','t', '\0' };
	char szCreateCompressor[] = { 'C','r','e','a','t','e','D','e','c','o','m','p','r','e','s','s','o','r', '\0' };
	char szDecompress[] = { 'D','e','c','o','m','p','r','e','s','s', '\0' };


	HMODULE hModCabinet = MyLoadlibrary(szCabinet);
	pfnDeCreateCompressor MyCreateDecompressor = (pfnDeCreateCompressor)MyGetProcAddress(hModCabinet, szCreateCompressor);
	pfnDecompress MyDecompress = (pfnDecompress)MyGetProcAddress(hModCabinet, szDecompress);

	//��ѹ��
	//1.��ȡpeͷ��Ϣ����λ��ѹ�������
	HMODULE hImageBase = GetImageBase();
	PIMAGE_DOS_HEADER pDosHdr = (PIMAGE_DOS_HEADER)hImageBase;
	PIMAGE_NT_HEADERS pNtHdr = (PIMAGE_NT_HEADERS)((char*)hImageBase + pDosHdr->e_lfanew);
	PIMAGE_SECTION_HEADER pSecHdrs = (PIMAGE_SECTION_HEADER)
		((char*)&pNtHdr->OptionalHeader + pNtHdr->FileHeader.SizeOfOptionalHeader);

	//ѹ��������ڴ�λ��
	DWORD dwOffset = pSecHdrs[1].VirtualAddress + (DWORD)hImageBase;

	//2.��ѹ����

	int* pDecompressCodeSize = NULL;//��ѹ�������С+4
	pDecompressCodeSize = (int*)dwOffset;

	char* pCode = NULL;//��ѹ������λ��
	pCode = (char*)(dwOffset + 4);

	//ѹ������ͷ����Ϣ
	struct DataInfo
	{
		int m_nVirtualSize; //��ѹ�������ݵĴ�С
		int m_nVirtualAddress; //��ѹ�������ݵĵ�ַ
		int m_nCompressSize; //ѹ�����ݵĴ�С
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

	int nOffset = dwOffset + *pDecompressCodeSize;//��һ����ͷ����Ϣλ��
	char* pComCode = NULL;
	pDataSec = NULL;
	for(int i=0;i<nSectionNum;i++)
	{
		pDataSec = (DataInfo*)nOffset;
		pComCode = (char*)(nOffset + sizeof(DataInfo));

		//��ѹ��
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
	
	//��IAT��
	PIMAGE_DATA_DIRECTORY pDataDirectory = &pNtHdr->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT];
	IMAGE_IMPORT_DESCRIPTOR *lpImport =
		(IMAGE_IMPORT_DESCRIPTOR*)(pDataDirectory->VirtualAddress + (DWORD)hImageBase);
	ImportProc(hImageBase, lpImport);

	//���ض�λ

	//��TLS


	
	__asm jmp nOep;

	return 0;
}


//��ȡKernel32��ַ
HMODULE GetKernel32Base()
{
	HMODULE hKer32 = NULL;
	__asm
	{
		mov eax, fs:[0x18]; //teb
		mov eax, [eax + 0x30]; //peb
		mov eax, [eax + 0x0C]; //_PEB_LDR_DATA
		mov eax, [eax + 0x0C]; //_LIST_ENTRY, ��ģ��
		mov eax, [eax]; //ntdll
		mov eax, [eax]; //kernel32
		mov eax, dword ptr[eax + 0x18]; //kernel32��ַ
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
	//ͨ��hModule�ҵ��������λ��
	PIMAGE_DOS_HEADER pDOSHeader = (PIMAGE_DOS_HEADER)hModule;
	PIMAGE_NT_HEADERS pNTHeader = (PIMAGE_NT_HEADERS)((DWORD)hModule + pDOSHeader->e_lfanew);
	DWORD dwExportTableAddr = pNTHeader->OptionalHeader.DataDirectory[0].VirtualAddress + (DWORD)hModule;
	PIMAGE_EXPORT_DIRECTORY pExportTable = (PIMAGE_EXPORT_DIRECTORY)dwExportTableAddr;

	FARPROC farRet = 0;
	//��Ϊ��ַ�ռ��64K���ܷ���.��������С��64K��Ϊ��ŵ���
	if ((DWORD)lpProcName <= 0xFFFF)
	{
		DWORD dwNumberOfFunctions = (WORD)lpProcName - pExportTable->Base;

		//����NumberOfFunctions
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
		//���Ƶ���

		//���lpProcName���ַ�������
		DWORD dwlpProcNameCount = 0;
		char* pName = (char*)lpProcName;
		while (*pName != '\0')
		{
			dwlpProcNameCount++;
			pName++;
		}

		//�������Ʊ�, �ҵ����Ʊ���±�
		size_t i = 0;
		for (; i < pExportTable->NumberOfNames; i++)
		{
			//��õ�i������Ƴ���
			char* pExportName = (char*)(*((DWORD*)(pExportTable->AddressOfNames + DWORD(hModule)) + i) + DWORD(hModule));
			char* pExportNameCMP = pExportName;
			DWORD dwExportNameCount = 0;
			while (*pExportNameCMP != '\0')
			{
				dwExportNameCount++;
				pExportNameCMP++;
			}

			//����������, �Ƚ�ÿ���ַ���
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
					//�ҵ��˶�Ӧ����
					//�ҵ��������Ӧ�±�
					WORD wNameOrdinals = *(WORD*)((pExportTable->AddressOfNameOrdinals + DWORD(hModule)) + i * sizeof(WORD));


					//�ڵ�ַ�����ҵ�������ַ
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
	//��ȡloadlibarary
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
