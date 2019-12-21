/********************************************************************************
** Form generated from reading UI file 'cresourcewidget.ui'
**
** Created by: Qt User Interface Compiler version 5.9.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CRESOURCEWIDGET_H
#define UI_CRESOURCEWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CResourceWidget
{
public:
    QTreeWidget *treeWidget;
    QTableWidget *tableWidget;

    void setupUi(QWidget *CResourceWidget)
    {
        if (CResourceWidget->objectName().isEmpty())
            CResourceWidget->setObjectName(QStringLiteral("CResourceWidget"));
        CResourceWidget->resize(622, 436);
        treeWidget = new QTreeWidget(CResourceWidget);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        __qtreewidgetitem->setText(0, QStringLiteral("1"));
        treeWidget->setHeaderItem(__qtreewidgetitem);
        treeWidget->setObjectName(QStringLiteral("treeWidget"));
        treeWidget->setGeometry(QRect(0, 0, 621, 241));
        treeWidget->header()->setVisible(false);
        tableWidget = new QTableWidget(CResourceWidget);
        tableWidget->setObjectName(QStringLiteral("tableWidget"));
        tableWidget->setGeometry(QRect(0, 240, 621, 192));

        retranslateUi(CResourceWidget);

        QMetaObject::connectSlotsByName(CResourceWidget);
    } // setupUi

    void retranslateUi(QWidget *CResourceWidget)
    {
        CResourceWidget->setWindowTitle(QApplication::translate("CResourceWidget", "Form", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class CResourceWidget: public Ui_CResourceWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CRESOURCEWIDGET_H
