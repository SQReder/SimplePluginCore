/** \file HiveCore.h
  \brief Класс ядра проекта
  \details Функции ложащиеся на ядро:
  - Загрузка/выгрузка плагинов;
  - Межплагиновое взаимодействие.
  */
#pragma once

#include <QtCore>
#include "PluginInterface.h"

/** \brief Ядро системы
    \details Реализовано паттерном "одиночка".
    Инcтанцирование поизводится через  \ref HiveCore::Instance().
*/
class HiveCore: PluginInterface
{
    /** \brief Список указателей на загруженные плагины с импортированными
    методами */
    typedef  QMap<PluginInterface*, QStringList> PluginMap;
public:
    /** \brief Пустой конструктор
    \details По-хорошему нужно сделать его приватным, но Qt не дает мне этого
    \todo пусть бросает исключение, чтоле? Ибо не должно вызываться нигде,
        кроме метода \ref Instance() */
    HiveCore() {}
    virtual ~HiveCore() {}

    /** \brief Создает статический экземпляр класса ядра и возвращает
            указатель на него
    \return Указатель на статический экземпляр ядра */
    static HiveCore* Instance() {
        static HiveCore* coreInstance = new HiveCore();
        return coreInstance;
    }

    /** \brief Загрузка плагинов
    \details Проматривает директорию <i>plugins</i> и пытается загрузить все
        имеющиеся там файлы в качестве плагина.
    \todo Сделать фильтр по типам файлов */
    void loadPlugins();

    /** \brief Вызов импортированной функции по ее имени
      \details Пытается найти переданное имя в списке импортированных функций
            и вызвать, передав необходимые параметры
      \code{.cpp}
        HiveCore* core = HiveCore::Instance();
        QString method("SomePlugin.SomeMethod");
        QByteArray param("Some data");
        QVariant* result = core->CallPluginMethod(method, &param);
      \endcode
      \param methodName Полное имя вызываемого метода.
            Например "SomePlugin.SomeMethod"
      \param params Параметр передаваемый вызываемому методу
      \return Указатель на результат выполнения вызываемой функции
      \throw std::runtime_error
    */
    QVariant CallPluginMethod(const QByteArray methodName, QVariant& params);

    /** \brief Предоставляет список всех загруженных методов
    \return Список всех загруженных методов */
    QVariant listLoadedMethods() const;

    /** Вызов внутренних функций ядра */
    QVariant CallInternal(const QByteArray methodName, QVariant& param);

    const QString getPluginId() const;
    const long Version() const;
    QStringList getMethodList() const;
protected:
    /** \brief Хранит список загруженных методов ассоциированых с интерфейсами к
            плагинам */
    PluginMap loadedMethods;

    /** \brief Загружает содержимое плагина
    \param pobj Указатель на инстанс полученный от QPluginLoader
    \return Количество загруженных методов */
    int LoadPluginContent(QObject* pobj);

    /** \brief Ищет интерфейс по имени метода
    \param methodName Имя метода который необходимо найти
    \return Первый интерфейс, предоставляющий искомый метод.
        Если совпадений не найдено то NULL.
    */
    PluginInterface* locateMethod(QByteArray methodName);

private:
    /// конструктор копирования скрыт что-бы не нарушать единственность класса
    HiveCore(const HiveCore&);

    /// оператор присваивания скрыт что-бы не нарушать единственность класса.
    const HiveCore& operator =(HiveCore&);

    /** Хранит список синонимов методов в виде
      * списка псевдоним => полное имя метода */
    QHash<QByteArray, QByteArray> aliases;

    /** пытается найти передаваемое значение в скписке псевдонимов методов, и,
      * если находит - заменяет значение в переданной переменной на
      * соответствуюшее имя
    */
    const QByteArray unaliasMethodName(const QByteArray &methodName);

    /** Создает новый псевдоним для метода или заменяет ассоциацию для
      * существующего псевдонима
      */
    void addAlias(QVariant& param);

    /** Удаляет псевдоним метода
      \param alias Псевдоним метода
      */
    void removeAlias(QVariant& alias);
};

QVariant CallPluginMethod(const QByteArray methodName,
                             QVariant &params);
QVariant CallPluginMethod(const QByteArray methodName);
