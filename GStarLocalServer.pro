#-------------------------------------------------
#
# Project created by QtCreator 2015-03-02T09:20:31
#
#-------------------------------------------------

QT       += network testlib core-private network

QT       -= gui

TARGET = tst_gstarlocalserver
CONFIG   += console no_private_qt_headers_warning
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += tst_gstarlocalserver.cpp \
    gstarlocalserver/gstarlocalserver.cpp \
    gstarlocalserver/gstarlocalsocket.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

HEADERS += \
    gstarlocalserver/gstarlocalserver.h \
    gstarlocalserver/gstarlocalsocket.h \
    gstarlocalserver/gstarlocalsocket_p.h \
    gstarlocalserver/gstarlocalserver_p.h \
    gstarlocalserver/gstarlocalglobal.h \
    gstarlocalserver/gstarlocalheader.h
