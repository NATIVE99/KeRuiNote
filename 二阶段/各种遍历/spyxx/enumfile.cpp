#include "enumfile.h"
#include <QIcon>
#include <Winbase.h>
enumfile::enumfile(Ui::MainWindow* ui)
{
    m_ui=ui;

}

void enumfile::slots_expend(QTreeWidgetItem *item)
{

    int n=item->childCount();
    //防止重复插入
    for(int i=0;i<n;i++)
    {
        int nAll= item->child(i)->childCount();
        for(int j=0;j<nAll;j++)
        {
            if(item->child(i)->child(j))
            {
                return;
            }
        }
    }


    if(m_ui->treeWidget->topLevelItem(0)==item)
    {

        for(int i=0;i<n;i++)
        {
            QString str= item->child(i)->text(0);
            str=str+"\\*";
            //qDebug()<<str;
            EnumFile(str,item->child(i),true);
        }
    }
    else
    {

        for(int i=0;i<n;i++)
        {
            QString Path= GetFullpath(item->child(i));
            Path=Path+"\\*";
            EnumFile(Path,item->child(i),true);
        }

    }
}

void enumfile::slots_DoubleClicked(QTreeWidgetItem *item, int col)
{
    QString load= GetFullpath(item);
    m_ui->lineEdit->setText(load);
    load=load+"\\*";
    EnumFile(load,item,false);
}


void enumfile::execut()
{

    pRoot = new QTreeWidgetItem();
    pRoot->setText(0,"Computer");
    QStyle* style = QApplication::style();
    QIcon icon = style->standardIcon(QStyle::SP_ComputerIcon);
    pRoot->setIcon(0,icon);
    m_ui->treeWidget->addTopLevelItem(pRoot);
    EnumVolume();
    m_ui->lineEdit->setText("Computer");

}

void enumfile::EnumFile(QString lpFileName,QTreeWidgetItem *item,bool IstreeEx)
{
    WIN32_FIND_DATA FindFileData={0};  // data buffer
    QListWidgetItem* pNewList=NULL;
    HANDLE hFind= FindFirstFile(lpFileName.toStdWString().c_str(),&FindFileData);
    if(!IstreeEx)
    {
        m_ui->listWidget_2->clear();
    }
    if(hFind==INVALID_HANDLE_VALUE)
    {
        FindClose(hFind);
        return;
    }
    else
    {
        do
        {
            QString str= QString::fromStdWString(FindFileData.cFileName);
            if(str==".."||str=="."){continue;}
            QTreeWidgetItem *pNewItem=new QTreeWidgetItem();
            if(!IstreeEx)
            {
                pNewList=new QListWidgetItem();

            }
            QStyle* style = QApplication::style();
            QIcon icon;
            if(FindFileData.dwFileAttributes==FILE_ATTRIBUTE_DIRECTORY)
            {
                // it is directory

                icon = style->standardIcon(QStyle::SP_DirIcon);

            }
            else
            {
                icon = style->standardIcon(QStyle::SP_FileIcon);
            }
            if(IstreeEx)
            {
                pNewItem->setText(0,str);
                pNewItem->setIcon(0,icon);
                item->addChild(pNewItem);

            }
            else
            {

                pNewList->setText(str);
                pNewList->setIcon(icon);
                m_ui->listWidget_2->addItem(pNewList);
            }

            //qDebug()<<str;

        }while(FindNextFile(hFind,&FindFileData));
    }
}

void enumfile::EnumVolume()
{
    TCHAR szbuf[1024]={0};

    GetLogicalDriveStrings(1024*sizeof(TCHAR),szbuf);
    int n=0;
    do
    {

        szbuf[n+2]=0;
        QString str=QString::fromStdWString(&szbuf[n]);
        QTreeWidgetItem* pDrive = new QTreeWidgetItem();
        pDrive->setText(0,str);
        QStyle* style = QApplication::style();
        QIcon icon = style->standardIcon(QStyle::SP_DriveHDIcon);
        pDrive->setIcon(0,icon);
        pRoot->addChild(pDrive);

        //init listwidegt

        QListWidgetItem* item=new QListWidgetItem();
        item->setText(str);
        item->setIcon(icon);
        m_ui->listWidget_2->addItem(item);
        //str=str+"*";
        // EnumFile(str);
        n=n+4;


    }while(szbuf[n]);


}

QString enumfile::GetFullpath(QTreeWidgetItem *item)
{
    QString str=item->text(0);
    QTreeWidgetItem* pTemp=item;
    while(pTemp!=pRoot)
    {

        pTemp=pTemp->parent();
        if(pTemp==pRoot)
        {
            //qDebug()<<str;
            return str;
        }
        str=pTemp->text(0)+"\\"+str;
    }


}
