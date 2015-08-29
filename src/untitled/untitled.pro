#-------------------------------------------------
#
# Project created by QtCreator 2015-08-27T21:58:51
#
#-------------------------------------------------

QT       -= core gui

TARGET = untitled
TEMPLATE = lib

DEFINES += UNTITLED_LIBRARY

SOURCES += untitled.cpp

HEADERS += untitled.h\
        untitled_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
