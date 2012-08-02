#ifndef MYPLUGIN_H
#define MYPLUGIN_H
#include "../../PluginInterface.h"
#include <QObject>

//===============================================================
class BasicPlugin: public QObject, public PluginInterface
{
    Q_OBJECT
    Q_INTERFACES(PluginInterface)
public:
    FuncMap operations() const;
    void SetCoreCallback(CoreCallbackFunc callback);

private:
    CoreCallbackFunc CallCoreFunction;
};
//===============================================================
#endif // MYPLUGIN_H
