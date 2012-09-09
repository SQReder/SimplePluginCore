#include <QtCore/QtPlugin>
#include <QtCore>

#include <iostream>
#include <stdexcept>
#include <assert.h>

#include "ConsoleColor.h"

using namespace std;
using namespace std;

#include "LispPlugin.h"
Env global_env;

//===============================================================
void LispPlugin::init() {
    QVariant evalAlias("eval Lisp.runLispScript");
    CallExternal("Core.addAlias", evalAlias);
}

//===============================================================
const QString LispPlugin::getPluginId() const {
    return QString("Lisp");
}
//===============================================================
const long LispPlugin::Version() const {
    return 0x01000000;
}
//===============================================================
QStringList LispPlugin::getMethodList() const {
    QStringList methodNames;

    methodNames << "runLispScript";

    return methodNames;
}
//===============================================================
QVariant LispPlugin::CallInternal(const QByteArray methodName, QVariant &param) {
    BEGIN_EXPORTED_SELECTOR_BY(methodName);

    EXPORT_METHOD(runLispScript);

    THROW_METHOD_NOT_EXPORTED;
}
//===============================================================
Q_EXPORT_PLUGIN2(LispPlugin, LispPlugin)
//===============================================================
class Lambda
{
public:
    Lambda() {
        throw runtime_error("there's no default constructor");
    }

    Lambda(LispPlugin* parent, QVariant vars, QVariant body) :
        _parent(parent),
        _vars(vars.toStringList()),
        _body(body) {}

    QVariant exec(QVariantList args, Env& env) {
        Env localEnv = Env::Env(&env);

        if (args.size() != _vars.size()) {
            throw runtime_error(
                        QString("wrong arguments count in call: wants %1, got %2")
                        .arg(_vars.size(), args.size())
                        .toStdString()
                        );
        }

        for(auto i = 0; i < _vars.size(); ++i) {
            localEnv.insert(_vars[i], args[i]);
        }

        return _parent->eval(_body, localEnv);
    }

private:
    LispPlugin *_parent ;
    QStringList _vars;
    QVariant _body;
};
Q_DECLARE_METATYPE ( Lambda )
/* ========================================================================== */
QByteArray LispPlugin::tokenToString(QVariant token) {
    switch (token.type()) {
    case QVariant::List: {
        QByteArray result;
        result.append("(");

        bool isFirstToken = true;
        foreach(auto tok, token.toList()) {
            auto stringedToken = tokenToString(tok);

            if (!isFirstToken) result.append(" ");
            else isFirstToken = false;

            result.append(stringedToken);
        }

        result.append(")");
        return result;
    }
    case QVariant::Double: {
        auto doubleValue = token.toDouble();
        return QByteArray::number(doubleValue, 'g', 11);
    }
    default:
        return token.toByteArray();
    }

    return "O_o";
}

/* ========================================================================== */
void LispPlugin::printSyntaxTree(QVariant token, int d) {

    switch (token.type()) {
    case QVariant::List:
        foreach(auto tok, token.toList()) {
            printSyntaxTree(tok, d + 1);
        }
        break;
    case QVariant::String:
    case QVariant::ByteArray:
    default:
        for(int i = 0; i < d; ++i) cout << "  ";
        cout << qPrintable(token.toString()) << endl;
    }
    cout.flush();
}
/* ========================================================================== */
QVariantList LispPlugin::tokenize(QByteArray program) {
    auto str_tokenz = program.replace("(", " ( ")
            .replace(")", " ) ")
            .simplified()
            .split(' ');

    QVariantList tokenz;
    foreach(auto token, str_tokenz)
        tokenz.append(token);

    return tokenz;
}
/* ========================================================================== */
QVariant LispPlugin::read_from(QVariantList& tokenz) {
    if (tokenz.isEmpty())
        throw runtime_error("unexpected EOF while reading");

    QByteArray token = tokenz.takeFirst().toByteArray();
    if (token == "(") {
        //auto L = QVariant(QVariant::List);
        QVariantList L;
        while (tokenz[0] != ")")
            L.append(read_from(tokenz));
        tokenz.takeFirst(); // pop off )
        return L;
    } else if (token == ")") {
        throw std::runtime_error("enexcepted )");
    } else {
        bool successCast;

        auto i = token.toInt(&successCast);
        if (successCast) return i;

        auto d = token.toDouble(&successCast);
        if (successCast) return d;

        return QString(token);
    }
}
/* ========================================================================== */
QVariant LispPlugin::parseSrc(QByteArray s) {
    auto tokenz = tokenize(s);
    return read_from(tokenz);
}
/* ========================================================================== */
bool LispPlugin::isa(QVariant ltok, QVariant::Type type) {
    return ltok.type() == type;
}

