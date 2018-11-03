/********************************************************************************
** Form generated from reading UI file 'cad.ui'
**
** Created by: Qt User Interface Compiler version 5.9.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CAD_H
#define UI_CAD_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_cadClass
{
public:
    QAction *line;
    QAction *rect;
    QAction *eillpse;
    QAction *color;
    QAction *select;
    QAction *undo;
    QAction *exect;
    QAction *open;
    QAction *save;
    QWidget *centralWidget;
    QMenuBar *menuBar;
    QMenu *menu;
    QMenu *menu_2;
    QMenu *menu_3;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *cadClass)
    {
        if (cadClass->objectName().isEmpty())
            cadClass->setObjectName(QStringLiteral("cadClass"));
        cadClass->resize(600, 400);
        line = new QAction(cadClass);
        line->setObjectName(QStringLiteral("line"));
        line->setCheckable(true);
        QIcon icon;
        icon.addFile(QStringLiteral(":/cad/Resources/line_32px_1101685_easyicon.net.ico"), QSize(), QIcon::Normal, QIcon::Off);
        line->setIcon(icon);
        rect = new QAction(cadClass);
        rect->setObjectName(QStringLiteral("rect"));
        rect->setCheckable(true);
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/cad/Resources/rectangle_32px_1125662_easyicon.net.ico"), QSize(), QIcon::Normal, QIcon::Off);
        rect->setIcon(icon1);
        eillpse = new QAction(cadClass);
        eillpse->setObjectName(QStringLiteral("eillpse"));
        eillpse->setCheckable(true);
        QIcon icon2;
        icon2.addFile(QStringLiteral(":/cad/Resources/ellipse_stroked_32px_1168912_easyicon.net.ico"), QSize(), QIcon::Normal, QIcon::Off);
        eillpse->setIcon(icon2);
        color = new QAction(cadClass);
        color->setObjectName(QStringLiteral("color"));
        QIcon icon3;
        icon3.addFile(QStringLiteral(":/cad/Resources/my_work_Color_plate_32px_515147_easyicon.net.ico"), QSize(), QIcon::Normal, QIcon::Off);
        color->setIcon(icon3);
        select = new QAction(cadClass);
        select->setObjectName(QStringLiteral("select"));
        select->setCheckable(true);
        QIcon icon4;
        icon4.addFile(QStringLiteral(":/cad/Resources/move_32px_1208133_easyicon.net.ico"), QSize(), QIcon::Normal, QIcon::Off);
        select->setIcon(icon4);
        undo = new QAction(cadClass);
        undo->setObjectName(QStringLiteral("undo"));
        QIcon icon5;
        icon5.addFile(QStringLiteral(":/cad/Resources/20181011081318398_easyicon_net_32.ico"), QSize(), QIcon::Normal, QIcon::Off);
        undo->setIcon(icon5);
        exect = new QAction(cadClass);
        exect->setObjectName(QStringLiteral("exect"));
        QIcon icon6;
        icon6.addFile(QStringLiteral(":/cad/Resources/Redo_32px_522651_easyicon.net.ico"), QSize(), QIcon::Normal, QIcon::Off);
        exect->setIcon(icon6);
        open = new QAction(cadClass);
        open->setObjectName(QStringLiteral("open"));
        save = new QAction(cadClass);
        save->setObjectName(QStringLiteral("save"));
        centralWidget = new QWidget(cadClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        cadClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(cadClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 600, 23));
        menu = new QMenu(menuBar);
        menu->setObjectName(QStringLiteral("menu"));
        menu_2 = new QMenu(menuBar);
        menu_2->setObjectName(QStringLiteral("menu_2"));
        menu_3 = new QMenu(menuBar);
        menu_3->setObjectName(QStringLiteral("menu_3"));
        cadClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(cadClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        cadClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(cadClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        cadClass->setStatusBar(statusBar);

        menuBar->addAction(menu->menuAction());
        menuBar->addAction(menu_2->menuAction());
        menuBar->addAction(menu_3->menuAction());
        menu->addAction(open);
        menu->addAction(save);
        menu_2->addSeparator();
        menu_2->addAction(line);
        menu_2->addAction(rect);
        menu_2->addAction(eillpse);
        menu_3->addAction(color);
        menu_3->addAction(select);
        menu_3->addSeparator();
        menu_3->addAction(undo);
        menu_3->addAction(exect);
        mainToolBar->addAction(line);
        mainToolBar->addAction(rect);
        mainToolBar->addAction(eillpse);
        mainToolBar->addAction(color);
        mainToolBar->addAction(select);
        mainToolBar->addAction(undo);
        mainToolBar->addAction(exect);

        retranslateUi(cadClass);

        QMetaObject::connectSlotsByName(cadClass);
    } // setupUi

    void retranslateUi(QMainWindow *cadClass)
    {
        cadClass->setWindowTitle(QApplication::translate("cadClass", "cad", Q_NULLPTR));
        line->setText(QApplication::translate("cadClass", "\347\233\264\347\272\277", Q_NULLPTR));
        rect->setText(QApplication::translate("cadClass", "\347\237\251\345\275\242", Q_NULLPTR));
        eillpse->setText(QApplication::translate("cadClass", "\346\244\255\345\234\206", Q_NULLPTR));
        color->setText(QApplication::translate("cadClass", "\351\242\234\350\211\262", Q_NULLPTR));
        select->setText(QApplication::translate("cadClass", "\347\247\273\345\212\250", Q_NULLPTR));
        undo->setText(QApplication::translate("cadClass", "\346\222\244\351\224\200", Q_NULLPTR));
        exect->setText(QApplication::translate("cadClass", "\351\207\215\345\201\232", Q_NULLPTR));
        open->setText(QApplication::translate("cadClass", "\346\211\223\345\274\200", Q_NULLPTR));
        save->setText(QApplication::translate("cadClass", "\344\277\235\345\255\230", Q_NULLPTR));
        menu->setTitle(QApplication::translate("cadClass", "\346\226\207\344\273\266", Q_NULLPTR));
        menu_2->setTitle(QApplication::translate("cadClass", "\345\233\276\345\275\242", Q_NULLPTR));
        menu_3->setTitle(QApplication::translate("cadClass", "\345\267\245\345\205\267", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class cadClass: public Ui_cadClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CAD_H
