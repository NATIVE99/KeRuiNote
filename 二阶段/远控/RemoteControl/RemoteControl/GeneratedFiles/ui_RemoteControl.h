/********************************************************************************
** Form generated from reading UI file 'RemoteControl.ui'
**
** Created by: Qt User Interface Compiler version 5.9.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_REMOTECONTROL_H
#define UI_REMOTECONTROL_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_RemoteControlClass
{
public:
    QWidget *centralWidget;
    QTableWidget *tableWidget;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *RemoteControlClass)
    {
        if (RemoteControlClass->objectName().isEmpty())
            RemoteControlClass->setObjectName(QStringLiteral("RemoteControlClass"));
        RemoteControlClass->resize(600, 400);
        centralWidget = new QWidget(RemoteControlClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        tableWidget = new QTableWidget(centralWidget);
        tableWidget->setObjectName(QStringLiteral("tableWidget"));
        tableWidget->setGeometry(QRect(0, 0, 591, 331));
        RemoteControlClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(RemoteControlClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 600, 23));
        RemoteControlClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(RemoteControlClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        RemoteControlClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(RemoteControlClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        RemoteControlClass->setStatusBar(statusBar);

        retranslateUi(RemoteControlClass);

        QMetaObject::connectSlotsByName(RemoteControlClass);
    } // setupUi

    void retranslateUi(QMainWindow *RemoteControlClass)
    {
        RemoteControlClass->setWindowTitle(QApplication::translate("RemoteControlClass", "RemoteControl", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class RemoteControlClass: public Ui_RemoteControlClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_REMOTECONTROL_H
