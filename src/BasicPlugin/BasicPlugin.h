/** \file BasicPlugin.h
\brief Интерфейс плагина
\details */
#pragma once

#include "../Core/PluginInterface.h"
#include <QObject>

//===============================================================
/** \brief Реализация интерфейсов для конкретного плагина */
class BasicPlugin: public QObject, public PluginInterface
{
    Q_OBJECT
    /** \brief указывает компилятору, что этот класс является интерфейсом
            плагина */
    Q_INTERFACES(PluginInterface)

    // реализация виртуальных методов интерфейса
    const QString getPluginId() const;
    const long Version() const;
    QVariant CallInternal(const QByteArray methodName, QVariant& param);
protected:
    QStringList getMethodList() const;

    // собственно полезная нагрузка. Реализацию экспортируемых методов,
    // для удобства, рекомендуется выносить в отдельный файл
    /** \brief Пример обычного метода */
    QVariant Duplicate(QVariant &one);
    /** \brief Пример метода, имитируюшего вызов метода из другого плагина */
    QVariant More(QVariant &str);
    /** \brief Пример метода не принимающего параметры */
    QVariant FunctionWithoutParams(void);
};
//===============================================================
