#include "PluginInterface.h"

void PluginInterface::SetCoreCallback(CoreCallbackFunc callback) {
    CallCoreFunction = callback;
}

template<class ParamType, class ReturnType>
void* PluginInterface::MethodWrapper(ReturnType (*methodPtr)(const ParamType&),
                                     const void *param) {
    // кастуем параметры в нужный тип и разыменовываем указатель
    const ParamType* methodParamPtr = reinterpret_cast<const ParamType*>(param);
    ParamType methodParam = *methodParamPtr;

    // вызываем собственно функцию, сохраняя результат
    ReturnType result = methodPtr(methodParam);

    // и возвращаем результат, обязательно в новом экземпляре класса.
    return reinterpret_cast<void*>(new ReturnType(result));
}
