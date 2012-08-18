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

    /// Приглашение ввода \'\>\>\'
    QString prompt;

    /// Запускает консоль
    QByteArray* StartConsole();
    /// Инициализация внутренних переменных
    void InitializeConsole();

    /// Показывает приглашение и некую информацию о консоли
    void ShowWelcome();

    /** Обработчик вводимых комманд
    */
    bool CommandParser(QString& commandList);

    /** Обработчик вызовов функций
    \todo Коряво парсит вызовы без параметров */
    void ParseMethodCall(QString cmd);
};
//===============================================================
