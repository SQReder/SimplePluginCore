#include "PluginInterface.h"
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
    HiveCore* core = HiveCore::Instance();
    return core->CallPluginMethod(methodName, param);
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
