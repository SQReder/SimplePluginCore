#include <QtCore/QCoreApplication>
#include <QtGui/QApplication>
#include <QDir>
#include <QPluginLoader>
#include <iostream>

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
            printf("error: %s", qPrintable(loader.errorString()));
        } else {
            ListPluginSlots(inst);
        }
    }
}
//===============================================================
void CallPluginMethod() {

}

//===============================================================
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    loadPlugins();

    return a.exec();
}
//===============================================================
