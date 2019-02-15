#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QAction>
#include <QScrollBar>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    InitTablewidget();
    InitFileEnum();
    InitService();
    EnumWin* pEnumWin=new EnumWin(ui);
    pEnumWin->execut();

    EnumReg* pEnumReg=new EnumReg(ui);
    pEnumReg->execut();
    ui->treeWidget_4 -> header() -> setMinimumSectionSize(500);
   // spyxx* pSpyxx=new spyxx(ui,ui->label_3);
  QObject::connect(ui->label_3, SIGNAL(SendHwnd(int)),this,SLOT(slots_RecvHwnd(int)));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::InitTablewidget()
{
    ui->tableWidget->setColumnCount(2);
    ui->tableWidget->setColumnWidth(0,250);
    QStringList str;
    str<<"process"<<"pid";
    ui->tableWidget->setHorizontalHeaderLabels(str);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);

    EnumPid* pEnumPid=new EnumPid(ui);
    Enum* pEnum=pEnumPid;
    pEnum->execut();
    pProcessMenu=new QMenu(ui->tableWidget);

    QAction* pMondule= pProcessMenu->addAction("Mondule");
    QAction* pThread=pProcessMenu->addAction("Thread");

    QObject::connect(ui->tableWidget,SIGNAL(customContextMenuRequested(const QPoint &)),this,SLOT(showMenu(const QPoint &)));
    QObject::connect(pMondule, SIGNAL(triggered(bool)),pEnumPid,SLOT(FindMonudle(bool)));
    QObject::connect(pThread, SIGNAL(triggered(bool)),pEnumPid,SLOT(FindThread(bool)));
}

void MainWindow::InitFileEnum()
{
    enumfile* pEnumfile=new enumfile(ui);

    ui->listWidget_2->setFixedWidth(300);
    Enum* pEnum= pEnumfile;
    pEnum->execut();
    ui->treeWidget -> header() -> setMinimumSectionSize(500);
    QObject::connect(ui->treeWidget,SIGNAL(itemExpanded(QTreeWidgetItem*)),pEnumfile,SLOT(slots_expend(QTreeWidgetItem*)));

    QObject::connect(ui->treeWidget,SIGNAL(itemDoubleClicked(QTreeWidgetItem * , int)),pEnumfile,SLOT(slots_DoubleClicked(QTreeWidgetItem*,int)));

}

void MainWindow::InitService()
{
    ui->tableWidget_2->setColumnCount(4);
    ui->tableWidget_2->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget_2->setSelectionBehavior(QAbstractItemView::SelectRows);
    QStringList str;
    str<<"lpDisplayName"<<"lpServiceName"<<"statue"<<"ctrid";
    ui->tableWidget_2->setHorizontalHeaderLabels(str);
    ui->tableWidget_2->setColumnWidth(0,170);
    EnumSerive* pEnumSerive=new EnumSerive(ui);
    Enum* pEnum= pEnumSerive;
    pEnum->execut();

    pServiceMenu=new QMenu(ui->tableWidget_2);

    pStart= pServiceMenu->addAction("start");
    pEnd=pServiceMenu->addAction("end");
    QObject::connect(ui->tableWidget_2,SIGNAL(customContextMenuRequested(const QPoint &)),this,SLOT(showServiceMenu(const QPoint &)));
    QObject::connect(pStart, SIGNAL(triggered(bool)),pEnumSerive,SLOT(slots_start(bool)));
    QObject::connect(pEnd, SIGNAL(triggered(bool)),pEnumSerive,SLOT(slots_end(bool)));

}

void MainWindow::showMenu(const QPoint &pos)
{

    pProcessMenu->exec(QCursor::pos());
}

void MainWindow::showServiceMenu(const QPoint &pos)
{
    int nRow=ui->tableWidget_2->currentRow();
    QString ServiceName = ui->tableWidget_2->item(nRow,1)->text();
    QString statue=ui->tableWidget_2->item(nRow,2)->text();
    if(statue=="Run")
    {
        pStart->setEnabled(false);
    }
    else
    {
        pEnd->setEnabled(false);
    }
    pServiceMenu->exec(QCursor::pos());
    pStart->setEnabled(true);
    pEnd->setEnabled(true);
}

void MainWindow::slots_RecvHwnd(int hwnd)
{
    HWND hnd=(HWND)(hwnd);

    TCHAR szbuf[MAX_PATH]={0};
    TCHAR szbuf2[MAX_PATH]={0};
    QString str=QString("%1").arg(QString::number((int)hwnd,16));
    GetWindowText(hnd,szbuf,MAX_PATH);
    GetClassName (hnd,szbuf2,MAX_PATH);
    ui->lineEdit_2->setText(str);
    ui->lineEdit_4->setText(QString::fromStdWString(szbuf2));
    ui->lineEdit_5->setText(QString::fromStdWString(szbuf));

}
