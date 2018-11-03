#include "cad.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    cad w;


    w.show();
    return a.exec();
}
