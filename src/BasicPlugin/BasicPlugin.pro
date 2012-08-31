TEMPLATE    = lib
CONFIG      += plugin

QT          += core
QT          -= gui

QMAKE_CXXFLAGS += -std=gnu++0x

HEADERS     = BasicPlugin.h \
              ../Core/PluginInterface.h
SOURCES     = BasicPlugin.cpp
TARGET      = BasicPlugin

DESTDIR     = ../../bin/plugins
