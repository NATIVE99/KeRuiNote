#include "crelocationwidget.h"
#include "ui_crelocationwidget.h"

CRelocationWidget::CRelocationWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CRelocationWidget)
{
    ui->setupUi(this);

	ui->tableWidget->setColumnCount(4);
	QStringList header;
	header << QStringLiteral("RVA") << QStringLiteral("Size of Block") << QStringLiteral("Items")<< QStringLiteral("VitruallAddress");
	ui->tableWidget->setHorizontalHeaderLabels(header);


	ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
	ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection); //设置只能选择一行，不能多行选中
	ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);   //设置每行内容不可更改

	ui->tableWidget_2->setColumnCount(3);
	QStringList header2;
	header2 << QStringLiteral("Item") << QStringLiteral("RVA") << QStringLiteral("Type");
	ui->tableWidget_2->setHorizontalHeaderLabels(header2);

	ui->tableWidget_2->setSelectionBehavior(QAbstractItemView::SelectRows);
	ui->tableWidget_2->setSelectionMode(QAbstractItemView::SingleSelection); //设置只能选择一行，不能多行选中
	ui->tableWidget_2->setEditTriggers(QAbstractItemView::NoEditTriggers);   //设置每行内容不可更改



}

CRelocationWidget::~CRelocationWidget()
{
    delete ui;
}

void CRelocationWidget::SetMsg(IMAGE_FILE_HEADER &FileHeader, PIMAGE_SECTION_HEADER pSections, QString m_Path, DWORD ExportDirAddress)
{
	m_FileHeader = FileHeader;
	m_pSections = pSections;
	m_File.setFileName(m_Path);
	if (m_File.open(QIODevice::ReadWrite))
	{

		m_dwVirtualAddress = ExportDirAddress;
		DWORD FA = RVA2FA(m_dwVirtualAddress);
	
		QDataStream in(&m_File);

		IMAGE_BASE_RELOCATION RelocaDir = {0};
		int nNum = 0;
		int i = 0;
		do 
		{
			//nNum
			m_File.seek(FA+ nNum);
			in.readRawData((char*)&RelocaDir, sizeof(IMAGE_BASE_RELOCATION));
			if(RelocaDir.SizeOfBlock==0)
			{
				break;
			}
			ui->tableWidget->insertRow(i);
			int nItem = (RelocaDir.SizeOfBlock - sizeof(IMAGE_BASE_RELOCATION)) >> 1;
			ui->tableWidget->setItem(i, 0, new QTableWidgetItem(QString("%1").arg(RelocaDir.VirtualAddress, 8, 16, QLatin1Char('0')).toUpper()));
			ui->tableWidget->setItem(i, 1, new QTableWidgetItem(QString("%1").arg(RelocaDir.SizeOfBlock, 8, 16, QLatin1Char('0')).toUpper()));
			ui->tableWidget->setItem(i, 2, new QTableWidgetItem(QString("%1").arg(nItem)));
			ui->tableWidget->setItem(i, 3, new QTableWidgetItem(QString("%1").arg(FA + nNum, 8, 16, QLatin1Char('0')).toUpper()));
			i++;
			nNum = RelocaDir.SizeOfBlock+ nNum;
		} while (RelocaDir.SizeOfBlock!=0);


		connect(ui->tableWidget, SIGNAL(itemDoubleClicked(QTableWidgetItem *)), this, SLOT(slot_FindReloc(QTableWidgetItem*)));



	}
}

DWORD CRelocationWidget::RVA2FA(DWORD nRva)
{
	int nPosSe = -1;
	//判断在哪个节里面
	for (int i = 0; i < m_FileHeader.NumberOfSections; i++)
	{
		int nStart = (int)(m_pSections + i)->VirtualAddress;
		int nEnd = (int)((m_pSections + i)->VirtualAddress + (int)((m_pSections + i)->Misc.VirtualSize));
		if (nRva >= nStart&& nRva<=nEnd)
		{
			nPosSe = i;
			break;
		}
	}
	DWORD FOA=0;
	if(nPosSe!=-1)
	{
		//减去ImageBase减去VirtualOffset得到RVA相对节的偏移再加上文件的节的偏移得到FOA
		FOA = nRva - (m_pSections + nPosSe)->VirtualAddress + (m_pSections + nPosSe)->PointerToRawData;
	}
	
	return FOA;

}

void CRelocationWidget::DeleteAllRow()
{	
		if (ui->tableWidget_2->rowCount() > 0)
		{
			ui->tableWidget_2->clearContents();

			int iLen = ui->tableWidget_2->rowCount();
			for (int i = 0; i < iLen; i++)
			{
				ui->tableWidget_2->removeRow(i);
			}
			ui->tableWidget_2->setRowCount(0);
		}
}

void CRelocationWidget::slot_FindReloc(QTableWidgetItem *item)
{
	DeleteAllRow();
	int nRow = item->row();
	QTableWidgetItem* pRec= ui->tableWidget->item(nRow, 3);
	QTableWidgetItem* pNum = ui->tableWidget->item(nRow, 2);
	QTableWidgetItem* pOffset = ui->tableWidget->item(nRow, 0);
	bool ok;
	DWORD RecFileOffset = pRec->text().toInt(&ok, 16);
	DWORD dwNum= pNum->text().toInt(&ok, 16);
	DWORD dwRVA = pOffset->text().toInt(&ok, 16);
	QDataStream in(&m_File);

	tagReloc tagReloclation;
	RecFileOffset += sizeof(IMAGE_BASE_RELOCATION);
	for(int i=0;i<dwNum;i++)
	{
		m_File.seek(RecFileOffset);
		in.readRawData((char*)&tagReloclation, sizeof(tagReloc));

		ui->tableWidget_2->insertRow(i);
		ui->tableWidget_2->setItem(i, 0, new QTableWidgetItem(QString("%1").arg(tagReloclation.u.wAll, 8, 16, QLatin1Char('0')).toUpper()));
		ui->tableWidget_2->setItem(i, 1, new QTableWidgetItem(QString("%1").arg(dwRVA +tagReloclation.u.MyRec.w0ffset, 8, 16, QLatin1Char('0')).toUpper()));
		if(tagReloclation.u.MyRec.wAttribute==0x3)
		{

			ui->tableWidget_2->setItem(i, 2, new QTableWidgetItem(QString("HIGHLOW")));
		}
		else if(tagReloclation.u.MyRec.wAttribute == 0)
		{
			ui->tableWidget_2->setItem(i, 2, new QTableWidgetItem(QString("ABSOLUTE ")));
		}
		RecFileOffset += sizeof(tagReloc);
	}

}
