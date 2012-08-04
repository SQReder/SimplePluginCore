TEMPLATE    = lib
CONFIG      += plugin

QT          += core
QT          -= gui

HEADERS     = Interface.h \
              ../Core/PluginInterface.h
SOURCES     = Interface.cpp
TARGET      = PluginTemplate

DESTDIR     = ../../bin/plugins
