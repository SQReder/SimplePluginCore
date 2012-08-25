TEMPLATE    = lib
CONFIG      += plugin

QT          += core
QT          -= gui

HEADERS     = TemplatePlugin.h \
              ../Core/PluginInterface.h
SOURCES     = TemplatePlugin.cpp
TARGET      = TemplatePlugin

DESTDIR     = ../../bin/plugins
