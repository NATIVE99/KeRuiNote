#pragma once

#include <QtWidgets/QMainWindow>
#include <qpushbutton.h>
#include <QMessageBox> 
#include <qstring.h>
#include <QDebug> 
#include <QVector>
#include <QScrollArea> 
#include "Cframwnd.h"
class cad : public QMainWindow
{
    Q_OBJECT

public:
    cad(QWidget *parent = Q_NULLPTR);

public slots:
void slots_click(bool);



private:
    Ui::cadClass ui;
 
    QPushButton *pushButton;

    
    QActionGroup m_ActionGroup;
   
   QScrollArea* m_Scorll;
    Cframwnd* m_fw;
     
};
