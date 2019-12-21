#include "cimportwidget.h"
#include "ui_cimportwidget.h"
#include <qdebug.h>
CImportwidget::CImportwidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CImportwidget)
{
    ui->setupUi(this);

	ui->tableWidget->setColumnCount(7);
	QStringList header;
	header << QStringLiteral("Module Name") <<QStringLiteral("Imports") << QStringLiteral("OFTs") << QStringLiteral("TimeDateStamp")
		<< QStringLiteral("ForWarderChain") << QStringLiteral("Name RVA") << QStringLiteral("IAT");
	ui->tableWidget->setHorizontalHeaderLabels(header);


	ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
	ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection); //设置只能选择一行，不能多行选中
	ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);   //设置每行内容不可更改

	ui->tableWidget_2->setColumnCount(4);
	QStringList header2;
	header2 << QStringLiteral("OFTs") << QStringLiteral("IAT") << QStringLiteral("Hint") << QStringLiteral("Name") ;
	ui->tableWidget_2->setHorizontalHeaderLabels(header2);

	ui->tableWidget_2->setSelectionBehavior(QAbstractItemView::SelectRows);
	ui->tableWidget_2->setSelectionMode(QAbstractItemView::SingleSelection); //设置只能选择一行，不能多行选中
	ui->tableWidget_2->setEditTriggers(QAbstractItemView::NoEditTriggers);   //设置每行内容不可更改
}

CImportwidget::~CImportwidget()
{
    delete ui;
}



void CImportwidget::SetMsg(IMAGE_FILE_HEADER &FileHeader, PIMAGE_SECTION_HEADER pSections, QString m_Path, DWORD ExportDirAddress)
{
	m_FileHeader = FileHeader;
	m_pSections = pSections;
	m_File.setFileName(m_Path);
	if(m_File.open(QIODevice::ReadWrite))
	{
	
		m_dwVirtualAddress = ExportDirAddress;
		DWORD FA = RVA2FA(m_dwVirtualAddress);
		m_File.seek(FA);
		QDataStream in(&m_File);

		IMAGE_IMPORT_DESCRIPTOR ImportDir;
		int nDllNum=0;
		char dllbuff[32] = { 0 };
		do{
			
		m_File.seek(FA+ nDllNum* sizeof(IMAGE_IMPORT_DESCRIPTOR));
		in.readRawData((char*)&ImportDir, sizeof(IMAGE_IMPORT_DESCRIPTOR));
		if(ImportDir.FirstThunk==NULL)
		{
			break;
		}
		//set dllname
		DWORD dwDllNameAddress = ImportDir.Name;
		dwDllNameAddress = RVA2FA(dwDllNameAddress);
		m_File.seek(dwDllNameAddress);
		in.readRawData(dllbuff, 32);
		ui->tableWidget->insertRow(nDllNum);

		ui->tableWidget->setItem(nDllNum, 0, new QTableWidgetItem(dllbuff));


		ui->tableWidget->setItem(nDllNum, 2, new QTableWidgetItem(QString("%1").arg(ImportDir.OriginalFirstThunk, 8, 16, QLatin1Char('0')).toUpper()));

		ui->tableWidget->setItem(nDllNum, 3, new QTableWidgetItem(QString("%1").arg(ImportDir.TimeDateStamp, 8, 16, QLatin1Char('0')).toUpper()));
		ui->tableWidget->setItem(nDllNum, 4, new QTableWidgetItem(QString("%1").arg(ImportDir.ForwarderChain, 8, 16, QLatin1Char('0')).toUpper()));
		ui->tableWidget->setItem(nDllNum, 5, new QTableWidgetItem(QString("%1").arg(ImportDir.Name, 8, 16, QLatin1Char('0')).toUpper()));
		ui->tableWidget->setItem(nDllNum, 6, new QTableWidgetItem(QString("%1").arg(ImportDir.FirstThunk, 8, 16, QLatin1Char('0')).toUpper()));


		nDllNum++;
		} while (ImportDir.FirstThunk != NULL);
		
		connect(ui->tableWidget, SIGNAL(itemDoubleClicked(QTableWidgetItem *)), this, SLOT(slot_FindFunction(QTableWidgetItem*)));


	}
	
}

//RVA转FOA
DWORD CImportwidget::RVA2FA(DWORD nRva)
{
	int nPosSe = 0;
	//判断在哪个节里面
	for (int i = 0; i < m_FileHeader.NumberOfSections; i++)
	{
		if (nRva > (int)((m_pSections + i)->VirtualAddress))
		{
			nPosSe = i;
		}
		else
		{
			break;
		}
	}
	//减去ImageBase减去VirtualOffset得到RVA相对节的偏移再加上文件的节的偏移得到FOA
	DWORD FOA = nRva - (m_pSections + nPosSe)->VirtualAddress + (m_pSections + nPosSe)->PointerToRawData;
	return FOA;
}

