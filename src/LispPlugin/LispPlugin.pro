TEMPLATE    = lib
CONFIG      += plugin

QT          += core
QT          -= gui

HEADERS     = LispPlugin.h \
              ../Core/PluginInterface.h \
    ConsoleColor.h
SOURCES     = LispPlugin.cpp \
    consolecolor.cpp
TARGET      = LispPlugin

DESTDIR     = ../../bin/plugins

QMAKE_CXXFLAGS += -std=gnu++0x
