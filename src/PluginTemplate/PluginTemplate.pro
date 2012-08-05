TEMPLATE    = lib
CONFIG      += plugin

QT          += core
QT          -= gui

HEADERS     = Interface.h \
              ../Core/PluginInterface.h \
    PluginContent.h
SOURCES     = Interface.cpp \
    PluginContent.cpp
TARGET      = PluginTemplate

DESTDIR     = ../../bin/plugins


