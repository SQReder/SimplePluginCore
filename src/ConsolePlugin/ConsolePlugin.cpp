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
    return 0x0201000;
}
//===============================================================
QStringList ConsolePlugin::getMethodList() const {
    QStringList methodNames;

    methodNames << "StartConsole"
                << "listAliases"
                << "createAlias"
                << "ParseMethodCall";

    return methodNames;
}
//===============================================================
QByteArray ConsolePlugin::CallInternal(const QByteArray& methodName,
                                        QByteArray& param) {
    BEGIN_EXPORTED_SELECTOR_BY(methodName);
    EXPORT_METHOD_NOPARAMS(StartConsole);
    EXPORT_METHOD_NORETURN(createAlias);
    EXPORT_METHOD_NOPARAMS(listAliases);
    RETURN_RESULT;
}
//===============================================================
QByteArray ConsolePlugin::StartConsole() {
    InitializeConsole();
    ShowWelcome();

    QString input;
    do {
        cout << prompt;
        cout.flush();
        input = cin.readLine();
    } while (CommandProcessor(input));
    return NULL;
}
//===============================================================
void ConsolePlugin::InitializeConsole() {
    prompt = ">>";
    QStringList defaultAliases;
    defaultAliases << "ls Core.listLoadedMethods"
                   << "call Console.ParseMethodCall"
                   << "dup Basic.Duplicate"
                   << "alias Console.createAlias";

    qDebug("Load default alias list...");
    QStringListIterator it(defaultAliases);
    while(it.hasNext()) {
        QByteArray alias;
        alias.append(it.next());
        createAlias(alias);
    }
}
//===============================================================
void ConsolePlugin::ShowWelcome() {
    cout << "Console started" << endl;
}
//===============================================================
bool ConsolePlugin::CommandProcessor(QString& commandLine) {
    QStringList parts = commandLine.split(QRegExp("[\t ]+"));
    if (parts[0] == "exit") {
        cout << "end session" << endl;
        return false;
    } else {
        resolveCall(commandLine);
        return true;
    }

    cout << "unknown command" << endl;
    return true;
}
//===============================================================
void ConsolePlugin::createAlias(QByteArray &param) {
    if (param.isEmpty()) {
        cout << listAliases();
        return;
    }

    QRegExp regex("^(\\w+)\\s+(\\w+\\.\\w+)$");
    QStringList parts;

    bool match = regex.indexIn(param) != -1;
    if (!match) {
        cout << "Incorrect syntax! Syntax: set aliasName Plugin.Function"
             << endl;
    } else {
        parts << regex.capturedTexts();

        qDebug("%s\t %s", qPrintable(parts[1]),
               qPrintable(parts[2]));

        aliases[parts[1]] = parts[2];
    }
}
//===============================================================
QByteArray ConsolePlugin::listAliases() {
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
    return result;
}
//===============================================================
void ConsolePlugin::resolveCall(QString& param) {
    if (param.isEmpty()) return;

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

    QByteArray result = CallExternal(method, _param);
    qDebug("result:\n");
    if (!result.isNull())
        cout << result << endl;

}
//===============================================================
