#include "processdialog.h"
#include "ui_processdialog.h"
#include <qdebug.h>
ProcessDialog::ProcessDialog(SOCKET s, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ProcessDialog)
{
    ui->setupUi(this);
    m_cliend = s;
    ui->tableWidget->setColumnCount(2);
    ui->tableWidget->setColumnWidth(0, 250);
    QStringList str;
    str << "process" << "pid";
    ui->tableWidget->setHorizontalHeaderLabels(str);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);

    m_PMenu = new QMenu;
    QAction* pEnd = new QAction(QStringLiteral("½áÊø½ø³Ì"));

    m_PMenu->addAction(pEnd);

    QObject::connect(ui->tableWidget, SIGNAL(customContextMenuRequested(const QPoint)), this, SLOT(slots_ShowMenu(const QPoint)));
    QObject::connect(pEnd, SIGNAL(triggered(bool)), this, SLOT(slots_sendEnd(bool)));
}

ProcessDialog::~ProcessDialog()
{
    delete ui;
}

struct tagData
{
    int nPid;
    int nLength;
    char  data[];
};

void ProcessDialog::handleResults(QByteArray arr)
{
    int nRow = 0;
    while(!arr.isEmpty())
    {
        tagData* ptag =(tagData*)arr.data();
        int nPid = ptag->nPid;
        int nLength = ptag->nLength;

        char* pDate = new char[nLength+2];
        memset(pDate, 0, nLength + 2);
        memcpy(pDate, arr.data() + sizeof(tagData), nLength);
        QString str = QString::fromWCharArray((wchar_t*)pDate);
        QString pid = QString("%1").arg(nPid);

        ui->tableWidget->insertRow(nRow);
        QTableWidgetItem* pItem = new QTableWidgetItem(str);
        pItem->setData(Qt::UserRole, nPid);
        ui->tableWidget->setItem(nRow, 0, pItem);

        ui->tableWidget->setItem(nRow, 1, new QTableWidgetItem(pid)); 

        arr.remove(0, nLength + sizeof(tagData));
        nRow++;
    }
}

void ProcessDialog::slots_ShowMenu(const QPoint & pos)
{
    m_PMenu->exec(QCursor::pos());
}

void ProcessDialog::slots_sendEnd(bool b)
{
    int nIndex = ui->tableWidget->currentRow();
    QTableWidgetItem * pItem = ui->tableWidget->item(nIndex, 0);
    QByteArray arr = pItem->data(Qt::UserRole).toByteArray();
    SendData2(m_cliend, CUT_END, arr.data(), arr.size());

}

void  ProcessDialog::slots_deleteProcess(QByteArray arr)
{
    QString str = QString(arr);
    int nRow =ui->tableWidget->rowCount();
    for(int i=0;i<nRow-1;i++)
    {
       QString pId=ui->tableWidget->takeItem(i, 1)->text();
        if(pId ==str)
        {
            ui->tableWidget->removeRow(i);
            break;
        }
    }

}