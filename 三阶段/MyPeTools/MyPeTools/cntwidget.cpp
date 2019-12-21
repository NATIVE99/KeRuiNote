#include "cntwidget.h"

CNtWidget::CNtWidget()

{
	this->setColumnCount(5);
	QStringList header;
	header << QStringLiteral("成员") << QStringLiteral("偏移量") << QStringLiteral("大小") << QStringLiteral("值") << QStringLiteral("内涵");
	this->setHorizontalHeaderLabels(header);
}

CNtWidget::~CNtWidget()
{
}

void CNtWidget::SetMsg(IMAGE_DOS_HEADER& dos,DWORD dwSig)
{
	int offset = dos.e_lfanew;

	int i = 0;
	this->insertRow(i);

	this->setItem(i, 0, new QTableWidgetItem("Signature"));

	QString stroffset = QString("%1").arg(offset, 8, 16, QLatin1Char('0'));
	this->setItem(i, 1, new QTableWidgetItem(stroffset.toUpper()));

	this->setItem(i, 2, new QTableWidgetItem("DWORD"));

	QString strValue = QString("%1").arg(dwSig, 4, 16, QLatin1Char('0'));
	this->setItem(i, 3, new QTableWidgetItem(strValue.toUpper()));

	this->setItem(i, 4, new QTableWidgetItem("PE"));
}


