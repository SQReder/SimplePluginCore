#include "BasicPlugin.h"

#include <stdio.h>

QByteArray* BasicPlugin::Concat(const QByteArray* one)
{
    QString str(*one);
    QByteArray arr;
    arr.append(str + "+" + str);
    return new QByteArray(arr);
}

QByteArray* BasicPlugin::MuchMoreConcat(const QByteArray* str) {
//    QString method("BasicPlugin.Concat");
//    return Concat(str) + "::" + CallExternalMethod<QString, QString>(method, str);
    return NULL;
}

QByteArray* BasicPlugin::FunctionWithoutParams(void) {
    QByteArray str("param");
    QByteArray* res = MuchMoreConcat(&str);

    printf("%s\n", res->data());

    return 0;
}
