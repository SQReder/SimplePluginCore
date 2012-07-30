#include <QtCore/QtPlugin>
#include <QString>
#include <QMap>

#include "BasicPlugin.h"
//#include "Concat.h"


FuncMap BasicPlugin::operations() const
{
    FuncMap map;
    map["concat"] = &ConcatWrapper;
    return map;
}

void operation(const QString& strText, const QString& strOperation)
{
    return; // strText + ":" + strOperation;
}

Q_EXPORT_PLUGIN2(basicplugin, BasicPlugin)
