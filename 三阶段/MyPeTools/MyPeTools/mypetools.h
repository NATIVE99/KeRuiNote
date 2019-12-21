#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_mypetools.h"
#include "cmytablewidget.h"
#include "cdoswidget.h"
#include "cntwidget.h"
#include "cfilewidget.h"
#include "COptionFileWidget.h"
#include "coptionsectinwidget.h"
#include "caddressconvertwidget.h"
#include "cimportwidget.h"
#include "cexportwidget.h"
#include "crelocationwidget.h"
#include "ctlswidget.h"
#include "cresourcewidget.h"
#include <QGridLayout>
#include <QPushButton>
#include <QFile>
#include <windows.h>
class MyPeTools : public QMainWindow
{
	Q_OBJECT

public:
	MyPeTools(QWidget *parent = Q_NULLPTR);


public slots:
	void slot_start();
	void slot_SetWidegtUi(QTreeWidgetItem*, int);
private:
	//file
	void ReadFile();
	bool GetPath();

	//UI
	void SetTreeUi();

private:
	Ui::MyPeToolsClass ui;

	//file path
	QString m_strPath;
	QFile   m_File;

	//bool
	bool m_Is32;

	//PE header
	/***********************************************/
	//DOS
	IMAGE_DOS_HEADER m_dos;
	//NT
	DWORD m_Signature;
	IMAGE_FILE_HEADER m_FileHeader;
	IMAGE_OPTIONAL_HEADER32 m_Op32;
	IMAGE_OPTIONAL_HEADER64 m_Op64;
	//Section
	PIMAGE_SECTION_HEADER m_pSections;

	//Tls
	IMAGE_TLS_DIRECTORY	m_Tls;
	/***********************************************/

	//widget

	CMyTableWidget* m_PreWidget;
	CDosWidget *m_pDosWidet;
	CNtWidget *m_PNtWidget;
	CFileWidget* m_pFileWidget;
	COptionFileWidget* m_pOpwidget;
	COptionSectinWidget* m_pSectinWidget;
	CAddressConvertWidget* m_pAddressConvertWidget;
	CImportwidget* m_pImportWidget;
	CExportWidget* m_pExportWidget;
	CRelocationWidget* m_pRelocaWidget;
	CTlsWidget* m_pTlsWidget;
	CResourceWidget* m_pResourseWidget;

	
};


