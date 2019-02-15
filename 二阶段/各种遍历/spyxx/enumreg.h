#ifndef ENUMREG_H
#define ENUMREG_H
#define MAX_VALUE_NAME 256
#include "enum.h"

class EnumReg : public Enum
{
    Q_OBJECT
public:
    EnumReg(Ui::MainWindow* ui);
    void execut();
protected:
    void EnumRegist(HKEY hKey,QTreeWidgetItem *item,bool IsExp);
    void Createkey(QString str);
    void Init();
    QString GetFullpath(QTreeWidgetItem *item);
    void SelectKey(QString str,int nId,QTreeWidgetItem *item,bool IsExp);
    int  Keyid(QString str);
    QString ValueTypes(DWORD dw);
    void Addkey(QString str,int nId);
public slots:
    void slots_DoubleClicked(QTreeWidgetItem* item,int col);
    void slots_expend(QTreeWidgetItem* item);
    void slots_add(bool ch);
    void showMenu(const QPoint &pos);
private:
    Ui::MainWindow* m_ui;
    QTreeWidgetItem* pRoot;
    QMenu* pMenu;
};

#endif // ENUMREG_H
