#ifndef PLUGINCONTENT_H
#define PLUGINCONTENT_H
#include "../Core/PluginInterface.h"

class PluginContent: public QObject, public PluginInterface
{
    Q_OBJECT
    Q_INTERFACES(PluginInterface)
public:
    FuncMap operations() const;
};

#endif // PLUGINCONTENT_H
