#include <QtCore/QCoreApplication>
#include <QtGui/QApplication>
#include <stdio.h>
#include "HiveCore.h"
#include "PluginInterface.h"

using namespace std;
HiveCore* hive;

//===============================================================
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    hive = new HiveCore();

    QString concat("BasicPlugin.Concat");
    QString param = "param";
    void* paramPtr = reinterpret_cast<void*>(&param);

    void* res = hive->CallPluginMethod(concat, paramPtr);
    QString str = *reinterpret_cast<QString*>(res);
    printf("result is %s\n", qPrintable(str));

    delete hive;

    return a.exec();
}
//===============================================================
