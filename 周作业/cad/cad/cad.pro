# ----------------------------------------------------
# This file is generated by the Qt Visual Studio Tools.
# ------------------------------------------------------

TEMPLATE = app
TARGET = cad
DESTDIR = ../Win32/Debug
QT += core gui widgets
CONFIG += debug console
DEFINES += _UNICODE WIN64 QT_DLL QT_WIDGETS_LIB
INCLUDEPATH += ./GeneratedFiles \
    . \
    ./GeneratedFiles/$(ConfigurationName)
DEPENDPATH += .
MOC_DIR += ./GeneratedFiles/$(ConfigurationName)
OBJECTS_DIR += debug
UI_DIR += ./GeneratedFiles
RCC_DIR += ./GeneratedFiles
include(cad.pri)

FORMS += \
    dialog.ui

HEADERS += \
    dialog.h

SOURCES += \
    dialog.cpp
