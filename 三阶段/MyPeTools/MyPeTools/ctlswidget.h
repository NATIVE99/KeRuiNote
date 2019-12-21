#ifndef CTLSWIDGET_H
#define CTLSWIDGET_H

#include "cmytablewidget.h"


class CTlsWidget : public CMyTableWidget
{
	Q_OBJECT
public:
    CTlsWidget();
	~CTlsWidget();
	void SetMsg(IMAGE_TLS_DIRECTORY& Tls, DWORD dwOffset);
};

#endif // CTLSWIDGET_H