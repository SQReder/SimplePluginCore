TEMPLATE    = lib
CONFIG      += plugin

QT          += core
QT          -= gui

HEADERS     = ConsolePlugin.h \
              ../Core/PluginInterface.h
SOURCES     = ConsolePlugin.cpp \
              impl.cpp
TARGET      = ConsolePlugin

DESTDIR     = ../../bin/plugins
