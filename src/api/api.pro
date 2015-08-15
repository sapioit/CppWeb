#-------------------------------------------------
#
# Project created by QtCreator 2015-08-09T11:26:46
#
#-------------------------------------------------

QT       -= core gui

TARGET = api
TEMPLATE = lib
CONFIG += c++14
DEFINES += API_LIBRARY

INCLUDEPATH += $$PWD/../../external/jsoncpp/dist
SOURCES += \
    $$PWD/../../external/jsoncpp/dist/jsoncpp.cpp \
    server.cpp \
    dispatcher.cpp \
    responsemanager.cpp \
    routeutility.cpp \
    Parser.cpp \
    log.cpp \
    storage.cpp \
    cachemanager.cpp \
    filesystem.cpp \
    resource.cpp

SOURCES += \
    DFA.cpp \
    main.cpp \
    Request.cpp \
    Socket.cpp \
    Watcher.cpp \
    response.cpp \

HEADERS += \
    DFA.h \
    Header.h \
    Request.h \
    Socket.h \
    Watcher.h \
    response.h \
    server.h \
    dispatcher.h \
    global.h \
    responsemanager.h \
    routeutility.h \
    Parser.h \
    date.h \
    log.h \
    storage.h \
    cachemanager.h \
    filesystem.h \
    resource.h \
    components.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

DISTFILES += \
    LICENSE
