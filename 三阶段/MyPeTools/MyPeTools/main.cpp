#include "mypetools.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	MyPeTools w;
	w.show();
	return a.exec();
}
