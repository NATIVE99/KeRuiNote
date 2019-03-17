#ifndef CMDDIALOG_H
#define CMDDIALOG_H

#include <QDialog>
#include <windows.h>
#include "../../commd/commend.h"
namespace Ui {
class CmdDialog;
}

class CmdDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CmdDialog(SOCKET s,QWidget *parent = 0);
    ~CmdDialog();
public slots:
    void handleResults(QByteArray str);
    void slots_SendCmd();
private:
    Ui::CmdDialog *ui;
    SOCKET m_client;
};

#endif // CMDDIALOG_H
