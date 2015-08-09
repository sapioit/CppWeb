#-------------------------------------------------
#
# Project created by QtCreator 2015-08-09T11:26:46
#
#-------------------------------------------------

QT       -= core gui

TARGET = api
TEMPLATE = lib
CONFIG += c++11
DEFINES += API_LIBRARY

SOURCES += \
    DFA.cpp \
    HttpUtility.cpp \
    main.cpp \
    Request.cpp \
    Socket.cpp \
    Watcher.cpp

HEADERS += \
    DFA.h \
    Header.h \
    HttpUtility.h \
    Request.h \
    Socket.h \
    Watcher.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

DISTFILES += \
    LICENSE
