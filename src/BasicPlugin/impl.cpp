#include "BasicPlugin.h"

#include <stdio.h>

QByteArray* BasicPlugin::Concat(QByteArray* one)
{
    QString str(*one);
    QByteArray arr;
    arr.append(str + "+" + str);
    return new QByteArray(arr);
}

QByteArray* BasicPlugin::MuchMoreConcat(QByteArray* str) {
    QByteArray method("BasicPlugin.Concat");
    QByteArray* res = Concat(str);
    res->append("::");

    QByteArray* external = CallExternal(method, str);
    res->append(external->data());
    return new QByteArray(*res);
}

QByteArray* BasicPlugin::FunctionWithoutParams(void) {
    QByteArray str("param");
    QByteArray* res = MuchMoreConcat(&str);

    printf("%s\n", res->data());

    return 0;
}
