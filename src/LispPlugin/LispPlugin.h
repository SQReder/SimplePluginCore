/** \file LispPlugin.h
\brief Интерфейс плагина
\details */
#pragma once

#include "../Core/PluginInterface.h"
#include <QObject>

class Env: public QVariantHash {
public:
    Env(Env* outer = NULL) { this->outer = outer; }

    Env *findvar(QString x) {
        if (this->contains(x))
            return this;
        else {
            if(outer != NULL) {
                return outer->findvar(x);
            }
        }

        return NULL;
    }

private:
    Env* outer;
};

//===============================================================
/** \brief Реализация интерфейсов для конкретного плагина */
class LispPlugin: public QObject, public PluginInterface
{
    Q_OBJECT
    /** \brief указывает компилятору, что этот класс является интерфейсом
            плагина */
    Q_INTERFACES(PluginInterface)

    // реализация виртуальных методов интерфейса
    const QString getPluginId() const;
    const long Version() const;
    QVariant CallInternal(const QByteArray methodName, QVariant& param);
protected:
    Env globalLispEnvironment;

    QStringList getMethodList() const;
};
//===============================================================
QVariant eval(QVariant x, Env& env);
QVariant runLispScript(QVariant source);

QByteArray tokenToString(QVariant token);
void printSyntaxTree(QVariant token, int d = -1);
QVariantList tokenize(QByteArray program);
QVariant read_from(QVariantList& tokenz);
QVariant parseSrc(QByteArray s);
bool isa(QVariant ltok, QVariant::Type type);
bool isa(QVariant ltok, const char* typeName);

void load(QVariant &ltok, QVariant &tok1, QVariant &tok2,
          QVariant &tok3, QVariant &tok4);
void load(QVariant &ltok, QVariant &tok0, QVariant &tok1, QVariant &tok2);
void load(QVariant &ltok, QVariant &tok0, QVariant &tok1);
void load(QVariant &ltok, QVariant &tok0);

QVariant exec(QVariant procName, QVariant _exps);
void repl();
void runTest(QByteArray source, QByteArray expectated, Env& env);
void tests();
