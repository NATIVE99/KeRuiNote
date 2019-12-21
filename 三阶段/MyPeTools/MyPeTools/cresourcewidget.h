#ifndef CRESOURCEWIDGET_H
#define CRESOURCEWIDGET_H

#include <QWidget>
#include <windows.h>
#include <QFile>
#include <QTreeWidgetItem>
namespace Ui {
class CResourceWidget;
}

class CResourceWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CResourceWidget(QWidget *parent = nullptr);
    ~CResourceWidget();
public:
	void SetMsg(IMAGE_FILE_HEADER &FileHeader, PIMAGE_SECTION_HEADER pSections, QString m_Path, DWORD ExportDirAddress);
private:

	DWORD RVA2FA(DWORD nRva);
	void DeleteAllRow();
	public slots:

	void slot_GetData(QTreeWidgetItem *item, int column);
	//void slot_FindFunction(QTableWidgetItem *item);
private:
	Ui::CResourceWidget *ui;
	DWORD m_dwVirtualAddress;
	QFile m_File;
	IMAGE_FILE_HEADER m_FileHeader;
	PIMAGE_SECTION_HEADER m_pSections;

};
#endif // CRESOURCEWIDGET_H
