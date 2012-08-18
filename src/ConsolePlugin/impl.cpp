#include <QTextStream>
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
    } else if (parts[0] == "ls") {
        ParseMethodCall(QString("call Core.listLoadedMethods "));
        return true;
    } else if (parts[0] == "call"){
        ParseMethodCall(commandLine);
        return true;
    }

    cout << "unknown command" << endl;
    return true;
}
//===============================================================
void ConsolePlugin::ParseMethodCall(QString cmd) {
    QRegExp regex("^(call)\\s(\\w+\\.\\w+)\\s*(.*)$");
    QStringList parts;

    int pos = regex.indexIn(cmd);
    if (pos == -1) {
        cout << "wrong function call" << endl;
        return;
    }

    parts << regex.capturedTexts();

    QString method(parts[2]);
    QString param = parts.count() > 2 ? parts[3] : "";

    cout << "call for  : " << method << endl;
    cout << "params is : " << param << endl;

    QByteArray* result = CallExternal(QByteArray(method.toLocal8Bit()), new QByteArray(param.toLocal8Bit()));
    cout << "result : " << *result << endl;
}
