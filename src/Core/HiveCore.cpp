#include <QtGui/QApplication>
#include <QtCore>
#include <stdexcept>
#include <iostream>

using namespace std;

#include "HiveCore.h"
#include "PluginInterface.h"
//===============================================================
const QString HiveCore::getPluginId() const { return "Core"; }
//===============================================================
const long HiveCore::Version() const { return 0x01010000; }
//===============================================================
QStringList HiveCore::getMethodList() const { return QStringList(); }
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
        pI->init();
        return pluginMethodList.count();
    } else {
        throw std::runtime_error("Wrong plugin interface!");
    }
}
//===============================================================
void HiveCore::loadPlugins() {
    QDir dir(QApplication::applicationDirPath());
    cout << "path : " <<  qPrintable(dir.path()) << endl;
    if (!dir.cd("plugins")) {
        qFatal("plugins directory does not exist");
        return;
    }

    foreach (QString strFileName, dir.entryList(QDir::Files)) {
        QPluginLoader loader(dir.absoluteFilePath(strFileName));
        QObject* inst(loader.instance());
        if (inst == NULL) {
            qWarning("Error loading plugin : %s", qPrintable(loader.errorString()));
        } else {
            try {
                LoadPluginContent(inst);
            } catch (std::runtime_error &e){
                qCritical("Error loading plugin %s : %s", qPrintable(strFileName), e.what());
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
QVariant HiveCore::CallPluginMethod(const QByteArray methodName,
                                       QVariant &params) {
#ifndef QT_NO_DEBUG
    qDebug("call method %s", methodName.data());
#endif

    auto result = QVariant();
    auto unaliasedName = unaliasMethodName(methodName);

    if(unaliasedName.mid(0,4) == "Core") {
        result = CallInternal(unaliasedName, params);
    } else {
        PluginInterface* pI = locateMethod(unaliasedName);

        if (pI != NULL) {
            try {
                result = pI->CallInternal(unaliasedName, params);
            } catch (std::runtime_error &e) {
                cout << "Call error: " << e.what() << endl;
            }
        }
        else
            cout << "method " << unaliasedName.data() << " doesn't found" << endl;
    }

    return result;
}
//===============================================================
QVariant HiveCore::CallInternal(const QByteArray methodName, QVariant &param) {
    BEGIN_EXPORTED_SELECTOR_BY(methodName);
    EXPORT_METHOD_NOPARAMS(listLoadedMethods);
    EXPORT_METHOD_NORETURN(addAlias);
    EXPORT_METHOD_NOPARAMS(listAliases);
    THROW_METHOD_NOT_EXPORTED;
}

//===============================================================
QVariant HiveCore::listLoadedMethods() const{
     QStringList list;
     foreach(QStringList methodList, loadedMethods.values()) {
         foreach(QString name, methodList)
            list.push_back(name);
     }

     return list;
}
//===============================================================
void HiveCore::addAlias(QVariant& param) {
    auto params = param.toByteArray().split(' ');
    if (params.size() != 2)
        throw runtime_error(QString("Wrong arguments count: wants 2 got %1")
                            .arg(params.size()).toStdString());
    aliases[params[0]] = params[1];
}
//===============================================================
const QByteArray HiveCore::unaliasMethodName(const QByteArray &methodName) {
    if (aliases.contains(methodName))
        return aliases[methodName];
    else
        return methodName;
}
//===============================================================
QStringList HiveCore::listAliases() {
    QStringList list;
    QHashIterator<QByteArray, QByteArray> it(aliases);
    while(it.hasNext()) {
        it.next();
        list.append(it.key() + " " + it.value());
    }

    return list;
}
//===============================================================
