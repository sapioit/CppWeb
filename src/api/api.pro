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


#HTTP
SOURCES += \
    http/cachemanager.cpp \
    http/parser.cpp \
    http/request.cpp \
    http/response.cpp \
    http/responsemanager.cpp \
    http/routeutility.cpp

HEADERS += \
    http/cachemanager.h \
    http/components.h \
    http/header.h \
    http/parser.h \
    http/request.h \
    http/response.h \
    http/responsemanager.h \
    http/routeutility.h \
    io/datapacket.h \
    io/scheduled_write.h
#HTTP-END

#IO
SOURCES += \
    io/filesystem.cpp \
    io/outputscheduler.cpp \
    io/socket.cpp \
    io/watcher.cpp

HEADERS += \
    io/filesystem.h \
    io/outputscheduler.h \
    io/socket.h \
    io/watcher.h
#IO-END

#JSON
SOURCES += \
    json/jsoncpp.cpp

HEADERS += \
    json/json.h
#JSON-END


#SERVER
SOURCES += \
    server/dispatcher.cpp \
    server/server.cpp

HEADERS += \
    server/dispatcher.h \
    server/server.h \
#SERVER-END

#MISC
SOURCES += \
    misc/log.cpp \
    misc/resource.cpp \
    misc/settings.cpp \
    misc/storage.cpp

HEADERS += \
    misc/date.h \
    misc/dfa.h \
    misc/log.h \
    misc/resource.h \
    misc/settings.h \
    misc/storage.h
#MISC-END

SOURCES += \
    main.cpp

unix {
    target.path = /mnt/exthdd/debugg
    INSTALLS += target
}

DISTFILES += \
    LICENSE
