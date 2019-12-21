/********************************************************************************
** Form generated from reading UI file 'mypetools.ui'
**
** Created by: Qt User Interface Compiler version 5.9.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MYPETOOLS_H
#define UI_MYPETOOLS_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MyPeToolsClass
{
public:
    QWidget *centralWidget;
    QTreeWidget *treeWidget;
    QPushButton *pushButton;
    QTextEdit *textEdit;
    QLabel *label;
    QWidget *gridLayoutWidget;
    QGridLayout *gridLayout;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MyPeToolsClass)
    {
        if (MyPeToolsClass->objectName().isEmpty())
            MyPeToolsClass->setObjectName(QStringLiteral("MyPeToolsClass"));
        MyPeToolsClass->resize(777, 557);
        centralWidget = new QWidget(MyPeToolsClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        treeWidget = new QTreeWidget(centralWidget);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        __qtreewidgetitem->setText(0, QStringLiteral("1"));
        treeWidget->setHeaderItem(__qtreewidgetitem);
        treeWidget->setObjectName(QStringLiteral("treeWidget"));
        treeWidget->setGeometry(QRect(0, 40, 171, 451));
        treeWidget->setAlternatingRowColors(false);
        treeWidget->header()->setVisible(false);
        treeWidget->header()->setCascadingSectionResizes(false);
        treeWidget->header()->setHighlightSections(false);
        treeWidget->header()->setProperty("showSortIndicator", QVariant(false));
        pushButton = new QPushButton(centralWidget);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(680, 10, 75, 23));
        textEdit = new QTextEdit(centralWidget);
        textEdit->setObjectName(QStringLiteral("textEdit"));
        textEdit->setGeometry(QRect(60, 10, 601, 21));
        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(0, 10, 54, 12));
        gridLayoutWidget = new QWidget(centralWidget);
        gridLayoutWidget->setObjectName(QStringLiteral("gridLayoutWidget"));
        gridLayoutWidget->setGeometry(QRect(170, 40, 591, 451));
        gridLayout = new QGridLayout(gridLayoutWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        MyPeToolsClass->setCentralWidget(centralWidget);
        treeWidget->raise();
        textEdit->raise();
        label->raise();
        pushButton->raise();
        gridLayoutWidget->raise();
        menuBar = new QMenuBar(MyPeToolsClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 777, 26));
        MyPeToolsClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MyPeToolsClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MyPeToolsClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MyPeToolsClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MyPeToolsClass->setStatusBar(statusBar);

        retranslateUi(MyPeToolsClass);
        QObject::connect(pushButton, SIGNAL(clicked()), MyPeToolsClass, SLOT(slot_start()));

        QMetaObject::connectSlotsByName(MyPeToolsClass);
    } // setupUi

    void retranslateUi(QMainWindow *MyPeToolsClass)
    {
        MyPeToolsClass->setWindowTitle(QApplication::translate("MyPeToolsClass", "MyPeTools", Q_NULLPTR));
        pushButton->setText(QApplication::translate("MyPeToolsClass", "\350\247\243\346\236\220", Q_NULLPTR));
        label->setText(QApplication::translate("MyPeToolsClass", "path: ", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MyPeToolsClass: public Ui_MyPeToolsClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MYPETOOLS_H
