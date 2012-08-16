#include "PluginInterface.h"
#include "HiveCore.h"
//==============================================================================
void PluginInterface::SetCoreCallback(CoreCallbackFunc callback) {
    CallCoreFunction = callback;
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
QByteArray* PluginInterface::CallExternalMethod(const QString& methodName, QByteArray* param) {
    return CallCoreFunction(methodName, param);
}
//==============================================================================
QByteArray *CallPluginMethod(const QString& methodName, QByteArray *params) {
    HiveCore *core = HiveCore::Instance();
    return core->CallPluginMethod(methodName, params);
}
