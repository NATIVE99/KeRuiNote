#include "mypetools.h"
#include <qdebug.h>
MyPeTools::MyPeTools(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	//set windows fixed
	setWindowFlags(windowFlags()&~Qt::WindowMaximizeButtonHint);
	setFixedSize(this->width(), this->height());
	
	//widget
	m_PreWidget = NULL;
	m_pDosWidet = new CDosWidget;
	m_PNtWidget = new CNtWidget;
	m_pFileWidget=new CFileWidget;
	m_pOpwidget=new COptionFileWidget;
	m_pSectinWidget= new COptionSectinWidget;
	m_pAddressConvertWidget = new CAddressConvertWidget;
	m_pImportWidget = new CImportwidget;
	m_pExportWidget = new CExportWidget;
	m_pRelocaWidget = new CRelocationWidget;
	m_pTlsWidget = new CTlsWidget;
	m_pResourseWidget = new CResourceWidget;

	//exe ICON
	QIcon icon;
	icon.addFile(tr(":/Resources/CFF Explorer.ico"));
	setWindowIcon(icon);
}

void MyPeTools::slot_start()
{
	
	ReadFile();
	SetTreeUi();

	
}

void MyPeTools::slot_SetWidegtUi(QTreeWidgetItem* item, int)
{
		
		QLayoutItem *child;
		while (child = ui.gridLayout->itemAt(0))
		{

			ui.gridLayout->takeAt(0);
			child->widget()->setParent(0);
		}
	


	if (item->text(0) == "DOS Head")
	{
		ui.gridLayout->addWidget(m_pDosWidet);	

		
	}
	else if (item->text(0) == "NT Head")
	{
		ui.gridLayout->addWidget(m_PNtWidget);

	}
	else if (item->text(0) == "File Head")
	{
		ui.gridLayout->addWidget(m_pFileWidget);

	}
	else if (item->text(0) == "Option Head")
	{
		ui.gridLayout->addWidget(m_pOpwidget);

	}
	else if (item->text(0) == "Section Head")
	{
		ui.gridLayout->addWidget(m_pSectinWidget);
	}
	else if(item->text(0) == "Address Convert")
	{
		ui.gridLayout->addWidget(m_pAddressConvertWidget);
	} 
	else if (item->text(0) == "ImportDirectory")
	{
		ui.gridLayout->addWidget(m_pImportWidget);
	}
	else if (item->text(0) == "ExportDirectory")
	{
		ui.gridLayout->addWidget(m_pExportWidget);
	}
	else if (item->text(0) == "RelocationDirectory")
	{
		ui.gridLayout->addWidget(m_pRelocaWidget);
	}
	else if (item->text(0) == "Tls")
	{
		ui.gridLayout->addWidget(m_pTlsWidget);
	}
	else if (item->text(0) == "Resourse")
	{
		ui.gridLayout->addWidget(m_pResourseWidget);
	}

}

void MyPeTools::ReadFile()
{
	if(GetPath())
	{
		m_File.setFileName(m_strPath);
		if(m_File.open(QIODevice::ReadWrite))
		{
			QDataStream in(&m_File);
			// read dos
			in.readRawData((char*)&m_dos, sizeof(IMAGE_DOS_HEADER));
			m_pDosWidet->SetMsg(m_dos);
			//read m_Signature
			m_File.seek(m_dos.e_lfanew);
			in.readRawData((char*)&m_Signature, 4);
			m_PNtWidget->SetMsg(m_dos, m_Signature);
			//read File_Head
			in.readRawData((char*)&m_FileHeader, sizeof(IMAGE_FILE_HEADER));
			m_pFileWidget->SetMsg(m_dos, m_FileHeader);
			//read option_file
			int nOptionSize = m_FileHeader.SizeOfOptionalHeader;
		
			//check 32-bit Or 64-bit
			int nCharacter = m_FileHeader.Characteristics;
			nCharacter &= 0x0100;
			if (nCharacter == 0x0100)
			{
				//32
				qDebug() << "32";
				in.readRawData((char*)&m_Op32, m_FileHeader.SizeOfOptionalHeader);
				m_Is32 = true;
			}
			else
			{
				//64
				qDebug() << "64";
				in.readRawData((char*)&m_Op64, m_FileHeader.SizeOfOptionalHeader);
			}
			m_pOpwidget->SetMsg(m_Op32, m_dos);
			//read section
			m_pSections = new IMAGE_SECTION_HEADER[m_FileHeader.NumberOfSections];
			in.readRawData((char*)m_pSections, sizeof(IMAGE_SECTION_HEADER)*m_FileHeader.NumberOfSections);
			m_pSectinWidget->SetMsg(m_pSections, m_FileHeader);

			m_pAddressConvertWidget->SetMsg(m_FileHeader, m_pSections,m_Op32.ImageBase);
			//import
			m_pImportWidget->SetMsg(m_FileHeader, m_pSections,m_strPath, m_Op32.DataDirectory[1].VirtualAddress);

			//export
			if(m_Op32.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress!=NULL)
			{
				m_pExportWidget->SetMsg(m_FileHeader, m_pSections, m_strPath, m_Op32.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress);
			}
		
			//Relocation
			if (m_Op32.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].VirtualAddress != NULL)
			{
				m_pRelocaWidget->SetMsg(m_FileHeader, m_pSections, m_strPath, m_Op32.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].VirtualAddress);
			}
			//Tls
			if(m_Op32.DataDirectory[IMAGE_DIRECTORY_ENTRY_TLS].VirtualAddress != NULL)
			{
				DWORD FA=m_pAddressConvertWidget->RVA2FA(m_Op32.DataDirectory[IMAGE_DIRECTORY_ENTRY_TLS].VirtualAddress);
				m_File.seek(FA);
				in.readRawData((char*)&m_Tls, sizeof(IMAGE_TLS_DIRECTORY32));
				m_pTlsWidget->SetMsg(m_Tls, FA);
			}

			//RESOURCE
			if (m_Op32.DataDirectory[IMAGE_DIRECTORY_ENTRY_RESOURCE].VirtualAddress != NULL)
			{
				m_pResourseWidget->SetMsg(m_FileHeader, m_pSections, m_strPath, m_Op32.DataDirectory[IMAGE_DIRECTORY_ENTRY_RESOURCE].VirtualAddress);
			}

			m_File.close();

		}
	}
}

