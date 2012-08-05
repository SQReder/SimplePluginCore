#include <QtCore/QtPlugin>
#include "interface.h"

#include "PluginContent.h"

FuncMap PluginContent::operations() const
{
    FuncMap map;
    return map;
}

Q_EXPORT_PLUGIN2(plugincontent, PluginContent)