bool LispPlugin::isa(QVariant ltok, const char* typeName) {
    return QString::compare(typeName, ltok.typeName());
}
/* ========================================================================== */
void LispPlugin::load(QVariant &ltok, QVariant &tok1, QVariant &tok2,
          QVariant &tok3, QVariant &tok4) {
    auto list = ltok.toList();
    auto lcount = list.size();

    if (lcount > 1) tok1 = list[1];
    if (lcount > 2) tok2 = list[2];
    if (lcount > 3) tok3 = list[3];
    if (lcount > 4) tok4 = list[4];
}

void LispPlugin::load(QVariant &ltok, QVariant &tok0, QVariant &tok1, QVariant &tok2)
{
    QVariant dumb;
    return load(ltok, tok0, tok1, tok2, dumb);
}

void LispPlugin::load(QVariant &ltok, QVariant &tok0, QVariant &tok1)
{
    QVariant dumb;
    return load(ltok, tok0, tok1, dumb, dumb);
}

void LispPlugin::load(QVariant &ltok, QVariant &tok0)
{
    QVariant dumb;
    return load(ltok, tok0, dumb, dumb, dumb);
}
/* ========================================================================== */
QVariant LispPlugin::exec(QVariant procName, QVariant _exps) {
    QVariant result;
    assert(isa(_exps, "StringList"));
    QVariantList exps = _exps.toList();
    if ("*" == procName) {
        auto a = exps[0].toDouble();
        auto b = exps[1].toDouble();
        return a * b;
    } else if ("+" == procName) {
        auto a = exps[0].toDouble();
        auto b = exps[1].toDouble();
        return a + b;
    }  else if ("-" == procName) {
        auto a = exps[0].toDouble();
        auto b = exps[1].toDouble();
        return a - b;
    } else if ("<=" == procName) {
        auto a = exps[0].toDouble();
        auto b = exps[1].toDouble();
        return a <= b;
    } else {
        if (exps.size() == 1)
            _exps = exps.takeFirst();
        return CallExternal(procName.toByteArray(), _exps);
    }


    throw runtime_error(
                QString("Unable to resolve symbol: %1 in this context")
                .arg(procName.toString()).toStdString());
}
/* ========================================================================== */
QVariant LispPlugin::eval(QVariant x, Env& env = global_env) {
    QVariant result(QVariant::List);
    Env localEnv = Env::Env(&env);

    QVariant firstToken;
    if (x.canConvert(QVariant::List)) {
        if (x.toList().size() != 0)
            firstToken = x.toList()[0];
        else
            return QVariantList();
    }

    if (isa(x, QVariant::String)) {
        QString symbolName = x.toString();
        auto someEnv = env.findvar(symbolName);
        if (someEnv == NULL)
            throw runtime_error(
                    QString("Unable to resolve symbol: %1 in this context")
                    .arg(symbolName).toStdString());
        result = someEnv->value(symbolName);
    }
    else if (isa(x, QVariant::UserType)) {
        if (isa(x, "Lambda")) {
            Lambda l = x.value<Lambda>();
            result = l.exec(x.toList(), localEnv);
        } else {
            return x;
        }
    }
    else if (!isa(x, QVariant::List)) {
        result = x;
    }
    else if ("quote" == firstToken) { // (quote exp)
        QVariant exp(QVariant::String);
        load(x, exp);
        result = exp;
    }
    else if ("if" == firstToken) { // (if test conseq alt)
        QVariant test, conseq, alt;
        load(x, test, conseq, alt);
        test = eval(test, localEnv);
        auto bTest = test.toBool();
        result = eval(bTest ? conseq : alt, localEnv);
    }
    else if ("define" == firstToken) { // (define var exp)
        QVariant var, exp;
        load(x, var, exp);
        env[var.toString()] = eval(exp, localEnv);
    }
    else if ("set!" == firstToken) { // (set! var exp)
        QVariant var, exp;
        load(x, var, exp);
        auto symbolName = var.toString();
        auto someEnv = env.findvar(symbolName);
        if (someEnv == NULL)
            env[symbolName] = eval(exp, localEnv);
        else
            (*someEnv)[symbolName] = eval(exp, localEnv);
    }
    else if ("begin" == firstToken) { // (begin exp ...)
        auto list = x.toList();
        QListIterator<QVariant> it(list);
        it.next();
        while (it.hasNext())
            result = eval(it.next(), localEnv);
    }
    else if ("lambda" == firstToken) {
        auto args = x.toList();
        result = QVariant::fromValue(Lambda(this, args[1], args[2]));
    }
    else { // (proc exp)
        QVariantList exps = x.toList();
        auto proc = exps.takeFirst();
        auto fname = proc.toString();

        for(int i = 0; i != exps.size(); ++i) {
            exps[i] = eval(exps[i], localEnv);
        }

        auto lambdaEnv = env.findvar(fname);
        if (lambdaEnv == NULL)
            result = exec(proc, exps);
        else {
            auto lambdaVar = (*lambdaEnv)[fname];
            if (!isa(lambdaVar, QVariant::UserType))
                throw runtime_error(
                        QString("Unable to resolve function: %1 in this context")
                        .arg(fname).toStdString());
            auto lambda = lambdaVar.value<Lambda>();
            result = lambda.exec(exps, localEnv);
        }
    }

    return result;
}

