TEMPLATE    = lib
CONFIG      += plugin

QMAKE_CXXFLAGS += -std=gnu++0x

QT          += core
QT          -= gui

HEADERS     = ConsolePlugin.h \
              ../Core/PluginInterface.h
SOURCES     = ConsolePlugin.cpp
TARGET      = ConsolePlugin

DESTDIR     = ../../bin/plugins
