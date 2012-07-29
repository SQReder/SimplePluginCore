#ifndef MYPLUGIN_H
#define MYPLUGIN_H
#include "../../MonoInterface.h"
#include <QObject>
#include "Concat.h"

//===============================================================
class BasicPlugin: public QObject, public MonoInterface
{
    Q_OBJECT
    Q_INTERFACES(MonoInterface)
public:
    FuncMap operations() const;
    void operation(const QString& strText, const QString& strOperation);
};
//===============================================================
#endif // MYPLUGIN_H
