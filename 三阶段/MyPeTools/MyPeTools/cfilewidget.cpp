#include "cfilewidget.h"

CFileWidget::CFileWidget()
{
	this->setColumnCount(5);
	QStringList header;
	header << QStringLiteral("成员") << QStringLiteral("偏移量") << QStringLiteral("大小") << QStringLiteral("值") << QStringLiteral("内涵");
	this->setHorizontalHeaderLabels(header);
}

CFileWidget::~CFileWidget()
{
}

void CFileWidget::SetMsg(IMAGE_DOS_HEADER& dos, IMAGE_FILE_HEADER& tagFileHead)
{


	const char* FileName[7][2] =
	{
		"Machine","WORD",
		"NumberOfSections","WORD",
		"TimeDateStamp","DWORD",
		"PointerToSymbolTable","DWORD",
		"NumberOfSymbols","DWORD",
		"SizeOfOptionalHeader","WORD",
		"Characteristics","WORD"
	};

	int offset = dos.e_lfanew + 4;
	int structoffset = 0;
	for (int i = 0; i < 7; i++)
	{
		this->insertRow(i);
		this->setItem(i, 0, new QTableWidgetItem(FileName[i][0]));

		QString stroffset = QString("%1").arg(offset, 8, 16, QLatin1Char('0'));
		this->setItem(i, 1, new QTableWidgetItem(stroffset.toUpper()));

		this->setItem(i, 2, new QTableWidgetItem(FileName[i][1]));

		if (strcmp(FileName[i][1], "WORD") == 0)
		{

			WORD dw = *(WORD*)((char*)&tagFileHead + structoffset);
			QString strValue = QString("%1").arg(dw, 4, 16, QLatin1Char('0'));
			this->setItem(i, 3, new QTableWidgetItem(strValue.toUpper()));
			offset += 2;
			structoffset += 2;
		}
		else
		{

			DWORD dw = *(DWORD*)((char*)&tagFileHead + structoffset);
			QString strValue = QString("%1").arg(dw, 4, 16, QLatin1Char('0'));
			this->setItem(i, 3, new QTableWidgetItem(strValue.toUpper()));
			offset += 4;
			structoffset += 4;
		}
	}
}