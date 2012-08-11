#include <QtCore/QtPlugin>
#include <QString>
#include <QMap>

#include "BasicPlugin.h"

#ifndef PLUG_NAME
#error warn "macro PLUG_NAME must be defined"
#endif

const QString BasicPlugin::getPluginId() const {
    return QString(PLUG_NAME);
}

QStringList BasicPlugin::getPluginMethods() {
    QStringList methodNames;
    methodNames << "Concat";
    methodNames << "MuchMoreConcat";
    methodNames << "SomePluginFunction";

    DecorateMethodNames(methodNames, PLUG_NAME);

    return methodNames;
}

void* BasicPlugin::CallInternal(QString methodName, const void* param) {
    BEGIN_EXPORTED_SELECTOR_BY
    CALL_EXPORTED_FUNC(methodName, Concat);
    CALL_EXPORTED_FUNC(methodName, MuchMoreConcat);
    CALL_EXPORTED_FUNC_NOPARAMS(methodName, FunctionWithoutParams);
    END_EXPORTED_SELECTOR;
}

template<class ParamType, class ReturnType>
void* BasicPlugin::InternalMethodWrapper(ReturnType (BasicPlugin::*methodPtr)(const ParamType&),
                                         const void* param) {
    // кастуем параметры в нужный тип и разыменовываем указатель
    const ParamType* methodParamPtr = reinterpret_cast<const ParamType*>(param);
    const ParamType methodParam = *methodParamPtr;

    // вызываем собственно функцию, сохраняя результат
    ReturnType result = (this->*methodPtr)(methodParam);

    // и возвращаем результат, обязательно в новом экземпляре класса.
    return reinterpret_cast<void*>(new ReturnType(result));
}

template<class ReturnType>
void* BasicPlugin::InternalMethodWrapper(ReturnType (BasicPlugin::*methodPtr)()) {
    // вызываем собственно функцию, сохраняя результат
    ReturnType result = (this->*methodPtr)();

    // и возвращаем результат, обязательно в новом экземпляре класса.
    return reinterpret_cast<void*>(new ReturnType(result));
}

Q_EXPORT_PLUGIN2(basicplugin, BasicPlugin)
