#ifndef ENUMPID_H
#define ENUMPID_H

#include "enum.h"
#include "dialog.h"

class EnumPid : public Enum
{
        Q_OBJECT
public:
    EnumPid( Ui::MainWindow* ui);
    void execut();
   public slots:
    void FindMonudle(bool ch);
    void FindThread(bool ch);
private:
    Ui::MainWindow* m_ui;
    Dialog* pDia;
    QStringList str ;

};

#endif // ENUMPID_H
