#pragma once

#include <QtPlugin>
#include <QMap>
#include <QStringList>

//======
#define BEGIN_EXPORTED_SELECTOR_BY \
    void* result = NULL; \
    QString name = PLUG_NAME; \
    name += ".";
#define CALL_EXPORTED_FUNC(SELECTOR, FNAME) \
    if (SELECTOR == name + #FNAME) \
        result = InternalMethodWrapper(&PLUG_CLASS::FNAME, param);\

#define CALL_EXPORTED_FUNC_NOPARAMS(SELECTOR, FNAME) \
    if (SELECTOR == name + #FNAME) \
        result = InternalMethodWrapper(&PLUG_CLASS::FNAME);
#define END_EXPORTED_SELECTOR \
    return result;
//======

class HiveCore;

class QStringList;
class QString;

typedef void* (*CoolVoidFunc)(const void*);
typedef void* (HiveCore::*CoreCallbackFunc)(QString& id, const void* param);

class PluginInterface
{
    public:
        virtual ~PluginInterface(){}

    virtual QStringList getPluginMethods() = 0;
    const virtual QString getPluginId() const = 0;
    virtual void* CallInternal(QString methodName, const void* param) = 0;

    void SetCoreCallback(CoreCallbackFunc);

protected:
    template<class ParamType, class ReturnType>
    ReturnType CallExternalMethod(QString& methodName, ParamType param);

    template<class ParamType, class ReturnType>
    void* InternalMethodWrapper(ReturnType (*methodPtr)(ParamType const&),
                                void const* param);

    template<class ReturnType>
    void* InternalMethodWrapper(ReturnType (*methodPtr)(void));

    void DecorateMethodNames(QStringList& methodNames, QString pluginId) const;

    CoreCallbackFunc CallCoreFunction;
    QStringList exportingMethods;
};

Q_DECLARE_INTERFACE(PluginInterface, "com.monolith.Plugin.HiveInterface/0.3")
