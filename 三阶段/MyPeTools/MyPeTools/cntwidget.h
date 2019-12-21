#pragma once

#include "cmytablewidget.h"
class CNtWidget : public CMyTableWidget
{
	Q_OBJECT

public:
	CNtWidget();
	~CNtWidget();
	void SetMsg(IMAGE_DOS_HEADER& dos,DWORD dwSig);
};
