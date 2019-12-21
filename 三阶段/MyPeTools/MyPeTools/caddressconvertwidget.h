#ifndef CADDRESSCONVERTWIDGET_H
#define CADDRESSCONVERTWIDGET_H

#include <QWidget>
#include <windows.h>
namespace Ui {
class CAddressConvertWidget;
}

class CAddressConvertWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CAddressConvertWidget(QWidget *parent = nullptr);
    ~CAddressConvertWidget();
public slots:
	void slot_AddressConvert();
public:
	void SetMsg(IMAGE_FILE_HEADER &FileHeader, PIMAGE_SECTION_HEADER pSections, DWORD ImageBase);
	int RVA2FA(int nRva);
private:
    Ui::CAddressConvertWidget *ui;
	IMAGE_FILE_HEADER m_FileHeader;
	PIMAGE_SECTION_HEADER m_pSections;
	DWORD m_ImageBase;
};

#endif // CADDRESSCONVERTWIDGET_H
