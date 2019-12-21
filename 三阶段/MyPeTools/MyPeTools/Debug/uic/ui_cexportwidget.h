/********************************************************************************
** Form generated from reading UI file 'cexportwidget.ui'
**
** Created by: Qt User Interface Compiler version 5.9.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CEXPORTWIDGET_H
#define UI_CEXPORTWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CExportWidget
{
public:
    QTableWidget *tableWidget;
    QTableWidget *tableWidget_2;

    void setupUi(QWidget *CExportWidget)
    {
        if (CExportWidget->objectName().isEmpty())
            CExportWidget->setObjectName(QStringLiteral("CExportWidget"));
        CExportWidget->resize(610, 457);
        tableWidget = new QTableWidget(CExportWidget);
        tableWidget->setObjectName(QStringLiteral("tableWidget"));
        tableWidget->setGeometry(QRect(0, 0, 601, 201));
        tableWidget_2 = new QTableWidget(CExportWidget);
        tableWidget_2->setObjectName(QStringLiteral("tableWidget_2"));
        tableWidget_2->setGeometry(QRect(0, 200, 601, 241));

        retranslateUi(CExportWidget);

        QMetaObject::connectSlotsByName(CExportWidget);
    } // setupUi

    void retranslateUi(QWidget *CExportWidget)
    {
        CExportWidget->setWindowTitle(QApplication::translate("CExportWidget", "Form", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class CExportWidget: public Ui_CExportWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CEXPORTWIDGET_H
