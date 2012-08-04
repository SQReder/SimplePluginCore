TEMPLATE    = lib
CONFIG      += plugin

QT          += core
QT          -= gui

HEADERS     = plugincontent.h \
              ../../PluginInterface.h
SOURCES     = plugincontent.cpp
TARGET      = BasicPlugin

DESTDIR     = ../../../plugins