bool MyPeTools::GetPath()
{
		m_strPath = ui.textEdit->toPlainText();
		if (m_strPath.size() < 1)
		{
			return false;
		}

		m_strPath.remove("file:///");
		m_strPath.replace("/", "\\");
		qDebug() << m_strPath;
		return true;
}

void MyPeTools::SetTreeUi()
{


	ui.treeWidget->clear();

	QString FileName = m_strPath.split("\\").last();
	QTreeWidgetItem* pRootItem = new QTreeWidgetItem();
	pRootItem->setText(0, FileName);
	pRootItem->setIcon(0, QApplication::style()->standardIcon(QStyle::SP_DesktopIcon));
	

	ui.treeWidget->addTopLevelItem(pRootItem);

	QTreeWidgetItem* pDosItem = new QTreeWidgetItem();
	pDosItem->setText(0, "DOS Head");
	pRootItem->addChild(pDosItem);
	pDosItem->setIcon(0, QApplication::style()->standardIcon(QStyle::SP_DirClosedIcon));

	QTreeWidgetItem* pNtItem = new QTreeWidgetItem();
	pNtItem->setText(0, "NT Head");
	pRootItem->addChild(pNtItem);

	pNtItem->setIcon(0, QApplication::style()->standardIcon(QStyle::SP_DirClosedIcon));

	QTreeWidgetItem* pFileItem = new QTreeWidgetItem();
	pFileItem->setText(0, "File Head");
	pNtItem->addChild(pFileItem);
	pFileItem->setIcon(0, QApplication::style()->standardIcon(QStyle::SP_DirClosedIcon));

	QTreeWidgetItem* pOpFileItem = new QTreeWidgetItem();
	pOpFileItem->setText(0, "Option Head");
	pNtItem->addChild(pOpFileItem);
	pOpFileItem->setIcon(0, QApplication::style()->standardIcon(QStyle::SP_DirClosedIcon));


	QTreeWidgetItem* pSectionItem = new QTreeWidgetItem();
	pSectionItem->setText(0, "Section Head");
	pRootItem->addChild(pSectionItem);
	pSectionItem->setIcon(0, QApplication::style()->standardIcon(QStyle::SP_DirClosedIcon));

	QTreeWidgetItem* pAddressConvertItem = new QTreeWidgetItem();
	pAddressConvertItem->setText(0, "Address Convert");
	pRootItem->addChild(pAddressConvertItem);
	pAddressConvertItem->setIcon(0, QApplication::style()->standardIcon(QStyle::SP_DirClosedIcon));


	QTreeWidgetItem* pImportDirItem = new QTreeWidgetItem();
	pImportDirItem->setText(0, "ImportDirectory");
	pRootItem->addChild(pImportDirItem);
	pImportDirItem->setIcon(0, QApplication::style()->standardIcon(QStyle::SP_DirClosedIcon));
	
	//
	QTreeWidgetItem* pExportDirItem = new QTreeWidgetItem();
	pExportDirItem->setText(0, "ExportDirectory");
	pRootItem->addChild(pExportDirItem);
	pExportDirItem->setIcon(0, QApplication::style()->standardIcon(QStyle::SP_DirClosedIcon));


	QTreeWidgetItem* pRelocaDirItem = new QTreeWidgetItem();
	pRelocaDirItem->setText(0, "RelocationDirectory");
	pRootItem->addChild(pRelocaDirItem);
	pRelocaDirItem->setIcon(0, QApplication::style()->standardIcon(QStyle::SP_DirClosedIcon));

	QTreeWidgetItem* pTlsDirItem = new QTreeWidgetItem();
	pTlsDirItem->setText(0, "Tls");
	pRootItem->addChild(pTlsDirItem);
	pTlsDirItem->setIcon(0, QApplication::style()->standardIcon(QStyle::SP_DirClosedIcon));

	QTreeWidgetItem* pResourceDirItem = new QTreeWidgetItem();
	pResourceDirItem->setText(0, "Resourse");
	pRootItem->addChild(pResourceDirItem);
	pResourceDirItem->setIcon(0, QApplication::style()->standardIcon(QStyle::SP_DirClosedIcon));



	connect(ui.treeWidget, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(slot_SetWidegtUi(QTreeWidgetItem*, int)));
}


