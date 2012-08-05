#ifndef INTERFACE_H
#define INTERFACE_H
#include "../Core/PluginInterface.h"

class PluginContent: public QObject, public PluginInterface
{
    Q_OBJECT
    Q_INTERFACES(PluginInterface)
public:
    FuncMap operations() const;
};

#endif // INTERFACE_H
