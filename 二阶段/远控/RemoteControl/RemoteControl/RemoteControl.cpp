#include "RemoteControl.h"
#include <qdebug>
extern RemoteControl* g_pWindow;
RemoteControl::RemoteControl(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    g_pWindow = this;
    //����Widegt���ܱ��Ŵ�
    setFixedSize(width(), height());
    //����tablewidget��ͷ
    ui.tableWidget->setColumnCount(1);
    QStringList head;
    head << "ip";
    ui.tableWidget->setHorizontalHeaderLabels(head);


    //�����Ҽ���
    ui.tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);//����tablewidegt�˵����
    m_pMeun.reset(new QMenu());
    QAction* pCmdAction=m_pMeun->addAction(QStringLiteral("cmd"));
    QAction* pGreenAction= m_pMeun->addAction(QStringLiteral("��Ļ"));
    //�����Ҽ�
    connect(ui.tableWidget,
        SIGNAL(customContextMenuRequested(const QPoint&)),
        this,
        SLOT(slot_ShowMenu(const QPoint&)));
  //�Ҽ��˵�������
    QObject::connect(pCmdAction,SIGNAL(triggered()),this,SLOT(slot_ShowCmd()));
    QObject::connect(pGreenAction, SIGNAL(triggered()), this, SLOT(slot_ShowGreen()));

   
    //��ʼ��socket

    m_pSocket.StartUp();
    m_pSocket.CreateSocketAndBind();

    //��socketrsever�ź�
    QObject::connect(&m_pSocket, SIGNAL(sendClinentInfo(CClientSession* )), this, SLOT(HandleClinentInfo(CClientSession* )));
    //accpet�߳�����
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
//�������ݣ�֪ͨ�ͻ��˿���cmd����
//��ȡ��ǰ��ѡ�е�item
//QTableWidgetItem * pItem = ui->tableWidget->currentItem();

    int nIndex = ui.tableWidget->currentRow();
    QTableWidgetItem * pItem = ui.tableWidget->item(nIndex, 0);

    SOCKET sClient = pItem->data(Qt::UserRole).toInt();

    //��ͻ��˷��ʹ�cmd������

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

    //ͨ��socket ����pClient
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

    //ͨ��socket ����pClient
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