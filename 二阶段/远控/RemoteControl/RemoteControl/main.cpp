#include "RemoteControl.h"
#include <QtWidgets/QApplication>
RemoteControl* g_pWindow;
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    RemoteControl w;
    w.show();
    return a.exec();
}
