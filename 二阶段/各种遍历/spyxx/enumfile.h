#ifndef ENUMFILE_H
#define ENUMFILE_H
#include "enum.h"

class enumfile : public Enum
{
     Q_OBJECT
public:
   void execut();
    enumfile(Ui::MainWindow* ui);
 signals:
    void SendStr(QString str);
public slots:
    void slots_expend(QTreeWidgetItem* item);
    void slots_DoubleClicked(QTreeWidgetItem* item,int col);
protected:
   void EnumFile(QString lpFileName,QTreeWidgetItem *item,bool IstreeEx);
   void EnumVolume();
   QString GetFullpath(QTreeWidgetItem *item);
private:
   Ui::MainWindow* m_ui;
   QTreeWidgetItem* pRoot;




};

#endif // ENUMFILE_H
