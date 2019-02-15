#include "enumserive.h"
#include <QMessageBox>
EnumSerive::EnumSerive(Ui::MainWindow* ui)
{
    m_ui=ui;
}

void EnumSerive::execut()
{
    EnumService();
}

void EnumSerive::slots_start(bool bRet)
{
    int nRow=m_ui->tableWidget_2->currentRow();
    QString ServiceName = m_ui->tableWidget_2->item(nRow,1)->text();
    SC_HANDLE sHandle=OpenService(schSCManager, ServiceName.toStdWString().c_str(),SERVICE_START);
    if(sHandle==NULL)
    {
        QMessageBox msgBox;
        msgBox.setText("START FAIL");
        msgBox.exec();
        return;
    }

   BOOL bRet1=StartService(sHandle,0,NULL);
   if(bRet1!=0)
   {
       m_ui->tableWidget_2->item(nRow,2)->setText("Run");
   }
}

void EnumSerive::slots_end(bool bRet)
{
    SERVICE_STATUS   ServiceStatus;
    SC_HANDLE sHandle;
    int nRow=m_ui->tableWidget_2->currentRow();
    QString ServiceName = m_ui->tableWidget_2->item(nRow,1)->text();

    sHandle=OpenService(schSCManager, ServiceName.toStdWString().c_str(),SERVICE_STOP);

    BOOL bRet1=ControlService(sHandle,SERVICE_CONTROL_STOP,&ServiceStatus);
    if(bRet1!=0)
    {
        m_ui->tableWidget_2->item(nRow,2)->setText(" ");
    }
}

void EnumSerive::EnumService()
{


      schSCManager = OpenSCManager(
                NULL,                    // local machine
                NULL,                    // ServicesActive database
                SC_MANAGER_ALL_ACCESS);  // full access rights

    //The lpServices parameter is a pointer to a buffer that receives an array of ENUM_SERVICE_STATUS_PROCESS structures.
    ENUM_SERVICE_STATUS_PROCESS*   lpServices=NULL;//一个数组的buff

    DWORD dwNeed=0;
    DWORD dwServicesReturned = 0;
    //返回信息要的数组大小
    bool bRet=EnumServicesStatusEx(schSCManager,
                                   SC_ENUM_PROCESS_INFO, SERVICE_WIN32,
                                   SERVICE_STATE_ALL, (LPBYTE)lpServices,
                                   0,
                                   &dwNeed,//需要多少字节的空间
                                   &dwServicesReturned,//返回的个数
                                   NULL,
                                   NULL);

    DWORD dwBufSize = 0;
    dwBufSize = dwNeed;

    TCHAR* pBuf = new TCHAR[dwBufSize];
    TCHAR* pAryOrg = pBuf;

    memset(pBuf, 0, dwBufSize);

    bRet = EnumServicesStatusEx(schSCManager, SC_ENUM_PROCESS_INFO, SERVICE_WIN32, SERVICE_STATE_ALL,
                                (LPBYTE)pBuf, dwBufSize, &dwNeed, &dwServicesReturned, NULL, NULL);


    lpServices = (LPENUM_SERVICE_STATUS_PROCESS)pBuf;


    for (unsigned int i = 0; i < dwServicesReturned; i++)
    {
        QString lpDisplayName=QString::fromStdWString(lpServices[i].lpDisplayName);
        QString lpServiceName=QString::fromStdWString(lpServices[i].lpServiceName);
        QString ctrid=QString("%1").arg(lpServices[i].ServiceStatusProcess.dwControlsAccepted);
          m_ui->tableWidget_2->insertRow(i);
        if(lpServices[i].ServiceStatusProcess.dwCurrentState==SERVICE_RUNNING)
        {
            m_ui->tableWidget_2->setItem(i,2,new QTableWidgetItem("Run"));
        }
        else
        {
             m_ui->tableWidget_2->setItem(i,2,new QTableWidgetItem(" "));
        }
        m_ui->tableWidget_2->setItem(i,3,new QTableWidgetItem(ctrid));
        m_ui->tableWidget_2->setItem(i,0,new QTableWidgetItem(lpDisplayName));
        m_ui->tableWidget_2->setItem(i,1,new QTableWidgetItem(lpServiceName));


    }
    delete[] pAryOrg;

}

int EnumSerive::FindCtrlon(int fdwControl)
{
    int fdwAccess=-1;
    switch (fdwControl)
       {
           case SERVICE_CONTROL_STOP:
               fdwAccess = SERVICE_STOP;
               break;

           case SERVICE_CONTROL_PAUSE:
           case SERVICE_CONTROL_CONTINUE:
               fdwAccess = SERVICE_PAUSE_CONTINUE;
               break;

           case SERVICE_CONTROL_INTERROGATE:
               fdwAccess = SERVICE_INTERROGATE;
               break;

           default:
               fdwAccess = SERVICE_INTERROGATE;
       }
    return fdwAccess;
}

