/** \file PluginInterface.h
  \brief Интерфейс коммуникации ядра с плагином
  \details Предоставляет интрефейс к плагину:
  - Экспорт имен методов
  - Вызов внутренних методов
  - Макросы для удобной кодогенерации вызовов
*/

#pragma once

#include <QtPlugin>
#include <QMap>
#include <QStringList>

/** Макросы для упрощения вызова экстпортируемых методов */
//======
/** \def BEGIN_EXPORTED_SELECTOR_BY(SELECTOR)
  Определяет базовые переменные для выбора нужного вызова
  - result для хранения результата
  - nameDecorator вида "PluginName."
  - macro_MethodName определяет переменную QString с именем вызываемой функции
  \param SELECTOR определяет переменную QString с именем вызываемой функции
 */
#define BEGIN_EXPORTED_SELECTOR_BY(SELECTOR) \
    QByteArray* result = NULL; \
    QString nameDecorator = getPluginId(); \
    nameDecorator+= "."; \
    QString macro_MethodName = SELECTOR;

/** \def EXPORT_METHOD(FNAME)
  Создает код проверяющий соответствие переданного имени функции
    экспортируемым фукнциям и вызывает метод при совпадении, передвавая
    ему параметры
  \param FNAME имя экспортируемой функции
  \code{.cpp}
    EXPORT_METHOD(SomeFunction)
    // разворачивается в
    if (macro_MethodName  == nameDecorator+ "SomeFunction")
        result = SomeFunction(param);
  \endcode */
#define EXPORT_METHOD(FNAME) \
    if (macro_MethodName  == nameDecorator+ #FNAME) result = FNAME(param);

/** \def EXPORT_METHOD_NOPARAMS(FNAME)
  Работает так-же как и EXPORT_METHOD но прндназначена для методов
  без параметров.
  \ref см EXPORT_METHOD(FNAME) */
#define EXPORT_METHOD_NOPARAMS(FNAME) \
    if (macro_MethodName == nameDecorator + #FNAME) result = FNAME();

/** \def RETURN_RESULT
  выходит из функции, возвращая результат
*/
#define RETURN_RESULT \
    return result;
//======

class QStringList;
class QString;

/** \brief Стандартный интерфейс плагина
    \details Используется QPluginLoader для загрузки плагина и последующего
        общения с ним
 */
class PluginInterface
{
    /** \brief Указатель на функцию со стандартным параметром
        \todo УКАЗАТЕЛЬ НА МЕТОД БЕЗ ПАРАМЕТРОВ! */
    typedef QByteArray* (*CoreCallbackFunc)(const QByteArray& id, QByteArray* param);
public:
    virtual ~PluginInterface(){}

    /** \brief Имя плагина. Им же декорируются имена экспортируемых функций
        \return Возврашает текстовый идентификатор плагина
        \code
            "SomePlugin"
        \endcode */
    const virtual QString getPluginId() const = 0;

    /** \brief Возвращает список декорированных имен методов,
            экспортируемых плагином
        \code{.cpp}
            ["SomePlugin.SomeMethod1", "SomePlugin.SomeMethod2", ...]
        \endcode
        \return Декорированный список методов */
    QStringList getPluginMethods();

    /** \brief Через этот метод вызываются экспортируемые методы
      \param methodName Строка-идентификатор метода. ex "SomePlugin.SomeMethod1"
      \param param Указатель на передаваемые аргументы. Если вызываемый метод
        плагина не принимает параметры, то игнорируется.
      \return Результат выполнения метода или NULL если метод не найден

      \code{.cpp}
      // пример использования
      QByteArray param("Some argument");
      QByteArray* result = pluginInstance->
                                CallInternal("SomePlugin.SomeMethod1", &param);
      \endcode */
    virtual QByteArray* CallInternal(const QByteArray& methodName, QByteArray* param = NULL) = 0;

    /** \brief Используется для установки указателя для вызова функции из других
        модулей или ядра.
        \param callback Указатель на функцию обрабатывающую внешние вызовы */
    void SetCoreCallback(CoreCallbackFunc callback);

protected:
    /** \brief Декорирует имена экспортируемых методов именем плагина и точкой
        \param methodNames список недекорированных имен методов
    */
    void DecorateMethodNames(QStringList& methodNames) const;

    /** \brief Список методов экспортируемых плагином
        \code{.cpp}
            ["SomeMethod1", "SomeMethod2", ...]
        \endcode
        \return Список методов*/
    virtual QStringList getMethodList() = 0;

    /** \brief Указатель на функцию вызова методов из других плагинов */
    CoreCallbackFunc CallExternal;
};

/** \def Объявляет Qt что данный интерфейс плагиновый и присваиваем ему id*/
Q_DECLARE_INTERFACE(PluginInterface, "com.monolith.Plugin.HiveInterface/0.4")
