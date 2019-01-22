#include "cmddialog.h"
#include "ui_cmddialog.h"

CmdDialog::CmdDialog(SOCKET s,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CmdDialog),m_s(s)
{
    ui->setupUi(this);
}

CmdDialog::~CmdDialog()
{
    delete ui;
}

void CmdDialog::slot1_return()
{

    QString str = ui->lineEdit->text();
    str += "\n";
    SendData(m_s,
        SERVER_CMD_DATA,
        str.toLocal8Bit().toStdString().c_str(),
        str.toLocal8Bit().toStdString().length());
    
}

void CmdDialog::Handledata(QByteArray data)
{

    QString str = QString::fromLocal8Bit(data);
    ui->textEdit->append(str);  
}