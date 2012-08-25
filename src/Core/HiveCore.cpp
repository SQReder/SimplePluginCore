#include <stdexcept>
#include <QtGui/QApplication>
#include <QDir>
#include <QPluginLoader>
#include <QPair>
#include <QStringList>

#include "HiveCore.h"
#include "PluginInterface.h"
//===============================================================
int HiveCore::LoadPluginContent(QObject* pobj) {
    if (!pobj) {
        throw std::runtime_error("Plugin wasn't instanced!");
    }

    PluginInterface* pI = qobject_cast<PluginInterface*>(pobj);
    if (pI) {
        pI->SetCoreCallback(&::CallPluginMethod);

        QList<QString> pluginMethodList = pI->getPluginMethods();
        loadedMethods.insert(pI, pluginMethodList);
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
PluginInterface* HiveCore::locateMethod(QByteArray methodName) {
    foreach (PluginInterface* pI, loadedMethods.keys()) {
        QStringList methodsList = loadedMethods[pI];
        if (methodsList.contains(methodName))
            return pI;
    }

    throw std::runtime_error(methodName.data());
}
//===============================================================
QByteArray HiveCore::CallPluginMethod(const QByteArray& methodName,
                                       QByteArray &params) {
#ifndef QT_NO_DEBUG
    qDebug("call method %s", methodName.data());
#endif
    try {
        if(methodName.mid(0,4) == "Core") {
            return CallCoreMetod(methodName.mid(5), params);
        } else {
            PluginInterface* pI = locateMethod(methodName);
            return pI->CallInternal(methodName, params);
        }
    } catch (std::runtime_error) {
        printf("METHOD %s DOESN'T FOUND\n", methodName.data());
        return NULL;
    }
}
//===============================================================
QByteArray HiveCore::CallCoreMetod(const QByteArray &methodName, QByteArray &param) {
    if (methodName == "listLoadedMethods") {
        QStringList list = listLoadedMethods();
        QByteArray result;
        foreach(QString name, list) {
            result.append(name);
            result.append("\n");
        }
        return QByteArray(result);
    }
    throw std::runtime_error(methodName.data());
}

//===============================================================
const QStringList HiveCore::listLoadedMethods() const {
     QStringList list;
     foreach(QStringList methodList, loadedMethods.values()) {
         foreach(QString name, methodList)
            list.push_back(name);
     }
     return list;
}
