/********************************************************************************
** Form generated from reading UI file 'cimportwidget.ui'
**
** Created by: Qt User Interface Compiler version 5.9.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CIMPORTWIDGET_H
#define UI_CIMPORTWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CImportwidget
{
public:
    QTableWidget *tableWidget;
    QTableWidget *tableWidget_2;

    void setupUi(QWidget *CImportwidget)
    {
        if (CImportwidget->objectName().isEmpty())
            CImportwidget->setObjectName(QStringLiteral("CImportwidget"));
        CImportwidget->resize(605, 480);
        tableWidget = new QTableWidget(CImportwidget);
        tableWidget->setObjectName(QStringLiteral("tableWidget"));
        tableWidget->setGeometry(QRect(0, 0, 601, 221));
        tableWidget_2 = new QTableWidget(CImportwidget);
        tableWidget_2->setObjectName(QStringLiteral("tableWidget_2"));
        tableWidget_2->setGeometry(QRect(0, 220, 601, 251));

        retranslateUi(CImportwidget);

        QMetaObject::connectSlotsByName(CImportwidget);
    } // setupUi

    void retranslateUi(QWidget *CImportwidget)
    {
        CImportwidget->setWindowTitle(QApplication::translate("CImportwidget", "Form", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class CImportwidget: public Ui_CImportwidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CIMPORTWIDGET_H
