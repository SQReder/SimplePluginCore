#include <QtCore/QtPlugin>
#include <QString>
#include <QMap>
#include <iostream>
using namespace std;

#include "TemplatePlugin.h"

//===============================================================
const QString TemplatePlugin::getPluginId() const {
    return QString("Example");
}
//===============================================================
const long TemplatePlugin::Version() const {
    return 0x00000000;
}
//===============================================================
QStringList TemplatePlugin::getMethodList() const {
    QStringList methodNames;

    methodNames << "toUpper"
                << "noParam"
                << "noReturn";

    return methodNames;
}
//===============================================================
QByteArray TemplatePlugin::CallInternal(const QByteArray& methodName, QByteArray &param) {
    BEGIN_EXPORTED_SELECTOR_BY(methodName);

    EXPORT_METHOD(toUpper);
    EXPORT_METHOD_NOPARAMS(noParam);
    EXPORT_METHOD_NORETURN(noReturn);

    RETURN_RESULT;
}
//===============================================================
Q_EXPORT_PLUGIN2(TemplatePlugin, TemplatePlugin)
//===============================================================
QByteArray TemplatePlugin::toUpper(QByteArray &param) {
    return param.toUpper();
}
//===============================================================
QByteArray TemplatePlugin::noParam(void) {
    return "I'm method with no parameter";
}
//===============================================================
void TemplatePlugin::noReturn(QByteArray& param) {
    cout << "got param: " << param.data() << endl;
}
