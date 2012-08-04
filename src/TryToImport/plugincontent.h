#ifndef PLUGINCONTENT_H
#define PLUGINCONTENT_H
#include "../../PluginInterface.h"

class PluginContent: public PluginInterface
{
    Q_OBJECT
    Q_INTERFACES(PluginInterface)
public:
    FuncMap operations() const;
    void SetCoreCallback(CoreCallbackFunc callback);

private:
    CoreCallbackFunc CallCoreFunction;
};

#endif // PLUGINCONTENT_H
