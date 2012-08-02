#ifndef INTERFACE_H
#define INTERFACE_H
#include <QtPlugin>
#include <QMap>

class QStringList;
class QString;
typedef void* (*CoolVoidFunc)(const void*);
typedef QMap<QString, CoolVoidFunc> FuncMap;

typedef QString PluginFunctionId;
typedef void* (*CoreCallbackFunc)(PluginFunctionId id, const void* param);

class PluginInterface
{
    public:
        virtual ~PluginInterface(){}

        virtual FuncMap operations() const = 0;
        virtual void SetCoreCallback(CoreCallbackFunc) = 0;
};

Q_DECLARE_INTERFACE(PluginInterface, "com.monolith.Plugin.MonoInterface/0.2")

#endif // INTERFACE_H
