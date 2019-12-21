#pragma once

#include "cmytablewidget.h"

class COptionFileWidget :public CMyTableWidget
{
	Q_OBJECT

public:
	COptionFileWidget();
	~COptionFileWidget();
	void SetMsg(IMAGE_OPTIONAL_HEADER32& Op32, IMAGE_DOS_HEADER& dos);
};
