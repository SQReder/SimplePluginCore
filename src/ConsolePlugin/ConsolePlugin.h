/** \file ConsolePlugin.h
    \brief Интерактивная консоль
    \details
*/
#pragma once

#include "../Core/PluginInterface.h"
#include <QObject>
#include <QHash>

//===============================================================
/// Консоль для непосредственного управления
class ConsolePlugin: public QObject, public PluginInterface
{
    Q_OBJECT
    Q_INTERFACES(PluginInterface)

    // реализация виртуальных методов интерфейса
    const QString getPluginId() const;
    const long Version() const;
    QVariant CallInternal(const QByteArray methodName, QVariant &param);
protected:
    QStringList getMethodList() const;
    void init();

    /// Приглашение ввода \'\>\>\'
    QString prompt;

    /// Запускает консоль
    QVariant StartConsole();
    /// Инициализация внутренних переменных
    void InitializeConsole();

    /// Показывает приглашение и некую информацию о консоли
    void ShowWelcome();

    /** Обработчик вводимых комманд
    */
    bool CommandProcessor(QString& commandList);

    /** Вызов метода, в том числе по синониму
        \details Вызывает метод, подставляя название вместо синонима,
        если необходимо
        \param param Коммандная строка полностью
      */
    QVariant resolveCall(QVariant &param);

    void PrintQVariantValue(QVariant& val);
};
//===============================================================
