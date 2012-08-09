#include <QtCore/QCoreApplication>
#include "HiveCore.h"

using namespace std;
HiveCore* hive;

//===============================================================
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    hive = new HiveCore();
//    loadPlugins();
//    printf("\n");

//    QString concat("BasicPlugin.Concat");

//    QString str = CallPluginMethod<QString, QString>(concat, "param");
//    printf("result is %s\n", qPrintable(str));

    delete hive;

    return a.exec();
}
//===============================================================
