#include "PluginInterface.h"
//==============================================================================
void PluginInterface::SetCoreCallback(CoreCallbackFunc callback) {
    CallCoreFunction = callback;
}
//==============================================================================
template<class ParamType, class ReturnType>
void* PluginInterface::InternalMethodWrapper(ReturnType (*methodPtr)(const ParamType&),
                                     const void *param) {
    // кастуем параметры в нужный тип и разыменовываем указатель
    const ParamType* typedParamPtr = reinterpret_cast<const ParamType*>(param);
    ParamType typedParam = *typedParamPtr;

    // вызываем собственно функцию, сохраняя результат
    ReturnType result = methodPtr(typedParam);

    // и возвращаем результат, обязательно в новом экземпляре класса.
    return reinterpret_cast<void*>(new ReturnType(result));
}
//==============================================================================
template<class ReturnType>
void* PluginInterface::InternalMethodWrapper(ReturnType (*methodPtr)()) {
    // вызываем функцию, сохраняя результат
    ReturnType result = methodPtr();

    // и возвращаем результат, обязательно в новом экземпляре класса.
    return reinterpret_cast<void*>(new ReturnType(result));
}
//==============================================================================
template<class ParamType, class ReturnType>
ReturnType PluginInterface::CallExternalMethod(QString& methodName, ParamType param) {
    // prepare param to send to plugin method
    void* voidParam = (void*) &param;

    // and call core method
    void* result = CoreCallbackFunc(methodName, voidParam);

    return *reinterpret_cast<ReturnType*>(result);
}
//==============================================================================
void PluginInterface::DecorateMethodNames(QStringList& methodNames,
                                                QString pluginId) const {
    for(QStringList::iterator methodName = methodNames.begin();
                              methodName != methodNames.end();
                              ++methodName) {
        *methodName = pluginId + "." + *methodName;
    }
}
//==============================================================================
