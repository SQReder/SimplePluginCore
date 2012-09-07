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
QVariant TemplatePlugin::CallInternal(const QByteArray& methodName, QVariant &param) {
    BEGIN_EXPORTED_SELECTOR_BY(methodName);

    EXPORT_METHOD(toUpper);
    EXPORT_METHOD_NOPARAMS(noParam);
    EXPORT_METHOD_NORETURN(noReturn);

    THROW_METHOD_NOT_EXPORTED;
}
//===============================================================
Q_EXPORT_PLUGIN2(TemplatePlugin, TemplatePlugin)
//===============================================================
QVariant TemplatePlugin::toUpper(QVariant &param) {
    return param.toString().toUpper();
}
//===============================================================
QVariant TemplatePlugin::noParam(void) {
    return "I'm method with no parameter";
}
//===============================================================
void TemplatePlugin::noReturn(QVariant &param) {
    cout << "got param: " << param.toByteArray().data() << endl;
}
