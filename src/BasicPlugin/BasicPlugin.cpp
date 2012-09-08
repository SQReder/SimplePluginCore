#include <QtCore/QtPlugin>
#include <QString>
#include <QMap>
#include <iostream>
using namespace std;

#include "BasicPlugin.h"

//===============================================================
const QString BasicPlugin::getPluginId() const {
    return QString("Basic");
}
//===============================================================
const long BasicPlugin::Version() const {
    return 0x02000000;
}
//===============================================================
QStringList BasicPlugin::getMethodList() const{
    QStringList methodNames;

    methodNames << "Duplicate";
    methodNames << "MuchMoreConcat";
    methodNames << "FunctionWithoutParams";

    return methodNames;
}
//===============================================================
QVariant BasicPlugin::CallInternal(const QByteArray methodName, QVariant &param) {
    BEGIN_EXPORTED_SELECTOR_BY(methodName);
    EXPORT_METHOD(Duplicate);
    EXPORT_METHOD(More);
    EXPORT_METHOD_NOPARAMS(FunctionWithoutParams);
    THROW_METHOD_NOT_EXPORTED;
}
//===============================================================
Q_EXPORT_PLUGIN2(basicplugin, BasicPlugin)
//===============================================================
QVariant BasicPlugin::Duplicate(QVariant& string) {
    return string.toByteArray() + "+" + string.toByteArray();
}
//===============================================================
QVariant BasicPlugin::More(QVariant &str) {
    QByteArray res = Duplicate(str).toByteArray();
    res.append("::");

    QByteArray external = CallExternal("Basic.Duplicate", str).toByteArray();
    res.append(external);
    return res;
}
//===============================================================
QVariant BasicPlugin::FunctionWithoutParams(void) {
    QVariant str("param");
    auto res = More(str);

    return res;
}
