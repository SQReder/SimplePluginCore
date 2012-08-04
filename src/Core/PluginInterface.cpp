#include "PluginInterface.h"

void PluginInterface::SetCoreCallback(CoreCallbackFunc callback) {
    CallCoreFunction = callback;
}
