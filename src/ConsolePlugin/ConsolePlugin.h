/** \file ConsolePlugin.h
\brief Интерактивная консоль
\details */
#pragma once

#include "../Core/PluginInterface.h"
#include <QObject>

//===============================================================
/// Консоль для непосредственного управления
class ConsolePlugin: public QObject, public PluginInterface
{
    Q_OBJECT
    Q_INTERFACES(PluginInterface)

    // реализация виртуальных методов интерфейса
    const QString getPluginId() const;
    QByteArray* CallInternal(const QByteArray& methodName, QByteArray* param);
protected:
    QStringList getMethodList();

    QString prompt;

    /// запускает консоль
    QByteArray* StartConsole();
    /// инициализация внутренних переменных
    void InitializeConsole();
    ///
    void ShowWelcome();
    bool CommandParser(QString& commandList);
    void ParseMethodCall(QString& cmd);
    QByteArray* list();
};
//===============================================================
