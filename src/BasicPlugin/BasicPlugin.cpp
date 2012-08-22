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
    return 0x01000000;
}
//===============================================================
QStringList BasicPlugin::getMethodList() {
    QStringList methodNames;

    methodNames << "echo";
    methodNames << "MuchMoreConcat";
    methodNames << "FunctionWithoutParams";

    return methodNames;
}
//===============================================================
QByteArray* BasicPlugin::CallInternal(const QByteArray& methodName, QByteArray* param) {
    BEGIN_EXPORTED_SELECTOR_BY(methodName);
    EXPORT_METHOD(echo);
    EXPORT_METHOD(More);
    EXPORT_METHOD_NOPARAMS(FunctionWithoutParams);
    RETURN_RESULT;
}
//===============================================================
Q_EXPORT_PLUGIN2(basicplugin, BasicPlugin)
//===============================================================
QByteArray* BasicPlugin::echo(QByteArray* one) {
    if (one == NULL)
        return NULL;
    QString str(*one);
    QByteArray arr;
    arr.append(str + "+" + str);
    return new QByteArray(arr);
}
//===============================================================
QByteArray* BasicPlugin::More(QByteArray* str) {
    QByteArray method("Basic.Concat");
    QByteArray* res = echo(str);
    res->append("::");

    QByteArray* external = CallExternal(method, str);
    res->append(external->data());
    return new QByteArray(*res);
}
//===============================================================
QByteArray* BasicPlugin::FunctionWithoutParams(void) {
    QByteArray str("param");
    QByteArray* res = More(&str);

    cout << res->data() << endl;

    return 0;
}
