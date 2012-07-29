#ifndef PLUGINCONTENT_H
#define PLUGINCONTENT_H

class QString;
class QObject;

QString ConcatDescription();
void* ConcatFunction(const void* param);
QString concat(const QString& one, const QString& two);

#endif // PLUGINCONTENT_H
