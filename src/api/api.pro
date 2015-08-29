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
    outputscheduler.cpp \
    settings.cpp

SOURCES += \
    dfa.cpp \
    main.cpp \
    request.cpp \
    socket.cpp \
    watcher.cpp \
    response.cpp \
    jsoncpp.cpp \

HEADERS += \
    dfa.h \
    header.h \
    request.h \
    socket.h \
    watcher.h \
    response.h \
    server.h \
    dispatcher.h \
    global.h \
    responsemanager.h \
    routeutility.h \
    parser.h \
    date.h \
    log.h \
    storage.h \
    cachemanager.h \
    filesystem.h \
    resource.h \
    components.h \
    outputscheduler.h \
    json.h \
    settings.h

unix {
    target.path = /mnt/exthdd/debugg
    INSTALLS += target
}

DISTFILES += \
    LICENSE
