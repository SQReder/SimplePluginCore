#include <QTextStream>
#include <QtDebug>
using namespace std;

#include "ConsolePlugin.h"

QTextStream cin(stdin);
QTextStream cout(stdout);
//===============================================================
QByteArray* ConsolePlugin::StartConsole() {
    InitializeConsole();
    ShowWelcome();

    QString input;
    do {
        cout << prompt;
        cout.flush();
        input = cin.readLine();
    } while (CommandParser(input));
    return NULL;
}
//===============================================================
void ConsolePlugin::InitializeConsole() {
    prompt = ">>";
    createAlias(new QByteArray("ls Core.listLoadedMethods"));
    createAlias(new QByteArray("call Console.ParseMethodCall"));
}
//===============================================================
void ConsolePlugin::ShowWelcome() {
    cout << "console started" << endl;
}
//===============================================================
bool ConsolePlugin::CommandParser(QString& commandLine) {
    QStringList parts = commandLine.split(QRegExp("[\t ]+"));
    if (parts[0] == "exit") {
        cout << "end session" << endl;
        return false;
    }
    /* else if (parts[0] == "ls") {
        ParseMethodCall(QString("call Core.listLoadedMethods "));
        return true;
    } */
    else if (parts[0] == "call"){
        ParseMethodCall(commandLine);
        return true;
    }

    cout << "unknown command" << endl;
    return true;
}
//===============================================================
void ConsolePlugin::ParseMethodCall(QString cmd) {
    QRegExp regex("^(call)\\s(\\w+\\.\\w+)(\\s*)(.*)$");
    QStringList parts;

    int hasMatch = regex.indexIn(cmd);
    if (hasMatch == -1) {
        cout << "wrong function call" << endl;
        return;
    }

    parts << regex.capturedTexts();

    QString method(parts[2]);
    QString param = parts[4];

    cout << "call for  : " << method << endl;
    cout << "params is : " << param << endl;

    QByteArray* result = CallExternal(QByteArray(method.toLocal8Bit()),
                                      new QByteArray(param.toLocal8Bit()));

    cout << "result:\n"
         << (result ? *result : "")
         << endl;
}
//===============================================================
void ConsolePlugin::createAlias(QByteArray *param) {
    QRegExp regex("^(\\w+)\\s+(\\w+\\.\\w+)$");
    QStringList parts;

    QString paramStr = QString(*param);

    bool match = regex.indexIn(paramStr) != -1;
    if (!match) {
        cout << "Incorrect syntax! Syntax: set aliasName Plugin.Function"
             << endl;
    } else {
        parts << regex.capturedTexts();

        qDebug("creating alias %s for %s\n", qPrintable(parts[1]),
               qPrintable(parts[2]));

        aliases[parts[1]] = parts[2];
    }
}
//===============================================================
QByteArray* ConsolePlugin::listAliases() {
    QByteArray result;
    result.append("Current aliases list:\n");
    QHashIterator<QString, QString> it(aliases);
    while (it.hasNext()) {
        it.next();
        result.append(it.key());
        result.append("\t");
        result.append(it.value());
        result.append("\n");
    }
    return new QByteArray(result);
}
//===============================================================
