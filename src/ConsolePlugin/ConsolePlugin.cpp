#include <QtCore/QtPlugin>
#include <QString>
#include <QMap>
#include <QTextStream>
#include <QtDebug>

#include "ConsolePlugin.h"

using namespace std;

Q_EXPORT_PLUGIN2(Consoleplugin, ConsolePlugin)

QTextStream cin(stdin);
QTextStream cout(stdout);
//===============================================================
const QString ConsolePlugin::getPluginId() const {
    return QString("Console");
}
//===============================================================
const long ConsolePlugin::Version() const {
    return 0x01010000;
}
//===============================================================
QStringList ConsolePlugin::getMethodList() {
    QStringList methodNames;

    methodNames << "StartConsole"
                << "listAliases"
                << "createAlias"
                << "ParseMethodCall";

    return methodNames;
}
//===============================================================
QByteArray* ConsolePlugin::CallInternal(const QByteArray& methodName,
                                        QByteArray* param) {
    BEGIN_EXPORTED_SELECTOR_BY(methodName);
    EXPORT_METHOD_NOPARAMS(StartConsole);
    EXPORT_METHOD_NORETURN(createAlias);
    EXPORT_METHOD_NOPARAMS(listAliases);
    EXPORT_METHOD(ParseMethodCall);
    RETURN_RESULT;
}
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
    createAlias(new QByteArray("con Basic.echo"));
    createAlias(new QByteArray("alias Console.createAlias"));
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
    } else {
        resolveAliases(commandLine);
        return true;
    }

    cout << "unknown command" << endl;
    return true;
}
//===============================================================
QByteArray *ConsolePlugin::ParseMethodCall(QByteArray* command) {
    QRegExp regex("^(\\w+\\.\\w+)(\\s*)(.*)$");
    QStringList parts;

    QString cmd(*command);
    int hasMatch = regex.indexIn(cmd);
    if (hasMatch == -1) {
        cout << "wrong function call " << *command << endl;
        return NULL;
    }

    parts << regex.capturedTexts();

    QString method(parts[1]);
    QString param = parts[3];

    cout << "call for  : " << method << endl;
    cout << "params is : " << param << endl;

    return CallExternal(QByteArray(method.toLocal8Bit()),
                                      new QByteArray(param.toLocal8Bit()));
}
//===============================================================
void ConsolePlugin::createAlias(QByteArray *param) {
    if (param == NULL) {
        cout << *listAliases();
        return;
    }

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
void ConsolePlugin::resolveAliases(QString& param) {
    if (param == NULL) return;

    QStringList parts = param.split(QRegExp("[\t ]+"));

    bool aliased = aliases.contains(parts[0]);
    if (aliased) {
        parts[0] = aliases[parts[0]];
    }

    QByteArray method;
    method.append(parts[0]);
    QByteArray _param;
    QStringListIterator it(parts);
    it.next();
    if (it.hasNext()) {
        _param.append(it.next());
        while(it.hasNext()) {
            _param.append(" ");
            _param.append(it.next());
        }
    }

    QByteArray *result = CallExternal(method, _param.isEmpty() ? NULL : &_param);
    qDebug("result:\n");
    if (result != NULL)
        cout << *result << endl;

}
//===============================================================
