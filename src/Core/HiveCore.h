#pragma once

#include <QMap>
#include <QStringList>

class PluginInterface;
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

    QByteArray* CallPluginMethod(const QString& methodName, QByteArray* params);

    const QStringList listLoadedMethods() const;
protected:
    PluginMap methods;

    int LoadPluginContent(QObject* pobj);
    PluginInterface* locateMethod(QString methodName);

private:
    HiveCore(const HiveCore&);
    const HiveCore& operator =(HiveCore&);
};
