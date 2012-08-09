#include <QtCore/QtPlugin>
#include <QString>
#include <QMap>

#include "BasicPlugin.h"
#include "Concat.h"

QString BasicPlugin::getPluginId() const {
    return "BasicPlugin";
}

QStringList BasicPlugin::getPluginMethods() const {
    QStringList methodNames;
    methodNames << "Concat";

    QString pluginId = getPluginId();
    for(QStringList::iterator methodName = methodNames.begin();
                              methodName != methodNames.end();
                              ++methodName) {
        *methodName = pluginId + "." + *methodName;
    }

    return methodNames;
}

void* BasicPlugin::CallInternal(QString methodName, const void* param) {
    QString (*n)(const QString&);
    n = &ConcatFunc;

    return InternalMethodWrapper(&ConcatFunc, param);
}

template<class ParamType, class ReturnType>
void* PluginInterface::InternalMethodWrapper(ReturnType (*methodPtr)(const ParamType&),
                                     const void* param) {
    // кастуем параметры в нужный тип и разыменовываем указатель
    const ParamType* methodParamPtr = reinterpret_cast<const ParamType*>(param);
    ParamType methodParam = *methodParamPtr;

    // вызываем собственно функцию, сохраняя результат
    ReturnType result = methodPtr(methodParam);

    // и возвращаем результат, обязательно в новом экземпляре класса.
    return reinterpret_cast<void*>(new ReturnType(result));
}

Q_EXPORT_PLUGIN2(basicplugin, BasicPlugin)
