#pragma once

#include <QMap>
#include <QStringList>
#include "PluginInterface.h"

typedef  QMap<PluginInterface*, QStringList> PluginMap;

class HiveCore
{
public:
    HiveCore() {}
    virtual ~HiveCore() {}

    static HiveCore* Instance() {
        static HiveCore* coreInstance = new HiveCore();
        return coreInstance;
    }

    void loadPlugins();

    void* CallPluginMethod(QString& methodName, const void *params);

    const QStringList listLoadedMethods() const;
protected:
    PluginMap methods;

    int LoadPluginContent(QObject* pobj);
    PluginInterface* locateMethod(QString methodName);

private:
    HiveCore(const HiveCore&);
    const HiveCore& operator =(HiveCore&);
};
