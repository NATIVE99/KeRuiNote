#include "coptionsectinwidget.h"

COptionSectinWidget::COptionSectinWidget()
{
	this->setColumnCount(10);
	QStringList header;
	header << QStringLiteral("名称") << QStringLiteral("虚拟大小") << QStringLiteral("虚拟地址") << QStringLiteral("Raw大小") << QStringLiteral("Raw地址") <<
		QStringLiteral("重定位地址") << QStringLiteral("行号") << QStringLiteral("重定位数值") << QStringLiteral("行号个数") << QStringLiteral("字符特征");
	this->setHorizontalHeaderLabels(header);
}

COptionSectinWidget::~COptionSectinWidget()
{
}

void COptionSectinWidget::SetMsg(PIMAGE_SECTION_HEADER pSections, IMAGE_FILE_HEADER& tagFileHead)
{
	const char* Name[] = { "BYTE[8]","DWORD","DWORD","DWORD","DWORD","DWORD","DWORD","WORD","WORD","DWORD" };

	this->insertRow(0);
	for (int i = 0; i < sizeof(Name) / sizeof(char*); i++)
	{
		this->setItem(0, i, new QTableWidgetItem(Name[i]));
	}

	for (int i = 1; i < tagFileHead.NumberOfSections + 1; i++)
	{
		this->insertRow(i);
		int structoffset = 0;
		for (int j = 0; j < 10; j++)
		{


			if (strcmp(Name[j], "WORD") == 0)
			{
				WORD dw = *(WORD*)((char*)&pSections[i - 1] + structoffset);
				QString strValue = QString("%1").arg(dw, 4, 16, QLatin1Char('0'));
				this->setItem(i, j, new QTableWidgetItem(strValue.toUpper()));
				structoffset += 2;
			}
			else if (strcmp(Name[j], "DWORD") == 0)
			{
				DWORD dw = *(DWORD*)((char*)&pSections[i - 1] + structoffset);
				QString strValue = QString("%1").arg(dw, 8, 16, QLatin1Char('0'));
				this->setItem(i, j, new QTableWidgetItem(strValue.toUpper()));

				structoffset += 4;
			}
			else
			{
				this->setItem(i, j, new QTableWidgetItem((char*)pSections[i - 1].Name));
				structoffset += 8;
			}
		}

	}
}
	

	
	

