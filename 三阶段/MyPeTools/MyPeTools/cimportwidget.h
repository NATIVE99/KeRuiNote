#ifndef CIMPORTWIDGET_H
#define CIMPORTWIDGET_H

#include <QWidget>
#include <qfile.h>
#include <windows.h>
#include <QTableWidgetItem>
namespace Ui {
class CImportwidget;
}

class CImportwidget : public QWidget
{
    Q_OBJECT

public:
    explicit CImportwidget(QWidget *parent = nullptr);
    ~CImportwidget();
	
public:
	void SetMsg(IMAGE_FILE_HEADER &FileHeader, PIMAGE_SECTION_HEADER pSections,QString m_Path,DWORD ExportDirAddress);
private:

	DWORD RVA2FA(DWORD nRva);
	void DeleteAllRow();
public slots:

	void slot_FindFunction(QTableWidgetItem *item);

private:
    Ui::CImportwidget *ui;
	DWORD m_dwVirtualAddress;
	QFile m_File;
	IMAGE_FILE_HEADER m_FileHeader;
	PIMAGE_SECTION_HEADER m_pSections;
};

#endif // CIMPORTWIDGET_H
