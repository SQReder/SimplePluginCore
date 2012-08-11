#pragma once

#include "../Core/PluginInterface.h"
#include <QObject>

#define PLUG_NAME "BasicPlugin"
#define PLUG_CLASS BasicPlugin
//===============================================================
class BasicPlugin: public QObject, public PluginInterface
{
    Q_OBJECT
    Q_INTERFACES(PluginInterface)

    QStringList getPluginMethods();
    const QString getPluginId() const;
    void* CallInternal(QString methodName, const void* param);
    template<class ParamType, class ReturnType>
    void* InternalMethodWrapper(ReturnType (BasicPlugin::*methodPtr)(const ParamType&),
                                         const void* param);
    template<class ReturnType>
    void* InternalMethodWrapper(ReturnType (BasicPlugin::*methodPtr)(void));
protected:
    QString Concat(const QString& one);
    QString MuchMoreConcat(const QString& str);
    int FunctionWithoutParams(void);
};
//===============================================================
