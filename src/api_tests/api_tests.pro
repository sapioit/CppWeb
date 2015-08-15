#-------------------------------------------------
#
# Project created by QtCreator 2015-08-09T11:31:43
#
#-------------------------------------------------

QT       += network testlib

QT       -= gui

TARGET = tst_api_teststest
CONFIG   += console
CONFIG   -= app_bundle
CONFIG += c++14

TEMPLATE = app


SOURCES += tst_api_teststest.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../api/bin/ -lapi
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../api/bin/ -lapi
else:unix: LIBS += -L$$OUT_PWD/../api/ -lapi

INCLUDEPATH += $$PWD/../api
DEPENDPATH += $$PWD/../api
