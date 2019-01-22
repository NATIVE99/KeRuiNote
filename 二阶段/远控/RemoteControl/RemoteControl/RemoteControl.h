#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_RemoteControl.h"
#include <qsharedpointer.h>
#include "SeverScoket.h"
#include "CAcceptThread.h"
#include "cmddialog.h"
class RemoteControl : public QMainWindow
{
    Q_OBJECT

public:
    RemoteControl(QWidget *parent = Q_NULLPTR);
    ~RemoteControl();
public slots:
    void slot_ShowMenu(const QPoint& pos);
    void slot_ShowCmd();
    void slot_ShowGreen();
    void HandleClinentInfo(CClientSession* pCs);

    void slot_RecvData(int s, int nDialogType, QByteArray data);
    void slot_CreateDialog(int s, int nDialogType);
private:
    Ui::RemoteControlClass ui;
    //ÓÒ¼ü²Ëµ¥
    QSharedPointer<QMenu> m_pMeun;
    QSharedPointer<CAcceptThread> m_pAccept;
    QSharedPointer<CmdDialog>    m_pCmdDia;
    SeverScoket m_pSocket;


};
