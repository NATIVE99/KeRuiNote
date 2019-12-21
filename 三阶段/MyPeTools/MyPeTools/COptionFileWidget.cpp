#include "COptionFileWidget.h"

COptionFileWidget::COptionFileWidget()
{
	this->setColumnCount(5);
	QStringList header;
	header << QStringLiteral("成员") << QStringLiteral("偏移量") << QStringLiteral("大小") << QStringLiteral("值") << QStringLiteral("内涵");
	this->setHorizontalHeaderLabels(header);
}

COptionFileWidget::~COptionFileWidget()
{
}

void COptionFileWidget::SetMsg(IMAGE_OPTIONAL_HEADER32& Op32, IMAGE_DOS_HEADER& dos)
{
	const char* FileName32[][2] =
	{
		"WORD","Magic",
		"BYTE" ,"MajorLinkerVersion",
		"BYTE","MinorLinkerVersion",
		"DWORD",   "SizeOfCode",
		"DWORD",   "SizeOfInitializedData",
		"DWORD",   "SizeOfUninitializedData",
		"DWORD",   "AddressOfEntryPoint",
		"DWORD",   "BaseOfCode",
		"DWORD",   "BaseOfData",
		"DWORD",   "ImageBase",
		"DWORD",   "SectionAlignment",
		"DWORD",   "FileAlignment",
		"WORD","MajorOperatingSystemVersion",
		"WORD","MinorOperatingSystemVersion",
		"WORD","MajorImageVersion",
		"WORD","MinorImageVersion",
		"WORD","MajorSubsystemVersion",
		"WORD","MinorSubsystemVersion",
		"DWORD",   "Win32VersionValue",
		"DWORD",   "SizeOfImage",
		"DWORD",   "SizeOfHeaders",
		"DWORD" ,  "CheckSum",
		"WORD",   "Subsystem",
		"WORD",    "DllCharacteristics",
		"DWORD",   "SizeOfStackReserve",
		"DWORD",   "SizeOfStackCommit",
		"DWORD",   "SizeOfHeapReserve",
		"DWORD",   "SizeOfHeapCommit",
		"DWORD",   "LoaderFlags",
		"DWORD",   "NumberOfRvaAndSizes"
	};

	int offset = dos.e_lfanew + 4 + sizeof(IMAGE_FILE_HEADER);
	int structoffset = 0;
	for (int i = 0; i < sizeof(FileName32) / sizeof(char*[2]); i++)
	{
		this->insertRow(i);
		this->setItem(i, 0, new QTableWidgetItem(FileName32[i][1]));
		this->setItem(i, 2, new QTableWidgetItem(FileName32[i][0]));

		QString stroffset = QString("%1").arg(offset, 8, 16, QLatin1Char('0'));
		this->setItem(i, 1, new QTableWidgetItem(stroffset.toUpper()));

		if (strcmp(FileName32[i][0], "DWORD") == 0)
		{
			DWORD dw = *(DWORD*)((char*)&Op32 + structoffset);
			QString strValue = QString("%1").arg(dw, 8, 16, QLatin1Char('0'));
			this->setItem(i, 3, new QTableWidgetItem(strValue.toUpper()));
			offset += 4;
			structoffset += 4;
		}
		else if (strcmp(FileName32[i][0], "WORD") == 0)
		{
			WORD dw = *(WORD*)((char*)&Op32 + structoffset);
			QString strValue = QString("%1").arg(dw, 4, 16, QLatin1Char('0'));
			this->setItem(i, 3, new QTableWidgetItem(strValue.toUpper()));
			offset += 2;
			structoffset += 2;
		}
		else
		{
			BYTE dw = *(BYTE*)((char*)&Op32 + structoffset);
			QString strValue = QString("%1").arg(dw, 2, 16, QLatin1Char('0'));
			this->setItem(i, 3, new QTableWidgetItem(strValue.toUpper()));
			offset += 1;
			structoffset += 1;
		}
	}

}
