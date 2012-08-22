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

    CallPluginMethod("Console.StartConsole");

    return 0;//a.exec();
}
//===============================================================
