#include <QtCore/QtPlugin>
#include <QString>
#include <QMap>

#include "BasicPlugin.h"
#include "Concat.h"

FuncMap BasicPlugin::operations() const
{
    FuncMap map;
    map["concat"] = &ConcatWrapper;
    return map;
}

void BasicPlugin::SetCoreCallback(CoreCallbackFunc callback) {
    CallCoreFunction = callback;
}

Q_EXPORT_PLUGIN2(basicplugin, BasicPlugin)
