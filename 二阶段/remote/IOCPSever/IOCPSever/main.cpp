#include "iocpsever.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    IOCPSever w;
    w.show();
    return a.exec();
}
