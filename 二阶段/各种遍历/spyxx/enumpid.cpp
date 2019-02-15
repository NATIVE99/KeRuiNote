#include "enumpid.h"

#include <Tlhelp32.h.>
#include <QTableWidgetItem>
#include <Tlhelp32.h>
#include "ui_dialog.h"
EnumPid::EnumPid( Ui::MainWindow* ui)
{
    m_ui=ui;
    pDia=new Dialog();

    //str<<"module";
    //pDia->ui->tableWidget->setHorizontalHeaderLabels(str);

}

void EnumPid::execut()
{
    

    HANDLE         hProcessSnap = NULL;
    BOOL           bRet      = FALSE;
    PROCESSENTRY32 pe32      = {0};
    int nRow=0;
    hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    pe32.dwSize=sizeof(PROCESSENTRY32 );



    if(Process32First(hProcessSnap,&pe32))
    {

        do
        {
            QString str= QString::fromWCharArray(pe32.szExeFile);
            QString pid=QString("%1").arg(pe32.th32ProcessID);
            m_ui->tableWidget->insertRow(nRow);
            m_ui->tableWidget->setItem(nRow,0,new QTableWidgetItem(str));
            m_ui->tableWidget->setItem(nRow,1,new QTableWidgetItem(pid));
            nRow++;
        }while(Process32Next(hProcessSnap,&pe32));
    }

    CloseHandle (hProcessSnap);


}

void EnumPid::FindMonudle(bool ch)
{

    HANDLE         hProcessSnap = NULL;
    BOOL           bRet      = FALSE;
    MODULEENTRY32  pe32      = {0};
    int nRow=0;
    QTableWidgetItem* p= m_ui->tableWidget->currentItem();
    QString str=p->text();
    hProcessSnap=CreateToolhelp32Snapshot(TH32CS_SNAPMODULE,str.toInt());
    pe32.dwSize=sizeof(MODULEENTRY32  );

    if( Module32First(hProcessSnap,&pe32))
    {

        do
        {
            QString str= QString::fromWCharArray(pe32.szModule);

            pDia->ui->tableWidget->insertRow(nRow);
            pDia->ui->tableWidget->setItem(nRow,0,new QTableWidgetItem(str));
            nRow++;
            //qDebug()<<str;

        }while(Module32Next(hProcessSnap,&pe32));

    }
    else
    {
        CloseHandle (hProcessSnap);
        pDia->ui->tableWidget->insertRow(nRow);
        pDia->ui->tableWidget->setItem(nRow,0,new QTableWidgetItem("NO find"));
    }

    pDia->exec();
    do
    {
         pDia->ui->tableWidget->currentRow();
        pDia->ui->tableWidget->removeRow(nRow);
    }while(nRow--);



}

void EnumPid::FindThread(bool ch)
{
    HANDLE         hProcessSnap = NULL;
    BOOL           bRet      = FALSE;
    THREADENTRY32   pe32      = {0};
    int nRow=0;
    QTableWidgetItem* p= m_ui->tableWidget->currentItem();
    QString str=p->text();
    pe32.dwSize=sizeof(THREADENTRY32);

    hProcessSnap=CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD,str.toInt());

    if( Thread32First(hProcessSnap,&pe32))
    {

        do
        {

            QString pid=QString("%1").arg(pe32.th32ThreadID);
            pDia->ui->tableWidget->insertRow(nRow);
            pDia->ui->tableWidget->setItem(nRow,0,new QTableWidgetItem(pid));
            nRow++;
            //qDebug()<<str;

        }while(Thread32Next(hProcessSnap,&pe32));

    }
    else
    {
        CloseHandle (hProcessSnap);
        pDia->ui->tableWidget->insertRow(nRow);
        pDia->ui->tableWidget->setItem(nRow,0,new QTableWidgetItem("NO find"));
    }

    pDia->exec();
    do
    {
        pDia->ui->tableWidget->removeRow(nRow);
    }while(nRow--);


}
