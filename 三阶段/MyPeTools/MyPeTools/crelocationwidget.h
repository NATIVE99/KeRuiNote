#ifndef CRELOCATIONWIDGET_H
#define CRELOCATIONWIDGET_H

#include <QWidget>
#include <windows.h>
#include <QFile>
#include <QTableWidgetItem>
struct  tagReloc
{	union 
	{
		WORD wAll;
		struct 
		{
			WORD w0ffset : 12;
			WORD wAttribute : 4;
			
		}MyRec;
	}u;
};
namespace Ui {
class CRelocationWidget;
}

class CRelocationWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CRelocationWidget(QWidget *parent = nullptr);
    ~CRelocationWidget();

public:
	void SetMsg(IMAGE_FILE_HEADER &FileHeader, PIMAGE_SECTION_HEADER pSections, QString m_Path, DWORD ExportDirAddress);
private:

	DWORD RVA2FA(DWORD nRva);
	void DeleteAllRow();

public slots:
	void slot_FindReloc(QTableWidgetItem *item);

private:
    Ui::CRelocationWidget *ui;
	DWORD m_dwVirtualAddress;
	QFile m_File;
	IMAGE_FILE_HEADER m_FileHeader;
	PIMAGE_SECTION_HEADER m_pSections;
};

#endif // CRELOCATIONWIDGET_H
