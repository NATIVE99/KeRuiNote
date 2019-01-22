#ifndef CMDDIALOG_H
#define CMDDIALOG_H

#include <QDialog>
#include <windows.h>
#include "../../comment/macro.h"
namespace Ui {
class CmdDialog;
}

class CmdDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CmdDialog(SOCKET s,QWidget *parent = 0);
    ~CmdDialog();
    void Handledata(QByteArray data);

public slots:
    void slot1_return();
private:
    Ui::CmdDialog *ui;
    SOCKET m_s;
};

#endif // CMDDIALOG_H
