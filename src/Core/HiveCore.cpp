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
            try {
                LoadPluginContent(inst);
            } catch (std::runtime_error &e){
                printf("Error loading plugin %s : %s\n", qPrintable(strFileName), e.what());
            }
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

    return NULL;
}
//===============================================================
QVariant HiveCore::CallPluginMethod(const QByteArray& methodName,
                                       QVariant &params) {
#ifndef QT_NO_DEBUG
    qDebug("call method %s", methodName.data());
#endif

    auto result = QVariant(QVariant::Invalid);

    if(methodName.mid(0,4) == "Core") {
        result = CallCoreMetod(methodName.mid(5), params);
    } else {
        PluginInterface* pI = locateMethod(methodName);

        if (pI != NULL)
            result = pI->CallInternal(methodName, params);
        else
            printf("method %s doesn't found\n", methodName.data());
    }

    return result;
}
//===============================================================
QVariant HiveCore::CallCoreMetod(const QByteArray &methodName, QVariant &param) {
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
