# ----------------------------------------------------
# This file is generated by the Qt Visual Studio Tools.
# ------------------------------------------------------

TEMPLATE = app
TARGET = IOCPSever
QT += core gui widgets


FORMS += \
    screendialog.ui \
    cmddialog.ui \
    processdialog.ui \
    filedialog.ui\
	iocpsever.ui

HEADERS += \
    screendialog.h \
    cmddialog.h \
    processdialog.h \
    filedialog.h\
	MySocket.h\
	Session.h\
	iocpsever.h

SOURCES += \
    screendialog.cpp \
    cmddialog.cpp \
    processdialog.cpp \
    filedialog.cpp\
	main.cpp\
	MySocket.cpp\
	Session.cpp\
	iocpsever.cpp

win32: LIBS += -L$$PWD/../../lib/x86/ZlibStatReleaseWithoutAsm/ -lzlibstat

INCLUDEPATH += $$PWD/../../include
DEPENDPATH += $$PWD/../../include
DEFINES += ZLIB_WINAPI
