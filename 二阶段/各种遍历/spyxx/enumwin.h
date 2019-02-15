#ifndef ENUMWIN_H
#define ENUMWIN_H

#include "enum.h"

class EnumWin : public Enum
{
    Q_OBJECT
public:
    EnumWin(Ui::MainWindow* ui);

    void execut();
protected:
    void EnumWindow();


private:
    Ui::MainWindow* m_ui;

};

#endif // ENUMWIN_H
