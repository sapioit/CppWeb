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

INCLUDEPATH += $$PWD/../../external/jsoncpp/dist
SOURCES += \
    $$PWD/../../external/jsoncpp/dist/jsoncpp.cpp \
    server.cpp \
    application.cpp \
    dispatcher.cpp \
    routesmanager.cpp \
    routescontainer.cpp \
    responsemanager.cpp

SOURCES += \
    DFA.cpp \
    HttpUtility.cpp \
    main.cpp \
    Request.cpp \
    Socket.cpp \
    Watcher.cpp \
    response.cpp \

HEADERS += \
    DFA.h \
    Header.h \
    HttpUtility.h \
    Request.h \
    Socket.h \
    Watcher.h \
    response.h \
    server.h \
    application.h \
    dispatcher.h \
    global.h \
    routesmanager.h \
    routescontainer.h \
    responsemanager.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

DISTFILES += \
    LICENSE
