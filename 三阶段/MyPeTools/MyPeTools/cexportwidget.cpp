#include "cexportwidget.h"
#include "ui_cexportwidget.h"
#include <QHeaderView>
CExportWidget::CExportWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CExportWidget)
{
    ui->setupUi(this);

	QHeaderView* headerView = ui->tableWidget->horizontalHeader();
	headerView->setSectionResizeMode(QHeaderView::Stretch);

	ui->tableWidget->setColumnCount(3);
	ui->tableWidget->setRowCount(11);
	QStringList header;
	header  << QStringLiteral("offset") << QStringLiteral("Size") << QStringLiteral("Value");
	ui->tableWidget->setHorizontalHeaderLabels(header);
	
	
	QStringList header2;
	header2 << QStringLiteral("Characteristics") << QStringLiteral("TimeDateStamp") << QStringLiteral("MajorVersion") << QStringLiteral("MinorVersion")
		<< QStringLiteral("Name") << QStringLiteral("Base")<<  QStringLiteral("NumberOfFunctions")
		<<  QStringLiteral("NumberOfNames") << QStringLiteral("AddressOfFunctions")<< QStringLiteral("AddressOfNames")
		<< QStringLiteral("AddressOfNameOrdinals");
	ui->tableWidget->setVerticalHeaderLabels(header2);


	ui->tableWidget_2->setColumnCount(5);
	QStringList header3;
	header3 << QStringLiteral("Ordinal") << QStringLiteral("Function RVA") << QStringLiteral("NameOrdian") << QStringLiteral("NameRVA")
		<< QStringLiteral("Name");

	ui->tableWidget_2->setHorizontalHeaderLabels(header3);

	
}

CExportWidget::~CExportWidget()
{
    delete ui;
}

