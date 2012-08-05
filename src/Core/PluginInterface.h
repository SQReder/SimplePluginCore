#ifndef INTERFACE_H
#define INTERFACE_H
#include <QtPlugin>
#include <QMap>
#include <QStringList>

class QStringList;
class QString;

typedef void* (*CoolVoidFunc)(const void*);
typedef QMap<QString, CoolVoidFunc> FuncMap;

typedef QString PluginMethosName;
typedef void* (*CoreCallbackFunc)(PluginMethosName id, const void* param);

class PluginInterface
{
    public:
        virtual ~PluginInterface(){}

    virtual QStringList getPluginMethods() const = 0;
    virtual QString getPluginId() const = 0;
    virtual void* Call(PluginMethosName methodName, const void* param) = 0;

    void SetCoreCallback(CoreCallbackFunc);

    template<class ParamType, class ReturnType>
    void* MethodWrapper(ReturnType (*methodPtr)(ParamType const&),
                        void const* param);
protected:
    CoreCallbackFunc CallCoreFunction;
    QStringList exportingMethods;
};

Q_DECLARE_INTERFACE(PluginInterface, "com.monolith.Plugin.HiveInterface/0.3")

#endif // INTERFACE_H
