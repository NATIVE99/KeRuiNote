#include "ctlswidget.h"

//typedef struct _IMAGE_TLS_DIRECTORY32 {
//	DWORD   StartAddressOfRawData;
//	DWORD   EndAddressOfRawData;
//	DWORD   AddressOfIndex;             // PDWORD
//	DWORD   AddressOfCallBacks;         // PIMAGE_TLS_CALLBACK *
//	DWORD   SizeOfZeroFill;
//	union {
//		DWORD Characteristics;
//		struct {
//			DWORD Reserved0 : 20;
//			DWORD Alignment : 4;
//			DWORD Reserved1 : 8;
//		} DUMMYSTRUCTNAME;
//	} DUMMYUNIONNAME;
//
//} IMAGE_TLS_DIRECTORY32;
CTlsWidget::CTlsWidget()
{
	this->setColumnCount(3);
	this->setRowCount(5);
	QStringList header;
	header << QStringLiteral("Offset") << QStringLiteral("Size") << QStringLiteral("Value");
	this->setHorizontalHeaderLabels(header);


	QStringList header2;
	header2 << QStringLiteral("StartAddressOfRawData") << QStringLiteral("EndAddressOfRawData") << QStringLiteral("AddressOfIndex") << QStringLiteral("AddressOfCallBacks")
		<< QStringLiteral("Characteristics");

	this->setVerticalHeaderLabels(header2);
}

CTlsWidget::~CTlsWidget()
{

}


void CTlsWidget::SetMsg(IMAGE_TLS_DIRECTORY& Tls,DWORD dwOffset)
{
	DWORD Offset = dwOffset;

	for(int i=0;i<5;i++)
	{
		this->setItem(i, 0, new QTableWidgetItem(QString("%1").arg(Offset, 8, 16, QLatin1Char('0')).toUpper()));
		this->setItem(i, 1, new QTableWidgetItem(QString("DWORD")));
		Offset += 4;
	}

	this->setItem(0, 2, new QTableWidgetItem(QString("%1").arg(Tls.StartAddressOfRawData, 8, 16, QLatin1Char('0')).toUpper()));
	this->setItem(1, 2, new QTableWidgetItem(QString("%1").arg(Tls.EndAddressOfRawData, 8, 16, QLatin1Char('0')).toUpper()));
	this->setItem(2, 2, new QTableWidgetItem(QString("%1").arg(Tls.AddressOfIndex, 8, 16, QLatin1Char('0')).toUpper()));
	this->setItem(3, 2, new QTableWidgetItem(QString("%1").arg(Tls.AddressOfCallBacks, 8, 16, QLatin1Char('0')).toUpper()));
	this->setItem(4, 2, new QTableWidgetItem(QString("%1").arg(Tls.Characteristics, 8, 16, QLatin1Char('0')).toUpper()));


}
