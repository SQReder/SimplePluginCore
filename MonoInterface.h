#ifndef INTERFACE_H
#define INTERFACE_H
#include <QtPlugin>
#include <QMap>

QT_BEGIN_NAMESPACE
class QStringList;
class QString;
typedef void* (*CoolVoidFunc)(const void*);
typedef QMap<QString, CoolVoidFunc> FuncMap;
QT_END_NAMESPACE


class MonoInterface
{
    public:
        virtual ~MonoInterface(){}

        virtual FuncMap operations() const = 0;
};

Q_DECLARE_INTERFACE(MonoInterface, "com.monolith.Plugin.MonoInterface/0.1")

#endif // INTERFACE_H
