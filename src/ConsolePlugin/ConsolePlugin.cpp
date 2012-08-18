#include <QtCore/QtPlugin>
#include <QString>
#include <QMap>

#include "ConsolePlugin.h"

//===============================================================
const QString ConsolePlugin::getPluginId() const {
    return QString("ConsolePlugin");
}
//===============================================================
QStringList ConsolePlugin::getMethodList() {
    QStringList methodNames;

    methodNames << "StartConsole";

    return methodNames;
}
//===============================================================
QByteArray* ConsolePlugin::CallInternal(const QByteArray& methodName, QByteArray* param) {
    BEGIN_EXPORTED_SELECTOR_BY(methodName);
    EXPORT_METHOD_NOPARAMS(StartConsole);
    RETURN_RESULT;
}
//===============================================================
Q_EXPORT_PLUGIN2(Consoleplugin, ConsolePlugin)
