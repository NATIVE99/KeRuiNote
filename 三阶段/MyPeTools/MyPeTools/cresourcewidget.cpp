#include "cresourcewidget.h"
#include "ui_cresourcewidget.h"
#include <QTextCodec>
CResourceWidget::CResourceWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CResourceWidget)
{
    ui->setupUi(this);


	//connect(ui->treeWidget, SIGNAL(itemDoubleClicked(QTreeWidgetItem *,int)), this, SLOT(slot_GetData(QTableWidgetItem*)));

	connect(ui->treeWidget, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(slot_GetData(QTreeWidgetItem*, int)));
}

CResourceWidget::~CResourceWidget()
{
    delete ui;
}

void CResourceWidget::SetMsg(IMAGE_FILE_HEADER &FileHeader, PIMAGE_SECTION_HEADER pSections, QString m_Path, DWORD ExportDirAddress)
{


	m_FileHeader = FileHeader;
	m_pSections = pSections;
	m_File.setFileName(m_Path);

	PIMAGE_RESOURCE_DIRECTORY  pResourceDird=new IMAGE_RESOURCE_DIRECTORY;
	PIMAGE_RESOURCE_DIRECTORY_ENTRY pResourceEntry = new IMAGE_RESOURCE_DIRECTORY_ENTRY;
	//IMAGE_RESOURCE_DATA_ENTRY
	if (m_File.open(QIODevice::ReadWrite))
	{

		m_dwVirtualAddress = ExportDirAddress;
		DWORD FA = RVA2FA(m_dwVirtualAddress);

		QDataStream in(&m_File);

		m_File.seek(FA);
		in.readRawData((char*)pResourceDird, sizeof(IMAGE_RESOURCE_DIRECTORY));
	

		QTreeWidgetItem* pRootItem = new QTreeWidgetItem();
		pRootItem->setText(0, "RESOURCE_DIRECTORY");
		pRootItem->setData(0,Qt::UserRole,(int)pResourceDird);
		ui->treeWidget->addTopLevelItem(pRootItem);

		//FA = FA + sizeof(IMAGE_RESOURCE_DIRECTORY);
		WCHAR szName[256] = { 0 };
		int n = pResourceDird->NumberOfIdEntries;
		n = n + pResourceDird->NumberOfNamedEntries;
		for(int i=0;i<n;i++)
		{
			m_File.seek(FA + sizeof(IMAGE_RESOURCE_DIRECTORY_ENTRY)*(i+2));
			pResourceEntry = new IMAGE_RESOURCE_DIRECTORY_ENTRY;
			in.readRawData((char*)pResourceEntry, sizeof(IMAGE_RESOURCE_DIRECTORY_ENTRY));
			

			QTreeWidgetItem* pSecondItem = new QTreeWidgetItem();
			if(pResourceEntry->NameIsString)
			{
				m_File.seek(pResourceEntry->NameOffset + FA+2);
				in.readRawData((char*)szName, 256);
				QString str1 = QString::fromWCharArray(szName);				
				pSecondItem->setText(0, QString("RESOURCE_DIRECTORY_ENTRY NAME:%1").arg(str1));
			}
			else
			{
				pSecondItem->setText(0,  QString("RESOURCE_DIRECTORY_ENTRY ID:%1").arg(pResourceEntry->Id));
			}
			
			pSecondItem->setData(0, Qt::UserRole, (int)pResourceEntry);
			pRootItem->addChild(pSecondItem);
		
		}

	}
}

DWORD CResourceWidget::RVA2FA(DWORD nRva)
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


void CResourceWidget::DeleteAllRow()
{
	if (ui->tableWidget->rowCount() > 0)
	{
		ui->tableWidget->clearContents();

		int iLen = ui->tableWidget->rowCount();
		for (int i = 0; i < iLen; i++)
		{
			ui->tableWidget->removeRow(i);
		}
		ui->tableWidget->setRowCount(0);
	}
}

void CResourceWidget::slot_GetData(QTreeWidgetItem *item, int column)
{
	PIMAGE_RESOURCE_DIRECTORY pData = (PIMAGE_RESOURCE_DIRECTORY)item->data(0,Qt::UserRole).toInt();

	pData->NumberOfIdEntries;
}

