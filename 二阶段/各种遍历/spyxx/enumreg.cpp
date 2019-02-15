#include "enumreg.h"

EnumReg::EnumReg(Ui::MainWindow* ui)
{
    m_ui=ui;
   pMenu=new QMenu(m_ui->tableWidget_3);
   QAction* pAdd=pMenu->addAction("ADD");
   QObject::connect(m_ui->treeWidget_4,SIGNAL(itemExpanded(QTreeWidgetItem*)),this,SLOT(slots_expend(QTreeWidgetItem*)));
   QObject::connect(m_ui->treeWidget_4,SIGNAL(itemDoubleClicked(QTreeWidgetItem * , int)),this,SLOT(slots_DoubleClicked(QTreeWidgetItem*,int)));
   QObject::connect(m_ui->tableWidget_3,SIGNAL(customContextMenuRequested(const QPoint &)),this,SLOT(showMenu(const QPoint &)));
   QObject::connect(pAdd, SIGNAL(triggered(bool)),this,SLOT(slots_add(bool)));
}

void EnumReg::execut()
{

    Init();
}

void EnumReg::EnumRegist(HKEY hKey,QTreeWidgetItem *item,bool IsExp)
{

    CHAR     achKey[MAX_PATH];
    CHAR     achClass[MAX_PATH] = {0};  // buffer for class name
    DWORD    cchClassName = MAX_PATH;  // length of class string
    DWORD    cSubKeys;                 // number of subkeys
    DWORD    cbMaxSubKey;              // longest subkey size
    DWORD    cchMaxClass;              // longest class string
    DWORD    cValues;              // number of values for key
    DWORD    cchMaxValue;          // longest value name
    DWORD    cbMaxValueData;       // longest value data
    DWORD    cbSecurityDescriptor; // size of security descriptor
    FILETIME ftLastWriteTime;      // last write time

    DWORD i, j;
    DWORD retCode, retValue;

    CHAR  achValue[MAX_VALUE_NAME] = {0};
    DWORD cchValue = MAX_VALUE_NAME;
    DWORD dwType=0;
    CHAR  achBuff[80] = {0};



    int nRow=0;
    // Get the class name and the value count.
    RegQueryInfoKeyA(hKey,        // key handle
        achClass,                // buffer for class name
        &cchClassName,           // length of class string
        NULL,                    // reserved
        &cSubKeys,               // number of subkeys
        &cbMaxSubKey,            // longest subkey size
        &cchMaxClass,            // longest class string
        &cValues,                // number of values for this key
        &cchMaxValue,            // longest value name
        &cbMaxValueData,         // longest value data
        &cbSecurityDescriptor,   // security descriptor
        &ftLastWriteTime);       // last write time



    // Enumerate the child keys, until RegEnumKeyEx fails. Then
    // get the name of each child key and copy it into the list box.

    for (i = 0, retCode = ERROR_SUCCESS;
    retCode == ERROR_SUCCESS; i++)
    {
        DWORD cName = MAX_PATH;
        retCode = RegEnumKeyExA(hKey,
            i,
            achKey,
            &cName,
            NULL,
            NULL,
            NULL,
            &ftLastWriteTime);
        if (retCode == (DWORD)ERROR_SUCCESS&&IsExp)
        {
            //printf("%s\r\n", achKey);
            QTreeWidgetItem* pNew=new QTreeWidgetItem();
            pNew->setText(0,achKey);
            QStyle* style = QApplication::style();
            QIcon icon = style->standardIcon(QStyle::SP_DirIcon);
            pNew->setIcon(0,icon);
            item->addChild(pNew);
        }
    }

    if (cValues&&(!IsExp))
    {


        for (j = 0, retValue = ERROR_SUCCESS;
        j < cValues; j++)
        {
            cchValue = MAX_VALUE_NAME;
            achValue[0] = '\0';
            retValue = RegEnumValueA(hKey, j, achValue,
                &cchValue,
                NULL,
                &dwType,    // &dwType,
                NULL,    // &bData,
                NULL);   // &bcData

            if (retValue == (DWORD)ERROR_SUCCESS)
            {

              QString str=QString::fromStdString(achValue);
              QString type=ValueTypes(dwType);
              m_ui->tableWidget_3->insertRow(nRow);
              m_ui->tableWidget_3->setItem(nRow,0,new QTableWidgetItem(str));
              m_ui->tableWidget_3->setItem(nRow,1,new QTableWidgetItem(type));
              nRow++;

            }

            achBuff[0] = '\0';
        }
    }


}

void EnumReg::Createkey(QString str)
{
QTreeWidgetItem*  pNew = new QTreeWidgetItem();
    pNew->setText(0,str);
    QStyle* style = QApplication::style();
    QIcon icon = style->standardIcon(QStyle::SP_DirIcon);
    pNew->setIcon(0,icon);
    pRoot->addChild(pNew);
}

