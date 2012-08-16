#pragma once

#include <QtPlugin>
#include <QMap>
#include <QStringList>

/** Макросы для упрощения вызова экстпортируемых методов */
//======
/** @def BEGIN_EXPORTED_SELECTOR_BY(SELECTOR)
  Определяет базовые переменные для выбора нужного вызова
  - result для хранения результата
  - nameDecorator вида "PluginName."
  - macro_MethodName определяет переменную QString с именем вызываемой функции
  @param SELECTOR определяет переменную QString с именем вызываемой функции
 */
#define BEGIN_EXPORTED_SELECTOR_BY(SELECTOR) \
    QByteArray* result = NULL; \
    QString nameDecorator = PLUG_NAME; \
    nameDecorator+= "."; \
    QString macro_MethodName = SELECTOR;

/** @def CALL_EXPORTED_FUNC(FNAME)
  Создает код проверяющий соответствие переданного имени функции
    экспортируемым фукнциям и вызывает метод при совпадении, передвавая
    ему параметры
  @param FNAME имя экспортируемой функции
  @code{.cpp}
    CALL_EXPORTED_FUNC(SomeFunction)
    // разворачивается в
    if (macro_MethodName  == nameDecorator+ "SomeFunction")
        result = SomeFunction(param);
  @endcode */
#define CALL_EXPORTED_FUNC(FNAME) \
    if (macro_MethodName  == nameDecorator+ #FNAME) result = FNAME(param);

/** @def CALL_EXPORTED_FUNC_NOPARAMS(FNAME)
  Работает так-же как и CALL_EXPORTED_FUNC но прндназначена для методов
  без параметров
@ref CALL_EXPORTED_FUNC(FNAME) */
#define CALL_EXPORTED_FUNC_NOPARAMS(FNAME) \
    if (macro_MethodName == nameDecorator + #FNAME) result = FNAME();

#define RETURN_RESULT \
    return result;
//======

class QStringList;
class QString;

/** @brief Стандартный интерфейс плагина
    @detailed Используется QPluginLoader для загрузки плагина и последующего
        общения с ним
 */
class PluginInterface
{
    /** @brief Указатель на функцию со стандартным параметром
        @todo УКАЗАТЕЛЬ НА МЕТОД БЕЗ ПАРАМЕТРОВ! */
    typedef QByteArray* (*CoreCallbackFunc)(const QString& id, QByteArray* param);
public:
    virtual ~PluginInterface(){}

    /** @brief Имя плагина. Им же декорируются имена экспортируемых функций
        @code
            "SomePlugin"
        @endcode */
    const virtual QString getPluginId() const = 0;

    /** @brief Возвращает список декорированных имен методов,
            экспортируемых плагином
        @code{.cpp}
            ["SomePlugin.SomeMethod1","SomePlugin.SomeMethod2"]
        @endcode */
    virtual QStringList getPluginMethods() = 0;

    /** @brief Через этот метод вызываются экспортируемые методы
      @param methodName Строка-идентификатор метода. ex "SomePlugin.SomeMethod1"
      @param param Указатель на передаваемые аргументы. Если вызываемый метод
        плагина не принимает параметры, то игнорируется.
      @code{.cpp}
      QByteArray param("Some argument");
      QByteArray* result = pluginInstance->
                                CallInternal("SomePlugin.SomeMethod1", &param);
      @endcode */
    virtual QByteArray* CallInternal(const QString methodName, QByteArray* param = NULL) = 0;

    /** @brief Используется для установки указателя для вызова функции из других
        модулей или ядра. */
    void SetCoreCallback(CoreCallbackFunc);

protected:
    /** @brief Декорирует имена экспортируемых методов именем плагина и точкой
        @param methodNames список недекорированных имен методов
        @todo перенести в приват и убрать параметр pluginId
    */
    void DecorateMethodNames(QStringList& methodNames, QString pluginId) const {
        for(QStringList::iterator methodName = methodNames.begin();
                                  methodName != methodNames.end();
                                  ++methodName) {
            *methodName = pluginId + "." + *methodName;
        }
    }

    /** @brief Указатель на функцию вызова методов из других плагинов */
    CoreCallbackFunc CallExternal;

    /** @todo deprecated */
    QStringList exportingMethods;
};

/** @def Объявляет Qt что данный интерфейс плагиновый и присваиваем ему id*/
Q_DECLARE_INTERFACE(PluginInterface, "com.monolith.Plugin.HiveInterface/0.4")
