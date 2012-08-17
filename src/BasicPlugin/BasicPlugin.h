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
    // для удобства, рекомендуется выносить в отдельный файл
    /** \brief Пример обычного метода */
    QByteArray* Concat(QByteArray* one);
    /** \brief Пример метода, имитируюшего вызов метода из другого плагина */
    QByteArray* MuchMoreConcat(QByteArray* str);
    /** \brief Пример метода не принимающего параметры */
    QByteArray* FunctionWithoutParams(void);
};
//===============================================================
