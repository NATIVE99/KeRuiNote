#include "caddressconvertwidget.h"
#include "ui_caddressconvertwidget.h"

CAddressConvertWidget::CAddressConvertWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CAddressConvertWidget)
{
    ui->setupUi(this);
}

CAddressConvertWidget::~CAddressConvertWidget()
{
    delete ui;
}

void CAddressConvertWidget::slot_AddressConvert()
{
	bool ok;
	QString strRva=ui->lineEdit_2->text();
	int nRVA = strRva.toInt(&ok, 16);
	int FA=RVA2FA(nRVA);
	
	int nVa = nRVA + m_ImageBase;
	QString hexVa = QString::number(nVa, 16);
	QString hexstr = QString::number(FA, 16);

	ui->lineEdit_3->setText(hexstr);
	ui->lineEdit->setText(hexVa);

}

void CAddressConvertWidget::SetMsg(IMAGE_FILE_HEADER &FileHeader, PIMAGE_SECTION_HEADER pSections, DWORD ImageBase)
{
	 m_FileHeader= FileHeader;
	 m_pSections= pSections;
	 m_ImageBase = ImageBase;
}

//RVA转FOA
int CAddressConvertWidget::RVA2FA(int nRva)
{

	int nPosSe = -1;
	//判断在哪个节里面
	for (int i = 0; i < m_FileHeader.NumberOfSections; i++)
	{
		int nStart = (int)(m_pSections + i)->VirtualAddress;
		int nEnd = (int)((m_pSections + i)->VirtualAddress + (int)((m_pSections + i)->Misc.VirtualSize));
		if (nRva >= nStart&& nRva <= nEnd)
		{
			nPosSe = i;
			break;
		}
	}
	DWORD FOA = 0;
	if (nPosSe != -1)
	{
		//减去ImageBase减去VirtualOffset得到RVA相对节的偏移再加上文件的节的偏移得到FOA
		FOA = nRva - (m_pSections + nPosSe)->VirtualAddress + (m_pSections + nPosSe)->PointerToRawData;
	}

	return FOA;
}