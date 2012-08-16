TEMPLATE    = lib
CONFIG      += plugin

QT          += core
QT          -= gui

HEADERS     = BasicPlugin.h \
              ../Core/PluginInterface.h
SOURCES     = BasicPlugin.cpp \
              impl.cpp
TARGET      = BasicPlugin

DESTDIR     = ../../bin/plugins
