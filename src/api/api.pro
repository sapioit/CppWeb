#-------------------------------------------------
#
# Project created by QtCreator 2015-08-09T11:26:46
#
#-------------------------------------------------

QT       -= core gui

TARGET = api
TEMPLATE = app
QMAKE_CXXFLAGS += -std=c++14
QMAKE_CXXFLAGS += -Wall
QMAKE_CXXFLAGS += -Ijson/
DEFINES += API_LIBRARY

QMAKE_CXXFLAGS_DEBUG += -O0 -g3
QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE += -O3

SOURCES += \
    server.cpp \
    dispatcher.cpp \
    responsemanager.cpp \
    routeutility.cpp \
    Parser.cpp \
    log.cpp \
    storage.cpp \
    cachemanager.cpp \
    filesystem.cpp \
    resource.cpp \
    outputscheduler.cpp

SOURCES += \
    DFA.cpp \
    main.cpp \
    Request.cpp \
    Socket.cpp \
    Watcher.cpp \
    response.cpp \
    jsoncpp.cpp \

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
    components.h \
    outputscheduler.h \
    json.h

unix {
    target.path = /mnt/exthdd/debugg
    INSTALLS += target
}

DISTFILES += \
    LICENSE
