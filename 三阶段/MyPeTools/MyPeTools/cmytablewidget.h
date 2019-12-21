#ifndef CMYTABLEWIDGET_H
#define CMYTABLEWIDGET_H

#include <QTableWidget>
#include <windows.h>
class CMyTableWidget:public QTableWidget
{
	Q_OBJECT
public:
    CMyTableWidget();
	~CMyTableWidget();
	//virtual void Show()=0;
};

#endif // CMYTABLEWIDGET_H