#ifndef CEXPORTWIDGET_H
#define CEXPORTWIDGET_H

#include <QWidget>
#include <windows.h>
#include <QFile>
namespace Ui {
class CExportWidget;
}

class CExportWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CExportWidget(QWidget *parent = nullptr);
    ~CExportWidget();
public:
	void SetMsg(IMAGE_FILE_HEADER &FileHeader, PIMAGE_SECTION_HEADER pSections, QString m_Path, DWORD ExportDirAddress);
private:

	DWORD RVA2FA(DWORD nRva);

private:
    Ui::CExportWidget *ui;
	DWORD m_dwVirtualAddress;
	QFile m_File;
	IMAGE_FILE_HEADER m_FileHeader;
	PIMAGE_SECTION_HEADER m_pSections;
};

#endif // CEXPORTWIDGET_H
