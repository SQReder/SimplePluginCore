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
        printf("Plugin file: %s\n", qPrintable(strFileName));
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
template<class T, class F>
F CallPluginMethod(QString& methodName, T param) {
//#ifndef QT_NO_DEBUG
//    cout << "call method " << qPrintable(methodName) << endl;
//#endif

//    // check for method are loaded
//    if (!methods.contains(methodName)) {
//        printf("method %s not loaded", qPrintable(methodName));
//        return NULL;
//    }

//    // get pointer to method
//    CoolVoidFunc func = methods[methodName];

//    // prepare param to send to plugin method
//    void* VoidParam = (void*) &param;

//    // and call method
//    void* result = func(VoidParam);


//    return *reinterpret_cast<F*>(result);
}

//===============================================================
PluginInterface* locateMethod(QString methodName)
{
    return NULL;
}

//===============================================================
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    loadPlugins();

//    printf("test call plugin function\n");
//    QString method("concat");
//    QString param("SQReder");
//    QString str;
//    str = CallPluginMethod<QString, QString>(method, param);
//    printf("result: %s", qPrintable(str));

    return a.exec();
}
//===============================================================
