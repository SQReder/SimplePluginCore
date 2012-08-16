#include "PluginInterface.h"
#include "HiveCore.h"
//==============================================================================
void PluginInterface::SetCoreCallback(CoreCallbackFunc callback) {
    CallCoreFunction = callback;
}
//==============================================================================
QByteArray *CallPluginMethod(const QString& methodName, QByteArray *params) {
    HiveCore *core = HiveCore::Instance();
    return core->CallPluginMethod(methodName, params);
}
