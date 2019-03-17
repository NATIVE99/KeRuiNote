#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_iocpsever.h"
#include "MySocket.h"
#include <mutex>
class IOCPSever : public QMainWindow
{
    Q_OBJECT

public:
    IOCPSever(QWidget *parent = Q_NULLPTR);
    void InitSocket();
    void InitUI();

    Session* GetQtbaleSocket();
public slots:

void slots_HandleSession(Session* ps);
void slots_ShowMenu(const QPoint & pos);
void slots_ShowScreen(bool ch);
void slots_ShowCmd(bool ch);
void slots_ShowProcess(bool ch);
void slots_ShowFile(bool ch);
void slots_UpDateTime(Session* ps);
private:
    Ui::IOCPSeverClass ui;
    MySocket* m_pSocket;
    QMenu* m_PMenu;

};
