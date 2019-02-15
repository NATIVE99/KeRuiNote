#-------------------------------------------------
#
# Project created by QtCreator 2019-02-02T12:13:26
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = spyxx
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    enum.cpp \
    enumpid.cpp \
    enumfile.cpp \
    dialog.cpp \
    enumserive.cpp \
    enumwin.cpp \
    enumreg.cpp \
    spyxx.cpp

HEADERS  += mainwindow.h \
    enum.h \
    enumpid.h \
    enumfile.h \
    dialog.h \
    enumserive.h \
    enumwin.h \
    enumreg.h \
    spyxx.h

FORMS    += mainwindow.ui \
    dialog.ui

RESOURCES += \
    spyxxico.qrc
LIBS += -lgdi32
