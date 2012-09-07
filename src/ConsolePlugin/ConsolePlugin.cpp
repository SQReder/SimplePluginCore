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
void ConsolePlugin::init() {
    prompt = ">>";
    QStringList defaultAliases;
    defaultAliases << "ls Core.listLoadedMethods"
                   << "call Console.ParseMethodCall"
                   << "dup Basic.Duplicate"
                   << "alias Console.createAlias"
                   << "p Console.ParseLisp"
                   << "test Console.testLisp";

    qDebug("Load default alias list...");
    QStringListIterator it(defaultAliases);
    while(it.hasNext()) {
        QByteArray alias;
        alias.append(it.next());
        QVariant param(alias);
        createAlias(param);
    }
}
//===============================================================
QStringList ConsolePlugin::getMethodList() const {
    QStringList methodNames;

    methodNames << "StartConsole"
                << "listAliases"
                << "createAlias"
                << "ParseMethodCall"
                << "runLispScript"
                << "resolveCall";

    return methodNames;
}
//===============================================================
QVariant ConsolePlugin::CallInternal(const QByteArray& methodName,
                                        QVariant& param) {
    BEGIN_EXPORTED_SELECTOR_BY(methodName);
    EXPORT_METHOD_NOPARAMS(StartConsole);
    EXPORT_METHOD_NORETURN(createAlias);
    EXPORT_METHOD_NOPARAMS(listAliases);
    EXPORT_METHOD(resolveCall);
    THROW_METHOD_NOT_EXPORTED;
}
//===============================================================
QVariant ConsolePlugin::StartConsole() {
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
void ConsolePlugin::ShowWelcome() {
    cout << " :: Console started :: " << endl;
}
//===============================================================
bool ConsolePlugin::CommandProcessor(QString& commandLine) {
    QStringList parts = commandLine.split(QRegExp("[\t ]+"));
    if (parts[0] == "q") {
        return false;
    } else {
        QVariant commandLineV(commandLine);
        resolveCall(commandLineV);
        return true;
    }

    cout << "unknown command" << endl;
    return true;
}
//===============================================================
void ConsolePlugin::createAlias(QVariant &param) {
    QString command = param.toString();
    if (command.isEmpty()) {
        cout << listAliases().toString();
        return;
    }

    QRegExp regex("^(.+)\\s+(\\w+\\.\\w+)$");
    QStringList parts;

    bool match = regex.indexIn(command) != -1;
    if (!match) {
        cout << "Using: alias aliasName Plugin.Function"   << endl;
    } else {
        parts << regex.capturedTexts();

        qDebug("%s\t %s", qPrintable(parts[1]),
               qPrintable(parts[2]));

        aliases[parts[1]] = parts[2];
    }
}
//===============================================================
QVariant ConsolePlugin::listAliases() {
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
QVariant ConsolePlugin::resolveCall(QVariant& param) {
    if (!param.isValid() || param.isNull())
        return QVariant();

    QString callString = param.toString();
    QString methodName = callString.split(QRegExp("[\t ]+")).takeFirst();
    QString callParams = callString.remove(0, methodName.length() + 1);

    bool isMethodNameAliased = aliases.contains(methodName);
    if (isMethodNameAliased) {
        methodName = aliases[methodName];
    }

    auto paramsV = QVariant(callParams);
    auto methodNameB = QByteArray(methodName.toLocal8Bit());
    QVariant result = CallExternal(methodNameB, paramsV);
    if (result.type() != QVariant::Invalid) {
        qDebug("---------------");
        cout << result.toString() << endl;
        qDebug("---------------");
    }

    return result;
}