void CExportWidget::SetMsg(IMAGE_FILE_HEADER &FileHeader, PIMAGE_SECTION_HEADER pSections, QString m_Path, DWORD ExportDirAddress)
{
	m_FileHeader = FileHeader;
	m_pSections = pSections;
	m_File.setFileName(m_Path);
	if (m_File.open(QIODevice::ReadWrite))
	{
		m_dwVirtualAddress = ExportDirAddress;
		DWORD FA = RVA2FA(m_dwVirtualAddress);
		m_File.seek(FA);
		QDataStream in(&m_File);

		
		IMAGE_EXPORT_DIRECTORY  ExportDir = {0};

		in.readRawData((char*)&ExportDir, sizeof(IMAGE_EXPORT_DIRECTORY));

		/*********************************************************************************************************************************************/
			ui->tableWidget->setItem(0, 2, new QTableWidgetItem(QString("%1").arg(ExportDir.Characteristics, 8, 16, QLatin1Char('0')).toUpper()));
			ui->tableWidget->setItem(1, 2, new QTableWidgetItem(QString("%1").arg(ExportDir.TimeDateStamp, 8, 16, QLatin1Char('0')).toUpper()));
			ui->tableWidget->setItem(2, 2, new QTableWidgetItem(QString("%1").arg(ExportDir.MajorVersion, 8, 16, QLatin1Char('0')).toUpper()));
			ui->tableWidget->setItem(3, 2, new QTableWidgetItem(QString("%1").arg(ExportDir.MinorVersion, 8, 16, QLatin1Char('0')).toUpper()));
			ui->tableWidget->setItem(4, 2, new QTableWidgetItem(QString("%1").arg(ExportDir.Name, 8, 16, QLatin1Char('0')).toUpper()));
			ui->tableWidget->setItem(5, 2, new QTableWidgetItem(QString("%1").arg(ExportDir.Base, 8, 16, QLatin1Char('0')).toUpper()));
			ui->tableWidget->setItem(6, 2, new QTableWidgetItem(QString("%1").arg(ExportDir.NumberOfFunctions, 8, 16, QLatin1Char('0')).toUpper()));
			ui->tableWidget->setItem(7, 2, new QTableWidgetItem(QString("%1").arg(ExportDir.NumberOfNames, 8, 16, QLatin1Char('0')).toUpper()));
			ui->tableWidget->setItem(8, 2, new QTableWidgetItem(QString("%1").arg(ExportDir.AddressOfFunctions, 8, 16, QLatin1Char('0')).toUpper()));
			ui->tableWidget->setItem(9, 2, new QTableWidgetItem(QString("%1").arg(ExportDir.AddressOfNames, 8, 16, QLatin1Char('0')).toUpper()));
			ui->tableWidget->setItem(10, 2, new QTableWidgetItem(QString("%1").arg(ExportDir.AddressOfNameOrdinals, 8, 16, QLatin1Char('0')).toUpper()));

			int i = 0;
			for( i=0;i<2;i++)
			{
				
				ui->tableWidget->setItem(i, 0, new QTableWidgetItem(QString("%1").arg(FA, 8, 16, QLatin1Char('0')).toUpper()));
				ui->tableWidget->setItem(i, 1, new QTableWidgetItem(QString("DWORD")));
				FA = FA + 4;
			}
			

			for( i;i<4;i++)
			{
				
				ui->tableWidget->setItem(i, 0, new QTableWidgetItem(QString("%1").arg(FA, 8, 16, QLatin1Char('0')).toUpper()));
				ui->tableWidget->setItem(i, 1, new QTableWidgetItem(QString("WORD")));
				FA = FA + 2;
			}

			for(i;i<11;i++)
			{
				
				ui->tableWidget->setItem(i, 0, new QTableWidgetItem(QString("%1").arg(FA, 8, 16, QLatin1Char('0')).toUpper()));
				ui->tableWidget->setItem(i, 1, new QTableWidgetItem(QString("DWORD")));
				FA = FA + 4;
			}
	
			/*********************************************************************************************************************************************/

			DWORD   AddressOfFunctions= ExportDir.AddressOfFunctions;
			DWORD   AddressOfNames= ExportDir.AddressOfNames;
			DWORD   AddressOfNameOrdinals= ExportDir.AddressOfNameOrdinals;
			AddressOfFunctions=RVA2FA(AddressOfFunctions);
			AddressOfNames = RVA2FA(AddressOfNames);
			AddressOfNameOrdinals = RVA2FA(AddressOfNameOrdinals);

			int Ord = 0;
			DWORD dwName=0;
			char szNameBuff[32] = { 0 };
			for(int i=0;i<ExportDir.NumberOfFunctions;i++)
			{
				ui->tableWidget_2->insertRow(i);
				m_File.seek(AddressOfNameOrdinals);
				in.readRawData((char*)&Ord, 2);
				m_File.seek(AddressOfNames);
				in.readRawData((char*)&dwName, 4);

				ui->tableWidget_2->setItem(i, 3, new QTableWidgetItem(QString("%1").arg(dwName, 8, 16, QLatin1Char('0')).toUpper()));
				if(dwName!=0)
				{	
				
					dwName=RVA2FA(dwName);
					m_File.seek(dwName);
					in.readRawData(szNameBuff, 32);
				}
				
				DWORD FunFlag = AddressOfFunctions + sizeof(DWORD)*Ord;
				m_File.seek(FunFlag);
				DWORD FunAddress = 0;
				in.readRawData((char*)&FunAddress, 4);


		
				ui->tableWidget_2->setItem(i, 2, new QTableWidgetItem(QString("%1").arg(Ord, 4, 16, QLatin1Char('0')).toUpper()));
				ui->tableWidget_2->setItem(i, 0, new QTableWidgetItem(QString("%1").arg(i, 8, 16, QLatin1Char('0')).toUpper()));
				ui->tableWidget_2->setItem(i, 1, new QTableWidgetItem(QString("%1").arg(FunAddress, 8, 16, QLatin1Char('0')).toUpper()));
				ui->tableWidget_2->setItem(i, 4, new QTableWidgetItem(QString(szNameBuff)));

				AddressOfNameOrdinals += 2;
				AddressOfNames += 4;

			
			}




	}
}

//RVA转FOA
DWORD CExportWidget::RVA2FA(DWORD nRva)
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
