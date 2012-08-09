#include <QObject>
#include "Concat.h"

QString Concat(const QString& one)
{
    return one + "+" + one;
}

QString MuchMoreConcat(const QString& str) {
    return Concat(str) + "::" + Concat(str);
}
