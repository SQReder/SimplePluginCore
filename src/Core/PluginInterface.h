#pragma once

#include <QtPlugin>
#include <QMap>
#include <QStringList>

//======
#define BEGIN_EXPORTED_SELECTOR_BY(SELECTOR) \
    QByteArray* result = NULL; \
    QString nameDecorator = PLUG_NAME; \
    nameDecorator+= "."; \
    QString macro_MethodName = SELECTOR;
#define CALL_EXPORTED_FUNC(FNAME) \
    if (macro_MethodName  == nameDecorator+ #FNAME) result = FNAME(param);

#define CALL_EXPORTED_FUNC_NOPARAMS(FNAME) \
    if (macro_MethodName == nameDecorator + #FNAME) result = FNAME();
#define RETURN_RESULT \
    return result;
//======

class QStringList;
class QString;

typedef void* (*CoolVoidFunc)(const void*);
typedef QByteArray* (*CoreCallbackFunc)(const QString& id, QByteArray* param);

class PluginInterface
{
    public:
        virtual ~PluginInterface(){}

    virtual QStringList getPluginMethods() = 0;
    const virtual QString getPluginId() const = 0;
    virtual QByteArray* CallInternal(const QString methodName, QByteArray* param) = 0;

    void SetCoreCallback(CoreCallbackFunc);

protected:
    QByteArray* CallExternalMethod(const QString& methodName, QByteArray* param);

    template<class ReturnType>
    void* InternalMethodWrapper(ReturnType (*methodPtr)(void));

    void DecorateMethodNames(QStringList& methodNames, QString pluginId) const {
        for(QStringList::iterator methodName = methodNames.begin();
                                  methodName != methodNames.end();
                                  ++methodName) {
            *methodName = pluginId + "." + *methodName;
        }
    }

    CoreCallbackFunc CallCoreFunction;
    QStringList exportingMethods;
};

Q_DECLARE_INTERFACE(PluginInterface, "com.monolith.Plugin.HiveInterface/0.3")
