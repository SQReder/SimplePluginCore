#include <stdexcept>
#include <QtGui/QApplication>
#include <QDir>
#include <QPluginLoader>
#include <QPair>
#include <QStringList>

#include "HiveCore.h"
//===============================================================
HiveCore::HiveCore() {
    loadPlugins();
}
//===============================================================
int HiveCore::LoadPluginContent(QObject* pobj) {
    if (!pobj) {
        throw std::runtime_error("Plugin wasn't instanced!");
    }

    PluginInterface* pI = qobject_cast<PluginInterface*>(pobj);
    if (pI) {
//        pI->SetCoreCallback(&HiveCore::CallPluginMethod);

        QList<QString> pluginMethodList = pI->getPluginMethods();
        methods.insert(pI, pluginMethodList);
        return pluginMethodList.count();
    } else {
        throw std::runtime_error("Wrong plugin interface!");
    }
}
//===============================================================
void HiveCore::loadPlugins() {
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
PluginInterface* HiveCore::locateMethod(QString methodName) {
    foreach (PluginInterface* pI, methods.keys()) {
        QStringList methodsList = methods[pI];
        if (methodsList.contains(methodName))
            return pI;
    }

    printf("method %s not loaded\n", qPrintable(methodName));
    return NULL;
}
//===============================================================
void* HiveCore::CallPluginMethod(QString& methodName, const void* params) {
#ifndef QT_NO_DEBUG
    printf("call method %s\n", qPrintable(methodName));
#endif

    PluginInterface* pI = locateMethod(methodName);
    //check for method are loaded
    if (!pI) {

        throw std::runtime_error(qPrintable(methodName));
    } else {
        return pI->CallInternal(methodName, params);
    }
}
