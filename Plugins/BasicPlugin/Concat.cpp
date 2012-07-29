#include <QObject>
#include "Concat.h"

//QString concat(const QString& one, const QString& two) {

//}

QString ConcatDescription()
{
    return "string concatenation";
}

void* ConcatFunction(const void* param) {
    QString str = *(QString*)param;
    return new QString(str + str);
}

QString concat(const QString& one, const QString& two);
