#ifndef MYPLUGIN_H
#define MYPLUGIN_H
#include "../Core/PluginInterface.h"
#include <QObject>

//===============================================================
class BasicPlugin: public QObject, public PluginInterface
{
    Q_OBJECT
    Q_INTERFACES(PluginInterface)

    QList<QString> getPluginMethods() const;
    QString getPluginId() const;
    void* Call(PluginMethosName methodName, const void* param);
};

template<class ParamType, class ReturnType>
void* MethodWrapper(ReturnType (*methodPtr)(const ParamType&),
                    const void* param);
//===============================================================
#endif // MYPLUGIN_H
