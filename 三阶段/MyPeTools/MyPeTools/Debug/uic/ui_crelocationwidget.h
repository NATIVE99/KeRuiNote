/********************************************************************************
** Form generated from reading UI file 'crelocationwidget.ui'
**
** Created by: Qt User Interface Compiler version 5.9.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CRELOCATIONWIDGET_H
#define UI_CRELOCATIONWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CRelocationWidget
{
public:
    QTableWidget *tableWidget;
    QTableWidget *tableWidget_2;

    void setupUi(QWidget *CRelocationWidget)
    {
        if (CRelocationWidget->objectName().isEmpty())
            CRelocationWidget->setObjectName(QStringLiteral("CRelocationWidget"));
        CRelocationWidget->resize(611, 480);
        tableWidget = new QTableWidget(CRelocationWidget);
        tableWidget->setObjectName(QStringLiteral("tableWidget"));
        tableWidget->setGeometry(QRect(0, 0, 611, 192));
        tableWidget_2 = new QTableWidget(CRelocationWidget);
        tableWidget_2->setObjectName(QStringLiteral("tableWidget_2"));
        tableWidget_2->setGeometry(QRect(0, 190, 611, 281));

        retranslateUi(CRelocationWidget);

        QMetaObject::connectSlotsByName(CRelocationWidget);
    } // setupUi

    void retranslateUi(QWidget *CRelocationWidget)
    {
        CRelocationWidget->setWindowTitle(QApplication::translate("CRelocationWidget", "Form", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class CRelocationWidget: public Ui_CRelocationWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CRELOCATIONWIDGET_H
