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

    QByteArray param("param");

    QByteArray* res = CallPluginMethod("BasicPlugin.MuchMoreConcat", &param);
    QString str(*res);
    printf("result is %s\n", qPrintable(str));

    printf("try to call unexistent method\n");
    CallPluginMethod("NonExsistent", NULL);

    return a.exec();
}
//===============================================================
