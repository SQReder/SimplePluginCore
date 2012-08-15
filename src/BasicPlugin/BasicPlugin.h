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
    QByteArray* CallInternal(const QString methodName, QByteArray* param);
protected:
    QByteArray* Concat(const QByteArray* one);
    QByteArray* MuchMoreConcat(const QByteArray* str);
    QByteArray* FunctionWithoutParams(void);
};
//===============================================================
