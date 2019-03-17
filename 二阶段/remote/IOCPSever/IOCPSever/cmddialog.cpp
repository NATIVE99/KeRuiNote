#include "cmddialog.h"
#include "ui_cmddialog.h"

CmdDialog::CmdDialog(SOCKET s, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CmdDialog)
{
    ui->setupUi(this);
    m_client = s;
    ui->textEdit->setReadOnly(true);
    QObject::connect(ui->lineEdit, SIGNAL(returnPressed()), this, SLOT(slots_SendCmd()));
}

CmdDialog::~CmdDialog()
{
    delete ui;
}

void CmdDialog::handleResults(QByteArray str)
{
    
    ui->textEdit->append(QString::fromLocal8Bit(str));
}

void CmdDialog::slots_SendCmd()
{
    QString str = ui->lineEdit->text() + "\n";

    SendData2(m_client, CUT_CMD, str.toStdString().c_str(), str.length());
    //send(m_client, str.toStdString().c_str(), str.length(), 0);
}


