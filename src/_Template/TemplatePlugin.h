/** \file TemplatePlugin.h
\brief Интерфейс плагина
\details */
#pragma once

#include "../Core/PluginInterface.h"
#include <QObject>

//===============================================================
/** \brief Реализация интерфейсов для конкретного плагина */
class TemplatePlugin: public QObject, public PluginInterface
{
    Q_OBJECT
    /** \brief указывает компилятору, что этот класс является интерфейсом
            плагина */
    Q_INTERFACES(PluginInterface)

    // реализация виртуальных методов интерфейса
    const QString getPluginId() const;
    const long Version() const;
    QVariant CallInternal(const QByteArray& methodName, QVariant& param);
protected:
    QStringList getMethodList() const;

    QVariant toUpper(QVariant &param);
    QVariant noParam(void);
    void noReturn(QVariant &param);
};
//===============================================================
