#-------------------------------------------------
#
# Project created by QtCreator 2013-06-15T13:21:01
#
#-------------------------------------------------

QT       += core gui

TARGET = QReader
TEMPLATE = app
RC_FILE += logo.rc
CONFIG += -static

include(qxtglobalshortcut/qxtglobalshortcut.pri)
win32{
    INCLUDEPATH += D:/lib/qt/xzing
    LIBS += -LD:/lib/qt/xzing \
           -lQZXing1
}

SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

RESOURCES += \
    res.qrc
