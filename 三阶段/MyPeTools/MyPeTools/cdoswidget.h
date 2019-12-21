#pragma once

#include "cmytablewidget.h"

class CDosWidget : public CMyTableWidget
{
	Q_OBJECT

public:
	CDosWidget();
	~CDosWidget();
	void SetMsg(IMAGE_DOS_HEADER& dos);
};
