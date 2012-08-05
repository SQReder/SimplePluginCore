TEMPLATE    = lib
CONFIG      += plugin

QT          += core
QT          -= gui

HEADERS     = BasicPlugin.h \
              ../Core/PluginInterface.h \
              Concat.h
SOURCES     = BasicPlugin.cpp \
              Concat.cpp
#              ../Core/PluginInterface.cpp
TARGET      = BasicPlugin

DESTDIR     = ../../bin/plugins
