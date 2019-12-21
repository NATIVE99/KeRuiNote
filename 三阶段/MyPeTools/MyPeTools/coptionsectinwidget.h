#pragma once


#include "cmytablewidget.h"
class COptionSectinWidget :  public CMyTableWidget
{
	Q_OBJECT

public:
	COptionSectinWidget();
	~COptionSectinWidget();
	void SetMsg(PIMAGE_SECTION_HEADER pSections, IMAGE_FILE_HEADER& tagFileHead);
};
