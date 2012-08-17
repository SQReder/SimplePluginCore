#include "PluginInterface.h"
#include "HiveCore.h"
//===============================================================
void PluginInterface::SetCoreCallback(CoreCallbackFunc callback) {
    CallExternal = callback;
}
//===============================================================
QStringList PluginInterface::getPluginMethods() {
    QStringList list = getMethodList();
    DecorateMethodNames(list);
    return QStringList(list);
}
//===============================================================
QByteArray *CallPluginMethod(const QByteArray& methodName, QByteArray *params) {
    HiveCore *core = HiveCore::Instance();
    return core->CallPluginMethod(methodName, params);
}
//===============================================================
void PluginInterface::DecorateMethodNames(QStringList& methodNames) const {
    QString pluginId = getPluginId();
    for(QStringList::iterator methodName = methodNames.begin();
                              methodName != methodNames.end();
                              ++methodName) {
        *methodName = pluginId + "." + *methodName;
    }
}