void EnumReg::Init()
{
    m_ui->tableWidget_3->setColumnCount(3);
    m_ui->tableWidget_3->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_ui->tableWidget_3->setSelectionBehavior(QAbstractItemView::SelectRows);
    QStringList str;
    str<<"DisplayName"<<"Type"<<"Date";
    m_ui->tableWidget_3->setHorizontalHeaderLabels(str);

    pRoot = new QTreeWidgetItem();
    pRoot->setText(0,"Reg");
    QStyle* style = QApplication::style();
    QIcon icon = style->standardIcon(QStyle::SP_ComputerIcon);
    pRoot->setIcon(0,icon);
    m_ui->treeWidget_4->addTopLevelItem(pRoot);

    m_ui->lineEdit_3->setText("Reg");

    Createkey("HKEY_CLASSES_ROOT");
    Createkey("HKEY_CURRENT_CONFIG");
    Createkey("HKEY_CURRENT_USER");
    Createkey("HKEY_LOCAL_MACHINE");
    Createkey("HKEY_USERS");

}

void EnumReg::slots_expend(QTreeWidgetItem *item)
{
    QString str=GetFullpath(item);
    int id=0;
    int nCount=item->childCount();
    //qDebug()<<str;
    //防止重复插入
    for(int i=0;i<nCount;i++)
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
    if(str=="Reg")
    {
        id=Keyid("HKEY_CLASSES_ROOT");
        SelectKey(NULL,id,item->child(0),true);
        id=Keyid("HKEY_CURRENT_CONFIG");
        SelectKey(NULL,id,item->child(1),true);
        id=Keyid("HKEY_CURRENT_USER");
        SelectKey(NULL,id,item->child(2),true);
        id=Keyid("HKEY_LOCAL_MACHINE");
        SelectKey(NULL,id,item->child(3),true);
        id=Keyid("HKEY_USERS");
        SelectKey(NULL,id,item->child(4),true);

    }
    else
    {
        id=Keyid(str);

        QString Newp;
        for(int i=0;i<nCount;i++)
        {
            Newp=str+"\\"+item->child(i)->text(0);
            qDebug()<<i<<"----"<<str;
            SelectKey(Newp,id,item->child(i),true);
        }

    }

}

void EnumReg::slots_add(bool ch)
{
    QString str=m_ui->lineEdit_3->text();
   int nId= Keyid(str);
   Addkey(str,nId);
}

void EnumReg::showMenu(const QPoint &pos)
{
        pMenu->exec(QCursor::pos());
}
QString EnumReg::GetFullpath(QTreeWidgetItem *item)
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

void EnumReg::SelectKey(QString str,int nId,QTreeWidgetItem *item,bool IsExp)
{

    switch(nId)
    {
    case 0:
    {
        HKEY hKey;
        str.replace("HKEY_CLASSES_ROOT\\","");
        RegOpenKeyExA(HKEY_CLASSES_ROOT,
            str.toStdString().c_str(),
            0,
            KEY_ALL_ACCESS,
            &hKey);

        EnumRegist(hKey,item,IsExp);

        RegCloseKey(hKey);
    }
    break;
    case 1:
    {
        HKEY hKey;
        str.replace("HKEY_CURRENT_CONFIG\\","");
        RegOpenKeyExA(HKEY_CURRENT_CONFIG,
             str.toStdString().c_str(),
            0,
            KEY_ALL_ACCESS,
            &hKey);

        EnumRegist(hKey,item,IsExp);

        RegCloseKey(hKey);
    }
    break;
    case 2:
    {
        HKEY hKey;
         str.replace("HKEY_CURRENT_USER\\","");
        RegOpenKeyExA(HKEY_CURRENT_USER,
             str.toStdString().c_str(),
            0,
            KEY_ALL_ACCESS,
            &hKey);

          EnumRegist(hKey,item,IsExp);

        RegCloseKey(hKey);
    }
    break;
    case 3:
    {
        HKEY hKey;
            str.replace("HKEY_LOCAL_MACHINE\\","");
            qDebug()<<str;
        RegOpenKeyExA(HKEY_LOCAL_MACHINE,
             str.toStdString().c_str(),
            0,
            KEY_ALL_ACCESS,
            &hKey);

        EnumRegist(hKey,item,IsExp);

        RegCloseKey(hKey);
    }
    break;
    case 4:
    {
        HKEY hKey;
            str.replace("HKEY_USERS\\","");
        RegOpenKeyExA(HKEY_USERS,
             str.toStdString().c_str(),
            0,
            KEY_ALL_ACCESS,
            &hKey);

         EnumRegist(hKey,item,IsExp);

        RegCloseKey(hKey);
    }
    break;

    }
}

