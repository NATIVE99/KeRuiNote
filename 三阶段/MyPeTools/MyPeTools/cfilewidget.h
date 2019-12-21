#pragma once

#include "cmytablewidget.h"

class CFileWidget :public CMyTableWidget
{
	Q_OBJECT

public:
	CFileWidget();
	~CFileWidget();
	void SetMsg(IMAGE_DOS_HEADER& dos, IMAGE_FILE_HEADER& tagFileHead);
};
