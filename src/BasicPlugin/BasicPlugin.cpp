#include <QtCore/QtPlugin>
#include <QString>
#include <QMap>

#include "BasicPlugin.h"
#include "Concat.h"

#ifndef PLUG_NAME
#error warn "macro PLUG_NAME must be defined"
#endif

const QString BasicPlugin::getPluginId() const {
    return PLUG_NAME;
}

QStringList BasicPlugin::getPluginMethods() const {
    QStringList methodNames;
    methodNames << "Concat";
    methodNames << "MuchMoreConcat";

    for(QStringList::iterator methodName = methodNames.begin();
                              methodName != methodNames.end();
                              ++methodName) {
        *methodName = PLUG_NAME + "." + *methodName;
    }

    return methodNames;
}

void* BasicPlugin::CallInternal(QString methodName, const void* param) {
    BEGIN_EXPORTED_SELECTOR_BY
    CALL_EXPORTED_MACRO(methodName, BasicPlugin, Concat, QString, QString)
    CALL_EXPORTED_MACRO(methodName, BasicPlugin, MuchMoreConcat, QString, QString)
    END_EXPORTED_SELECTOR
            ;
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

QString BasicPlugin::Concat(const QString& one)
{
    return one + "+" + one;
}

QString BasicPlugin::MuchMoreConcat(const QString& str) {
    return Concat(str) + "::" + Concat(str);
}



Q_EXPORT_PLUGIN2(basicplugin, BasicPlugin)
