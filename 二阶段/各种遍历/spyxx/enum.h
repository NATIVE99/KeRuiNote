#ifndef ENUM_H
#define ENUM_H

#include "ui_mainwindow.h"
#include <QString>
#include <qdebug.h>
#include <windows.h>
#include <QObject>
class Enum:public QObject
{
     Q_OBJECT
public:
        virtual void execut()=0;
protected:
       Enum();
};

#endif // ENUM_H
