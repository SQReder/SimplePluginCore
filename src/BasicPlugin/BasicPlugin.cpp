#include <QtCore/QtPlugin>
#include <QString>
#include <QMap>

#include "BasicPlugin.h"

#ifndef PLUG_NAME
#error warn "macro PLUG_NAME must be defined"
#endif
//===============================================================
const QString BasicPlugin::getPluginId() const {
    return QString(PLUG_NAME);
}
//===============================================================
QStringList BasicPlugin::getMethodList() {
    QStringList methodNames;

    methodNames << "Concat";
    methodNames << "MuchMoreConcat";
    methodNames << "SomePluginFunction";

    return methodNames;
}
//===============================================================
QByteArray* BasicPlugin::CallInternal(const QString methodName, QByteArray* param) {
    BEGIN_EXPORTED_SELECTOR_BY(methodName);
    CALL_EXPORTED_FUNC(Concat);
    CALL_EXPORTED_FUNC(MuchMoreConcat);
    CALL_EXPORTED_FUNC_NOPARAMS(FunctionWithoutParams);
    RETURN_RESULT;
}
//===============================================================
Q_EXPORT_PLUGIN2(basicplugin, BasicPlugin)
