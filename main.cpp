#include <QtCore/QCoreApplication>
#include <QtGui/QApplication>
#include <QDir>
#include <QPluginLoader>
#include <iostream>

using namespace std;

#include <MonoInterface.h>

FuncMap methods;
//===============================================================
void ListPluginSlots(QObject* pobj) {
    if (!pobj) {
        printf("Plugin wasn't instanced!\n");
        return;
    }

    MonoInterface* pI = qobject_cast<MonoInterface*>(pobj);
    if (pI) {
        FuncMap pluginContent = pI->operations();
        foreach (QString str, pluginContent.keys()) {
//            QAction* pact = new QAction(str, pobj);
//            connect(pact, SIGNAL(triggered()),
//                    this, SLOT(slotStringOperation())
//                   );
//            m_pmnuPlugins->addAction(pact);
            printf("%s\n", qPrintable(str));
        }
        methods.unite(pluginContent);
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

    printf("list %d plugins\n", dir.entryList(QDir::Files).count());

    foreach (QString strFileName, dir.entryList(QDir::Files)) {
        printf("lib file: %s\n", qPrintable(strFileName));
        QPluginLoader loader(dir.absoluteFilePath(strFileName));
        QObject* inst(loader.instance());
        if (inst == NULL) {
            printf("error: %s\n", qPrintable(loader.errorString()));
        } else {
            ListPluginSlots(inst);
        }
    }
}
//===============================================================
template<class T, class F>
F CallPluginMethod(QString& methodName, T param) {
#ifndef QT_NO_DEBUG
    cout << "call method " << qPrintable(methodName) << endl;
#endif

    // check for method are loaded
    if (!methods.contains(methodName)) {
        printf("method %s not loaded", qPrintable(methodName));
        return NULL;
    }

    // get pointer to method
    CoolVoidFunc func = methods[methodName];

    // prepare param to send to plugin method
    void* VoidParam = (void*) &param;

    // and call method
    void* result = func(VoidParam);


    return *reinterpret_cast<F*>(result);
}

//===============================================================
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    loadPlugins();

    printf("test call plugin function\n");
    QString method("concat");
    QString param("SQReder");
    QString str;
    str = CallPluginMethod<QString, QString>(method, param);
    printf("result: %s", qPrintable(str));

    return a.exec();
}
//===============================================================
