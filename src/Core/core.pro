QT       += core

QT       -= gui

TARGET    = core
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

DESTDIR  = ../../bin

SOURCES += main.cpp \
    PluginInterface.cpp \
    HiveCore.cpp

HEADERS += PluginInterface.h \
    HiveCore.h

QMAKE_CXXFLAGS += -std=gnu++0x
