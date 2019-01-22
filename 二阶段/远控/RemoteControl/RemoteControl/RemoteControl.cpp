#include "RemoteControl.h"
#include <qdebug>
extern RemoteControl* g_pWindow;
RemoteControl::RemoteControl(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    g_pWindow = this;
    //设置Widegt不能被放大
    setFixedSize(width(), height());
    //设置tablewidget表头
    ui.tableWidget->setColumnCount(1);
    QStringList head;
    head << "ip";
    ui.tableWidget->setHorizontalHeaderLabels(head);


    //设置右键菜
    ui.tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);//设置tablewidegt菜单风格
    m_pMeun.reset(new QMenu());
    QAction* pCmdAction=m_pMeun->addAction(QStringLiteral("cmd"));
    QAction* pGreenAction= m_pMeun->addAction(QStringLiteral("屏幕"));
    //关联右键
    connect(ui.tableWidget,
        SIGNAL(customContextMenuRequested(const QPoint&)),
        this,
        SLOT(slot_ShowMenu(const QPoint&)));
  //右键菜单关联槽
    QObject::connect(pCmdAction,SIGNAL(triggered()),this,SLOT(slot_ShowCmd()));
    QObject::connect(pGreenAction, SIGNAL(triggered()), this, SLOT(slot_ShowGreen()));

   
    //初始化socket

    m_pSocket.StartUp();
    m_pSocket.CreateSocketAndBind();

    //绑定socketrsever信号
    QObject::connect(&m_pSocket, SIGNAL(sendClinentInfo(CClientSession* )), this, SLOT(HandleClinentInfo(CClientSession* )));
    //accpet线程启动
    m_pAccept.reset(new CAcceptThread(m_pSocket));
    m_pAccept->start();

    
}
RemoteControl::~RemoteControl()
{
    m_pAccept->exit();
}

void RemoteControl::slot_ShowMenu(const QPoint& pos)
{
    m_pMeun->exec(QCursor::pos());
}

void RemoteControl::slot_ShowCmd()
{
    qDebug()<< "ShowCmd";
  /*  m_pCmdDia.reset(new CmdDialog());
m_pCmdDia->show();*/
//发送数据，通知客户端开启cmd功能
//获取当前的选中的item
//QTableWidgetItem * pItem = ui->tableWidget->currentItem();

    int nIndex = ui.tableWidget->currentRow();
    QTableWidgetItem * pItem = ui.tableWidget->item(nIndex, 0);

    SOCKET sClient = pItem->data(Qt::UserRole).toInt();

    //向客户端发送打开cmd的命令

    QSharedPointer<CClientSession> clientSession = m_pSocket.GetClient(sClient);
    if (clientSession->m_pCmdDlg == NULL) {
        qDebug() << "sizeof(tagPacket)" << sizeof(tagPacket) << endl;
        SendData(sClient, SERVER_CMD, NULL, 0);
    }
    else {
        clientSession->m_pCmdDlg->show();
    }
}
void RemoteControl::slot_ShowGreen()
{
    qDebug() << "slot_ShowGreen";
}

void RemoteControl::HandleClinentInfo(CClientSession* pCs)
{
    //QString str = QString("delta: %1").arg(d, 0, 'E', 3);
    QString strIp = QString("%1:%2").arg((inet_ntoa)(pCs->m_addr.sin_addr))
        .arg((ntohs)(pCs->m_addr.sin_port));

    int nRow = ui.tableWidget->rowCount();
    ui.tableWidget->insertRow(nRow);

    QTableWidgetItem * pNewTableItem = new QTableWidgetItem();
    pNewTableItem->setText(strIp);
    pNewTableItem->setData(Qt::UserRole, pCs->m_s);
    ui.tableWidget->setItem(nRow, 0, pNewTableItem);

}
void RemoteControl::slot_RecvData(int s, int nDialogType, QByteArray data)
{
    SOCKET sClient = (SOCKET)s;

    //通过socket 查找pClient
    QSharedPointer<CClientSession> clientSession = m_pSocket.GetClient(sClient);

    switch (nDialogType)
    {
    case CMD_CREATE:
    {
        qDebug() << "slot_CreateCmdDialog" << endl;
        if (clientSession->m_pCmdDlg == NULL) {
            clientSession->m_pCmdDlg = new CmdDialog(sClient);
            //clientSession->m_pCmdDlg->exec();
            clientSession->m_pCmdDlg->show();
        }
        else {
            clientSession->m_pCmdDlg->show();
        }

    }
    break;

    // case
    }
}
void RemoteControl::slot_CreateDialog(int s, int nDialogType)
{

    SOCKET sClient = (SOCKET)s;

    //通过socket 查找pClient
    QSharedPointer<CClientSession> clientSession = m_pSocket.GetClient(sClient);

    switch (nDialogType)
    {
    case CMD_CREATE:
    {
        qDebug() << "slot_CreateCmdDialog" << endl;
        if (clientSession->m_pCmdDlg == NULL) {
            clientSession->m_pCmdDlg = new CmdDialog(sClient);
            //clientSession->m_pCmdDlg->exec();
            clientSession->m_pCmdDlg->show();
        }
        else {
            clientSession->m_pCmdDlg->show();
        }

    }
    break;
    }
}