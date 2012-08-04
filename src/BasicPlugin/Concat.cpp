#include <QObject>
#include "Concat.h"

QString ConcatDescription()
{
    return "string concatenation";
}

void* ConcatWrapper(const void* param) {
    // кастуем параметры в нужный тип и разыменовываем указатель
    const QString* strPtr = reinterpret_cast<const QString*>(param);
    QString str = *strPtr;

    // вызываем собственно функцию, сохраняя результат
    QString result = ConcatFunc(str, str);

    // и возвращаем результат, обязательно в новом экземпляре класса.
    return reinterpret_cast<void*>(new QString(result));
}

QString ConcatFunc(const QString& one, const QString& two)
{
    return one + two;
}
