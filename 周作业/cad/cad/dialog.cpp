#include "dialog.h"
#include "ui_dialog.h"
#include "qDebug"
 QColor g_color;
 int    g_witgh;
 int    g_Isbrush;
Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    QString str = QString(QStringLiteral("background-color: rgb(%1, %2, %3);"))
        .arg(g_color.red())
        .arg(g_color.green())
        .arg(g_color.blue());
    ui->pushButton->setStyleSheet(str);

    ui->horizontalSlider->setMinimum(1);
    ui->horizontalSlider->setMaximum(100);
    ui->horizontalSlider->setValue(g_witgh);
    if(g_Isbrush==2)
    {
        ui->checkBox->setCheckState(Qt::Checked);
    }
   
}

Dialog::~Dialog()
{
    delete ui;
}

//²Û
//////////////////////////////////////

void Dialog::slot1_color()
{
    QColorDialog ColorDig;
   int Resl=ColorDig.exec();

   if(Resl== QDialog::Accepted)
   {
       g_color = ColorDig.currentColor();
       qDebug() << g_color.red() << " " << g_color.green();
       QString str = QString(QStringLiteral("background-color: rgb(%1, %2, %3);"))
           .arg(g_color.red())
           .arg(g_color.green())
           .arg(g_color.blue());
       ui->pushButton->setStyleSheet(str);
   
   }

}

void Dialog::slot_slidermove()
{
    g_witgh=ui->horizontalSlider->value();

    qDebug() << g_witgh;
}

void Dialog::slot1_StateChaget(int state)
{
    if(state== Qt::Checked)
    {
        g_Isbrush = state;
        qDebug() << g_Isbrush;
    }
    else
    {
        g_Isbrush = 0;
    }

}