#ifndef PROCESSDIALOG_H
#define PROCESSDIALOG_H

#include <QDialog>
#include <QMenu>
#include "../../commd/commend.h"
namespace Ui {
class ProcessDialog;
}

class ProcessDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ProcessDialog(SOCKET s,QWidget *parent = 0);
    ~ProcessDialog();
public slots:
    void handleResults(QByteArray arr);
    void slots_ShowMenu(const QPoint & pos);
    void slots_sendEnd(bool b);
    void slots_deleteProcess(QByteArray arr);
private:
    Ui::ProcessDialog *ui;
    QMenu* m_PMenu;
    SOCKET m_cliend;
};

#endif // PROCESSDIALOG_H
