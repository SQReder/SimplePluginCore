#include "BasicPlugin.h"

#include <stdio.h>

QString BasicPlugin::Concat(const QString& one)
{
    return one + "+" + one;
}

QString BasicPlugin::MuchMoreConcat(const QString& str) {
    QString method("BasicPlugin.Concat");
//    return Concat(str) + "::" + CallExternalMethod<QString, QString>(method, str);
}

int BasicPlugin::FunctionWithoutParams(void) {
    QString str("param");
    printf("%s\n", qPrintable(MuchMoreConcat(str)));

    return 0;
}
