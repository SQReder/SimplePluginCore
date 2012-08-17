#include <QtCore/QtPlugin>
#include <QString>
#include <QMap>

#include "BasicPlugin.h"

//===============================================================
const QString BasicPlugin::getPluginId() const {
    return QString("BasicPlugin");
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
    EXPORT_METHOD(Concat);
    EXPORT_METHOD(MuchMoreConcat);
    EXPORT_METHOD_NOPARAMS(FunctionWithoutParams);
    RETURN_RESULT;
}
//===============================================================
Q_EXPORT_PLUGIN2(basicplugin, BasicPlugin)
