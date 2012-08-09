#pragma once

#include "PluginInterface.h"

typedef  QMap<PluginInterface*, QStringList> PluginMap;

class HiveCore : public QObject
{
    Q_OBJECT
public:
    HiveCore();
    virtual ~HiveCore() {}

    void* CallPluginMethod(QString& methodName, const void *params);
protected:
    PluginMap methods;

    void loadPlugins();
    int LoadPluginContent(QObject* pobj);
    PluginInterface* locateMethod(QString methodName);
};
