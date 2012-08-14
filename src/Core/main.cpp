#include <QtCore/QCoreApplication>
#include <QtGui/QApplication>
#include <stdio.h>
#include "HiveCore.h"
#include "PluginInterface.h"

using namespace std;
HiveCore* hive = HiveCore::Instance();

//===============================================================
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    hive = HiveCore::Instance();
    hive->loadPlugins();

    QStringList loadedMethods = hive->listLoadedMethods();
    foreach(QString str, loadedMethods) {
        printf("%s\n", qPrintable(str));
    }

//    QString concat("BasicPlugin.MuchMoreConcat");
    QString concat("BasicPlugin.Concat");
    QString param = "param";
    void* paramPtr = reinterpret_cast<void*>(&param);

    void* res = hive->CallPluginMethod(concat, paramPtr);
    QString str = *reinterpret_cast<QString*>(res);
    printf("result is %s\n", qPrintable(str));

    return a.exec();
}
//===============================================================