void LispPlugin::repl() {
    QTextStream in(stdin);
    QString input;
    do {
        cout << "lisp>>";
        cout.flush();
        input = in.readLine();
        if (input == "") continue;

        QByteArray program(input.toLocal8Bit());
        try {
            auto result = eval(parseSrc(program), global_env);
            cout << tokenToString(result).data() << endl;
        } catch (runtime_error &e) {
            cout << e.what() << endl;
        }
    } while (1);
}

/* ========================================================================== */
void LispPlugin::runTest(QByteArray source, QByteArray expectated, Env& env = global_env) {
    cout << source.data() << " => ";
    cout.flush();

    try {
        auto program = parseSrc(source);
        auto current = tokenToString(eval(program, env));
        bool isTestPassed = (current == expectated);

        cout << current.data() << "\t";
        if (isTestPassed) {
            ConsoleColor col(FOREGROUND_INTENSITY|FOREGROUND_GREEN);
            cout << "PASSED";
        } else {
            ConsoleColor col(FOREGROUND_INTENSITY|FOREGROUND_RED);
            cout << "FAILED wants " << expectated.data();
        }
        cout << endl;
    } catch (runtime_error &e) {
        ConsoleColor col(FOREGROUND_INTENSITY|FOREGROUND_RED);
        cout << "FAILED with exception: " << e.what() << endl;
    }
}

/* ========================================================================== */
void LispPlugin::tests() {
    // пустая программа
    runTest("()", "()");
    runTest("1", "1");

    // простое цитирование
    runTest("(quote word)", "word");
    runTest("(quote (some long sentence))", "(some long sentence)");

    // ветвления
    runTest("(if 1 (quote true) (quote false))", "true");
    runTest("(if 0 (quote true) (quote false))", "false");

    // глобальные переменные по-умолчанию
    global_env.insert("test", "true");
    runTest("test", "true");

    global_env.insert("conseq", "true");
    global_env.insert("alt", "false");

    // проверка значений записей по-умолчанию
    runTest("(if test conseq alt)", "true");

    global_env["test"] = "false";
    runTest("(if test conseq alt)", "false");

    // определение переменной в в коде
    runTest("(define x 1)", "()");

    // проверка значения переменной
    runTest("(if x 1 0)", "1");

    runTest("(set! x 0)", "()");

    runTest("(if x 1 0)", "0");

    runTest("(set! x (+ x 1))", "()");

    runTest("x", "1");

    runTest("(begin (set! x 1) (set! x (+ x 1)) (* x 2))", "4");

    runTest("x", "2");

    // пробуем лямбды
    runTest("(define area (lambda (r) (* 3.141592653 (* r r))))", "()");
    runTest("(area 3)", "28.274333877");

    runTest("(define fact (lambda (n) (if (<= n 1) 1 (* n (fact (- n 1))))))", "()");
    runTest("(fact 10)", "3628800");
    runTest("(fact 100)", "9.3326215444e+157");
}
/* ========================================================================== */
QVariant LispPlugin::runLispScript(QVariant source) {
    try {
        return eval(parseSrc(source.toByteArray()));
    } catch (runtime_error &e) {
        cout << endl << "Evaluate error: " << e.what() << endl;
    } catch (exception &e) {
        cout << "Unexpected exception: " << e.what() << endl;
    }

    return QVariant(QVariant::Invalid);
}
