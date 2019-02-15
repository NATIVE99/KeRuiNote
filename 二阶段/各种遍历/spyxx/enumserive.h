#ifndef ENUMSERIVE_H
#define ENUMSERIVE_H
#include "enum.h"

class EnumSerive : public Enum
{
      Q_OBJECT
public:
    EnumSerive(Ui::MainWindow* ui);
     void execut();
public slots:
     void slots_start(bool bRet);
     void slots_end(bool bRet);
protected:
     void EnumService();
     int  FindCtrlon(int fdwControl);
private:
    Ui::MainWindow* m_ui;
    SC_HANDLE  schSCManager;
};

#endif // ENUMSERIVE_H
