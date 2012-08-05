#include <QtCore/QCoreApplication>
#include <QtGui/QApplication>
#include <QDir>
#include <QPluginLoader>
#include <QPair>
#include <QStringList>
#include <iostream>

using namespace std;

#include "PluginInterface.h"

typedef  QMap<PluginInterface*, QStringList> PluginMap;
PluginMap methods;
//===============================================================
void LoadPluginContent(QObject* pobj) {
    if (!pobj) {
        printf("Plugin wasn't instanced!\n");
        return;
    }

    PluginInterface* pI = qobject_cast<PluginInterface*>(pobj);
    if (pI) {
        QList<QString> pluginMethodList = pI->getPluginMethods();

        printf("List methods for plugin %s\n", qPrintable(pI->getPluginId()));
        foreach (QString methodName, pluginMethodList) {
            printf("%s\n", qPrintable(methodName));
        }

        methods.insert(pI, pluginMethodList);
    }
}
//===============================================================
void loadPlugins()
{
    QDir dir(QApplication::applicationDirPath());
    printf("path : %s\n", qPrintable(dir.path()));
    if (!dir.cd("plugins")) {
        printf("plugins directory does not exist");
        return;
    }

    foreach (QString strFileName, dir.entryList(QDir::Files)) {
        //printf("Plugin file: %s\n", qPrintable(strFileName));
        QPluginLoader loader(dir.absoluteFilePath(strFileName));
        QObject* inst(loader.instance());
        if (inst == NULL) {
//            printf("error: %s\n", qPrintable(loader.errorString()));
        } else {
            LoadPluginContent(inst);
        }
    }
}
//===============================================================
PluginInterface* locateMethod(QString methodName)
{
    foreach (PluginInterface* pI, methods.keys()) {
        QStringList methodsList = methods[pI];
        if (methodsList.contains(methodName))
            return pI;
    }

    return NULL;
}
//===============================================================
template<class ParamType, class ReturnType>
ReturnType CallPluginMethod(QString& methodName, ParamType param) {
#ifndef QT_NO_DEBUG
    cout << "call method " << qPrintable(methodName) << endl;
#endif

    PluginInterface* pI = locateMethod(methodName);
    //check for method are loaded
    if (!pI) {
        printf("method %s not loaded", qPrintable(methodName));
        return NULL;
    }

    // get pointer to method
//    CoolVoidFunc func = methods[methodName];

    // prepare param to send to plugin method
    void* voidParam = (void*) &param;

    // and call method
    void* result = pI->Call(methodName, voidParam);


    return *reinterpret_cast<ReturnType*>(result);
}

//===============================================================
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    loadPlugins();
    printf("\n");

    QString concat("BasicPlugin.Concat");

    QString str = CallPluginMethod<QString, QString>(concat, "param");
    printf("result is %s\n", qPrintable(str));

    return a.exec();
}
//===============================================================
