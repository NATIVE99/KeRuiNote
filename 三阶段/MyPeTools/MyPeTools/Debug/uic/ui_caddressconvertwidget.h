/********************************************************************************
** Form generated from reading UI file 'caddressconvertwidget.ui'
**
** Created by: Qt User Interface Compiler version 5.9.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CADDRESSCONVERTWIDGET_H
#define UI_CADDRESSCONVERTWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CAddressConvertWidget
{
public:
    QLineEdit *lineEdit;
    QLabel *label;
    QPushButton *pushButton;
    QLabel *label_2;
    QLineEdit *lineEdit_2;
    QLineEdit *lineEdit_3;
    QLabel *label_3;

    void setupUi(QWidget *CAddressConvertWidget)
    {
        if (CAddressConvertWidget->objectName().isEmpty())
            CAddressConvertWidget->setObjectName(QStringLiteral("CAddressConvertWidget"));
        CAddressConvertWidget->resize(400, 180);
        lineEdit = new QLineEdit(CAddressConvertWidget);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));
        lineEdit->setGeometry(QRect(50, 30, 121, 21));
        label = new QLabel(CAddressConvertWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(10, 30, 31, 16));
        pushButton = new QPushButton(CAddressConvertWidget);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(280, 110, 92, 28));
        label_2 = new QLabel(CAddressConvertWidget);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(10, 70, 51, 16));
        lineEdit_2 = new QLineEdit(CAddressConvertWidget);
        lineEdit_2->setObjectName(QStringLiteral("lineEdit_2"));
        lineEdit_2->setGeometry(QRect(50, 70, 121, 21));
        lineEdit_3 = new QLineEdit(CAddressConvertWidget);
        lineEdit_3->setObjectName(QStringLiteral("lineEdit_3"));
        lineEdit_3->setGeometry(QRect(52, 110, 121, 21));
        label_3 = new QLabel(CAddressConvertWidget);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(10, 110, 41, 16));

        retranslateUi(CAddressConvertWidget);
        QObject::connect(pushButton, SIGNAL(clicked()), CAddressConvertWidget, SLOT(slot_AddressConvert()));

        QMetaObject::connectSlotsByName(CAddressConvertWidget);
    } // setupUi

    void retranslateUi(QWidget *CAddressConvertWidget)
    {
        CAddressConvertWidget->setWindowTitle(QApplication::translate("CAddressConvertWidget", "Form", Q_NULLPTR));
        label->setText(QApplication::translate("CAddressConvertWidget", "VA:", Q_NULLPTR));
        pushButton->setText(QApplication::translate("CAddressConvertWidget", "convert", Q_NULLPTR));
        label_2->setText(QApplication::translate("CAddressConvertWidget", "RVA:", Q_NULLPTR));
        label_3->setText(QApplication::translate("CAddressConvertWidget", "FA:", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class CAddressConvertWidget: public Ui_CAddressConvertWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CADDRESSCONVERTWIDGET_H
