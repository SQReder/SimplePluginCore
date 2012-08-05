#ifndef PLUGINCONTENT_H
#define PLUGINCONTENT_H

class QString;
class QObject;

QString ConcatDescription();
void* ConcatWrapper(const void* param);
QString ConcatFunc(const QString& one);

#endif // PLUGINCONTENT_H
