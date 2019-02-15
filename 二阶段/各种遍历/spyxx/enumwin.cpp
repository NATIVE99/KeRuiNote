#include "enumwin.h"
QTreeWidgetItem* pRoot;
EnumWin::EnumWin(Ui::MainWindow* ui)
{
    m_ui=ui;
}

void EnumWin::execut()
{
    HWND hwnd= GetDesktopWindow();
    char szTitle[MAX_PATH] = {0};
    char szClass[MAX_PATH] = {0};
    int nMaxCount = MAX_PATH;

    LPSTR lpClassName = szClass;
    LPSTR lpWindowName = szTitle;
    GetWindowTextA(hwnd, lpWindowName, nMaxCount);
    GetClassNameA(hwnd, lpClassName, nMaxCount);

    pRoot = new QTreeWidgetItem();
    QString str=QString("%1%2%3%4").arg(tr("窗口")).arg(QString::number((int)hwnd,16)).arg(lpWindowName).arg(lpClassName);
    pRoot->setText(0,str);
    m_ui->treeWidget_2->addTopLevelItem(pRoot);

    EnumWindow();
}

BOOL CALLBACK EnumChildProc(
        HWND hwnd,      // handle to child window
        LPARAM lParam   // application-defined value
        )
{
    QTreeWidgetItem *pItem=(QTreeWidgetItem *)lParam;

    char szTitle[MAX_PATH] = {0};
    char szClass[MAX_PATH] = {0};
    int nMaxCount = MAX_PATH;

    LPSTR lpClassName = szClass;
    LPSTR lpWindowName = szTitle;

    GetWindowTextA(hwnd, lpWindowName, nMaxCount);
    GetClassNameA(hwnd, lpClassName, nMaxCount);

    QString str=QString("%1%2%3%4").arg(QStringLiteral("窗口")).arg(QString::number((int)hwnd,16)).arg(lpWindowName).arg(lpClassName);
    QTreeWidgetItem *pNewItem=new QTreeWidgetItem();

    pNewItem->setText(0, str);
    pItem->addChild(pNewItem);
    return TRUE;

}


BOOL CALLBACK EnumWindowsProc(
        HWND hwnd,      // handle to parent window
        LPARAM lParam   // application-defined value
        )
{
    char szTitle[MAX_PATH] = {0};
    char szClass[MAX_PATH] = {0};
    int nMaxCount = MAX_PATH;

    LPSTR lpClassName = szClass;
    LPSTR lpWindowName = szTitle;

    GetWindowTextA(hwnd, lpWindowName, nMaxCount);
    GetClassNameA(hwnd, lpClassName, nMaxCount);

    QString str=QString("%1%2%3%4").arg(QStringLiteral("窗口")).arg(QString::number((int)hwnd,16)).arg(lpWindowName).arg(lpClassName);
    QTreeWidgetItem *pNewItem=new QTreeWidgetItem();

    pNewItem->setText(0, str);
    pRoot->addChild(pNewItem);

    EnumChildWindows(hwnd,EnumChildProc,(LPARAM)pNewItem);
    return TRUE;

}

void EnumWin::EnumWindow()
{
    EnumWindows(EnumWindowsProc,0);

}

