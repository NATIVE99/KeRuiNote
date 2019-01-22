/********************************************************************************
** Form generated from reading UI file 'cmddialog.ui'
**
** Created by: Qt User Interface Compiler version 5.9.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CMDDIALOG_H
#define UI_CMDDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QTextEdit>

QT_BEGIN_NAMESPACE

class Ui_CmdDialog
{
public:
    QLineEdit *lineEdit;
    QTextEdit *textEdit;

    void setupUi(QDialog *CmdDialog)
    {
        if (CmdDialog->objectName().isEmpty())
            CmdDialog->setObjectName(QStringLiteral("CmdDialog"));
        CmdDialog->resize(492, 415);
        lineEdit = new QLineEdit(CmdDialog);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));
        lineEdit->setGeometry(QRect(0, 10, 491, 31));
        textEdit = new QTextEdit(CmdDialog);
        textEdit->setObjectName(QStringLiteral("textEdit"));
        textEdit->setGeometry(QRect(0, 40, 491, 371));

        retranslateUi(CmdDialog);
        QObject::connect(lineEdit, SIGNAL(returnPressed()), CmdDialog, SLOT(slot1_return()));

        QMetaObject::connectSlotsByName(CmdDialog);
    } // setupUi

    void retranslateUi(QDialog *CmdDialog)
    {
        CmdDialog->setWindowTitle(QApplication::translate("CmdDialog", "Dialog", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class CmdDialog: public Ui_CmdDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CMDDIALOG_H
