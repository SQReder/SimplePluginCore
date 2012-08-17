/** \file BasicPlugin.h
\brief Интерфейс плагина
\details */
#pragma once

#include "../Core/PluginInterface.h"
#include <QObject>

#define PLUG_NAME "BasicPlugin"
#define PLUG_CLASS BasicPlugin
//===============================================================
/** \brief Реализация интерфейсов для конкретного плагина */
class BasicPlugin: public QObject, public PluginInterface
{
    Q_OBJECT
    /** \brief указывает компилятору, что этот класс является интерфейсом
            плагина */
    Q_INTERFACES(PluginInterface)

    // реализация виртуальных методов интерфейса
    QStringList getPluginMethods();
    const QString getPluginId() const;
    QByteArray* CallInternal(const QString methodName, QByteArray* param);
protected:
    // собственно полезная нагрузка. Реализацию экспортируемых методов,
    // для удобоства, рекомендуется выносить в отдельный файл
    QByteArray* Concat(QByteArray* one);
    QByteArray* MuchMoreConcat(QByteArray* str);
    QByteArray* FunctionWithoutParams(void);
};
//===============================================================
