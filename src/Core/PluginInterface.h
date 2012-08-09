#pragma once

#include <QtPlugin>
#include <QMap>
#include <QStringList>

class HiveCore;

class QStringList;
class QString;

typedef void* (*CoolVoidFunc)(const void*);
typedef void* (HiveCore::*CoreCallbackFunc)(QString& id, const void* param);

class PluginInterface
{
    public:
        virtual ~PluginInterface(){}

    virtual QStringList getPluginMethods() const = 0;
    virtual QString getPluginId() const = 0;
    virtual void* CallInternal(QString methodName, const void* param) = 0;

    void SetCoreCallback(CoreCallbackFunc);

protected:
    template<class ParamType, class ReturnType>
    ReturnType CallExternalMethod(QString& methodName, ParamType param);

    template<class ParamType, class ReturnType>
    void* InternalMethodWrapper(ReturnType (*methodPtr)(ParamType const&),
                                void const* param);

    CoreCallbackFunc CallCoreFunction;
    QStringList exportingMethods;
};

Q_DECLARE_INTERFACE(PluginInterface, "com.monolith.Plugin.HiveInterface/0.3")
