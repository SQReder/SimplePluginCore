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
                   << "dup Basic.Duplicate"
                   << "test Console.testLisp";

    qDebug("Load default alias list...");
    QStringListIterator it(defaultAliases);
    while(it.hasNext()) {
        QVariant alias(it.next());
        CallExternal("Core.addAlias", alias);
    }
}
//===============================================================
QStringList ConsolePlugin::getMethodList() const {
    QStringList methodNames;

    methodNames << "StartConsole"
                << "listAliases"
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
QVariant ConsolePlugin::resolveCall(QVariant& param) {
    if (!param.isValid() || param.isNull())
        return QVariant();

    QString callString = param.toString();
    QString methodName = callString.split(QRegExp("[\t ]+")).takeFirst();
    QString callParams = callString.remove(0, methodName.length() + 1);

    auto paramsV = QVariant(callParams);
    auto methodNameB = QByteArray(methodName.toLocal8Bit());
    QVariant result = CallExternal(methodNameB, paramsV);
    if (result.type() != QVariant::Invalid) {
        qDebug("---------------");
        PrintQVariantValue(result);
        qDebug("---------------");
    }

    return result;
}
//===============================================================
void ConsolePlugin::PrintQVariantValue(QVariant &val) {
    switch (val.type()) {
    case QVariant::StringList: {
        foreach (auto str, val.toStringList()) {
            cout << qPrintable(str) << endl;
        }
        break;
    }
    case QVariant::List: {
        foreach (auto item, val.toList()) {
            PrintQVariantValue(item);
        }
        break;
    }
    case QVariant::ByteArray: {
        cout << val.toByteArray() << endl;
        break;
    }
    default:
        cout << val.typeName() << " : " << val.toString();
    }
}
//===============================================================
