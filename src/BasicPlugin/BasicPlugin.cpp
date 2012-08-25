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
QByteArray BasicPlugin::CallInternal(const QByteArray& methodName, QByteArray &param) {
    BEGIN_EXPORTED_SELECTOR_BY(methodName);
    EXPORT_METHOD(Duplicate);
    EXPORT_METHOD(More);
    EXPORT_METHOD_NOPARAMS(FunctionWithoutParams);
    RETURN_RESULT;
}
//===============================================================
Q_EXPORT_PLUGIN2(basicplugin, BasicPlugin)
//===============================================================
QByteArray BasicPlugin::Duplicate(QByteArray& string) {
    return string + "+" + string;
}
//===============================================================
QByteArray BasicPlugin::More(QByteArray &str) {
    QByteArray res = Duplicate(str);
    res.append("::");

    QByteArray external = CallExternal("Basic.Duplicate", str);
    res.append(external);
    return res;
}
//===============================================================
QByteArray BasicPlugin::FunctionWithoutParams(void) {
    QByteArray str("param");
    QByteArray res = More(str);

    cout << res.data() << endl;

    return 0;
}