void CImportwidget::slot_FindFunction(QTableWidgetItem *item)
{
	DeleteAllRow();
	int nRow = item->row();
	QTableWidgetItem* pIAT=ui->tableWidget->item(nRow, 6);
	QTableWidgetItem* pFirstThunk= ui->tableWidget->item(nRow, 2);

	bool ok;
	DWORD dwIatFA = RVA2FA(pIAT->text().toInt(&ok, 16));
	DWORD dwFirstThunkFA = RVA2FA(pFirstThunk->text().toInt(&ok, 16));
	DWORD firstThunk=0;
	QDataStream in(&m_File);
	m_File.seek(dwFirstThunkFA);
	in.readRawData((char*)&firstThunk, 4);

	if(firstThunk !=0)
	{
		//m_File.seek(dwFirstThunkFA);

		

		DWORD dwIATContext = NULL;
		int nFunNum = 0;
		char szFunNameBuff[32] = { 0 };
		IMAGE_THUNK_DATA  thunkData;
		do {


			m_File.seek(dwFirstThunkFA + 4 * nFunNum);
			in.readRawData((char*)&thunkData, 4);
			if (thunkData.u1.Ordinal == 0)
			{
				break;
			}

			ui->tableWidget_2->insertRow(nFunNum);
			ui->tableWidget_2->setItem(nFunNum, 0, new QTableWidgetItem(QString("%1").arg(thunkData.u1.Ordinal, 8, 16, QLatin1Char('0')).toUpper()));
			if (IMAGE_SNAP_BY_ORDINAL32(thunkData.u1.Ordinal))
			{
				WORD nOrd = 0;
				memcpy(&nOrd, &thunkData, 2);
				ui->tableWidget_2->setItem(nFunNum, 3, new QTableWidgetItem((QString("ordinal:%1").arg(nOrd, 4, 16, QLatin1Char('0')).toUpper())));
			}
			else
			{
				//name
				DWORD dw = RVA2FA(thunkData.u1.AddressOfData);
				m_File.seek(dw);

				in.readRawData(szFunNameBuff, 32);

				ui->tableWidget_2->setItem(nFunNum, 3, new QTableWidgetItem((&szFunNameBuff[2])));

			}
			WORD Ord = 0;
			memcpy(&Ord, szFunNameBuff, 2);
			ui->tableWidget_2->setItem(nFunNum, 2, new QTableWidgetItem(QString("%1").arg(Ord, 4, 16, QLatin1Char('0')).toUpper()));

			//iat
			m_File.seek(dwIatFA + 4 * nFunNum);
			in.readRawData((char*)&dwIATContext, 4);
			ui->tableWidget_2->setItem(nFunNum, 1, new QTableWidgetItem(QString("%1").arg(dwIATContext, 4, 16, QLatin1Char('0')).toUpper()));


			nFunNum++;

		} while (thunkData.u1.Ordinal != 0);
		ui->tableWidget->setItem(nRow, 1, new QTableWidgetItem(QString("%1").arg(nFunNum).toUpper()));
	}
	else
	{

		DWORD dwIATContext = NULL;
		int nFunNum = 0;
		char szFunNameBuff[32] = { 0 };
		IMAGE_THUNK_DATA  thunkData;
		do {


			m_File.seek(dwIatFA + 4 * nFunNum);
			in.readRawData((char*)&thunkData, 4);
			if (thunkData.u1.Ordinal == 0)
			{
				break;
			}

			ui->tableWidget_2->insertRow(nFunNum);
			ui->tableWidget_2->setItem(nFunNum, 0, new QTableWidgetItem("N/A"));
			ui->tableWidget_2->setItem(nFunNum, 1, new QTableWidgetItem(QString("%1").arg(thunkData.u1.Ordinal, 8, 16, QLatin1Char('0')).toUpper()));
			if (IMAGE_SNAP_BY_ORDINAL32(thunkData.u1.Ordinal))
			{
				WORD nOrd = 0;
				memcpy(&nOrd, &thunkData, 2);
				ui->tableWidget_2->setItem(nFunNum, 3, new QTableWidgetItem((QString("ordinal:%1").arg(nOrd,4,16,QLatin1Char('0')).toUpper())));
			}
			else
			{
				//name
				DWORD dw = RVA2FA(thunkData.u1.AddressOfData);
				m_File.seek(dw);

				in.readRawData(szFunNameBuff, 32);

				ui->tableWidget_2->setItem(nFunNum, 3, new QTableWidgetItem((&szFunNameBuff[2])));

			}
			WORD Ord = 0;
			memcpy(&Ord, szFunNameBuff, 2);
			ui->tableWidget_2->setItem(nFunNum, 2, new QTableWidgetItem(QString("%1").arg(Ord, 4, 16, QLatin1Char('0')).toUpper()));

			//iat
			


			nFunNum++;

		} while (thunkData.u1.Ordinal != 0);
		ui->tableWidget->setItem(nRow, 1, new QTableWidgetItem(QString("%1").arg(nFunNum).toUpper()));



	}


}


void CImportwidget::DeleteAllRow()
{
	if(ui->tableWidget_2->rowCount()>0)
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