#include "cdoswidget.h"

CDosWidget::CDosWidget()
{
	this->setColumnCount(5);
	QStringList header;
	header << QStringLiteral("成员") << QStringLiteral("偏移量") << QStringLiteral("大小") << QStringLiteral("值") << QStringLiteral("内涵");
	this->setHorizontalHeaderLabels(header);
}

CDosWidget::~CDosWidget()
{
}



void CDosWidget::SetMsg(IMAGE_DOS_HEADER& dos)
{


	IMAGE_DOS_HEADER* PDos = NULL;
	const char* DosName[] = { "e_magic","e_cblp","e_cp","e_crlc","e_cparhdr","e_minalloc","e_maxalloc","e_ss","e_sp","e_csum","e_ip",
		"e_cs","e_lfarlc","e_ovno","e_res[0]","e_res[1]","e_res[2]","e_res[3]","e_oemid","e_oeminfo","e_res2[0]","e_res2[1]","e_res2[2]",
		"e_res2[3]","e_res2[4]","e_res2[5]","e_res2[6]","e_res2[7]","e_res2[8]","e_res2[9]" };
	int nRow = 0;
	int offset = 0;
	for (int i = 0; i < sizeof(DosName) / sizeof(char*); i++)
	{

		this->insertRow(i);
		this->setItem(i, 0, new QTableWidgetItem(DosName[i]));
		QString stroffset = QString("%1").arg(offset, 8, 16, QLatin1Char('0'));
		this->setItem(i, 1, new QTableWidgetItem(stroffset.toUpper()));
		this->setItem(i, 2, new QTableWidgetItem("WORD"));
		WORD dw = *(((WORD*)&dos) + i);
		QString strValue = QString("%1").arg(dw, 4, 16, QLatin1Char('0'));
		this->setItem(i, 3, new QTableWidgetItem(strValue.toUpper()));
		offset += 2;
		nRow = i;
	}


	offset += 2;

	this->insertRow(nRow + 1);
	this->setItem(nRow + 1, 0, new QTableWidgetItem("e_lfanew"));
	QString stroffset = QString("%1").arg(offset, 8, 16, QLatin1Char('0'));
	this->setItem(nRow + 1, 1, new QTableWidgetItem(stroffset.toUpper()));
	this->setItem(nRow + 1, 2, new QTableWidgetItem("DWORD"));

	QString strValue = QString("%1").arg(dos.e_lfanew, 4, 16, QLatin1Char('0'));
	this->setItem(nRow + 1, 3, new QTableWidgetItem(strValue.toUpper()));
}