int EnumReg::Keyid(QString str)
{
    int nId=-1;

    if(str.contains("HKEY_CLASSES_ROOT"))
    {
        nId=0;
    }else if(str.contains("HKEY_CURRENT_CONFIG"))
    {
        nId=1;

    }else if(str.contains("HKEY_CURRENT_USER"))
    {
        nId=2;
    }else if(str.contains("HKEY_LOCAL_MACHINE"))
    {
        nId=3;
    }else if(str.contains("HKEY_USERS"))
    {
        nId=4;
    }
    return nId;
}

QString EnumReg::ValueTypes(DWORD dw)
{
    QString str;
    switch(dw)
    {
    case REG_BINARY:
    {
        str="REG_BINARY";
    }
        break;
    case REG_DWORD:
    {
        str="REG_DWORD";
    }
        break;
    case REG_EXPAND_SZ:
    {
        str="REG_EXPAND_SZ";
    }
        break;
    case REG_SZ:
    {
        str="REG_SZ";
    }
        break;
    }
    return str;
}

void EnumReg::Addkey(QString str,int nId)
{
    char szbuf[8]="text";
    switch(nId)
    {
    case 0:
    {
        HKEY hKey;
        str.replace("HKEY_CLASSES_ROOT\\","");
        RegOpenKeyExA(HKEY_CLASSES_ROOT,
            str.toStdString().c_str(),
            0,
            KEY_ALL_ACCESS,
            &hKey);
        RegSetValueExA(hKey,      // subkey handle
                    "Text",  // value name
                    0,                 // must be zero
                    REG_SZ,         // value type
                    (LPBYTE)szbuf,  // pointer to value data
                     8) ;   // length of value data

        RegCloseKey(hKey);
    }
    break;
    case 1:
    {
        HKEY hKey;
        str.replace("HKEY_CURRENT_CONFIG\\","");
        RegOpenKeyExA(HKEY_CURRENT_CONFIG,
             str.toStdString().c_str(),
            0,
            KEY_ALL_ACCESS,
            &hKey);
        RegSetValueExA(hKey,      // subkey handle
                    "Text",  // value name
                    0,                 // must be zero
                    REG_SZ,         // value type
                    (LPBYTE)szbuf,  // pointer to value data
                     8) ;   // length of value data

        RegCloseKey(hKey);
    }
    break;
    case 2:
    {
        HKEY hKey;
         str.replace("HKEY_CURRENT_USER\\","");
        RegOpenKeyExA(HKEY_CURRENT_USER,
             str.toStdString().c_str(),
            0,
            KEY_ALL_ACCESS,
            &hKey);
        RegSetValueExA(hKey,      // subkey handle
                    "Text",  // value name
                    0,                 // must be zero
                    REG_SZ,         // value type
                    (LPBYTE)szbuf,  // pointer to value data
                     8) ;   // length of value data


        RegCloseKey(hKey);
    }
    break;
    case 3:
    {
        HKEY hKey;
            str.replace("HKEY_LOCAL_MACHINE\\","");
            qDebug()<<str;
        RegOpenKeyExA(HKEY_LOCAL_MACHINE,
             str.toStdString().c_str(),
            0,
            KEY_ALL_ACCESS,
            &hKey);

        RegSetValueExA(hKey,      // subkey handle
                    "Text",  // value name
                    0,                 // must be zero
                    REG_SZ,         // value type
                    (LPBYTE)szbuf,  // pointer to value data
                     8) ;   // length of value data

        RegCloseKey(hKey);
    }
    break;
    case 4:
    {
        HKEY hKey;
            str.replace("HKEY_USERS\\","");
        RegOpenKeyExA(HKEY_USERS,
             str.toStdString().c_str(),
            0,
            KEY_ALL_ACCESS,
            &hKey);

        RegSetValueExA(hKey,      // subkey handle
                    "Text",  // value name
                    0,                 // must be zero
                    REG_SZ,         // value type
                    (LPBYTE)szbuf,  // pointer to value data
                     8) ;   // length of value data

        RegCloseKey(hKey);
    }
    break;

    }
}

void EnumReg::slots_DoubleClicked(QTreeWidgetItem *item, int col)
{
    QString load= GetFullpath(item);
    m_ui->lineEdit_3->setText(load);

    int delRow=m_ui->tableWidget_3->rowCount();
    if(delRow>0)
    {
        do
        {
            m_ui->tableWidget_3->removeRow(delRow);
        }while(delRow--);
    }
    int nId=Keyid(load);
    SelectKey(load,nId,item,false);
}


