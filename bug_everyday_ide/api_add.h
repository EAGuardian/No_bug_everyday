#ifndef API_ADD_H
#define API_ADD_H
#include "mainwindow.h"
#include "ui_mainwindow.h"
void apiadd(QsciLexerCPP *textLexer, QsciScintilla *textEdit)
{
    QsciAPIs *api = new QsciAPIs(textLexer);
    api->add(QString(""));
    //#开头文件
    api->add(QString("#include"));
    api->add(QString("#define"));
    api->add(QString("#ifdef"));
    api->add(QString("#else"));

    //关键字
    api->add(QString("auto"));
    api->add(QString("break"));
    api->add(QString("case"));
    api->add(QString("char"));
    api->add(QString("const"));
    api->add(QString("continue"));
    api->add(QString("default"));
    api->add(QString("do"));
    api->add(QString("double"));
    api->add(QString("else"));
    api->add(QString("enum"));
    api->add(QString("extern"));
    api->add(QString("float"));
    api->add(QString("for"));
    api->add(QString("goto"));
    api->add(QString("if"));
    api->add(QString("int"));
    api->add(QString("long"));
    api->add(QString("register"));
    api->add(QString("return"));
    api->add(QString("short"));
    api->add(QString("signed"));
    api->add(QString("sizeof"));
    api->add(QString("static"));
    api->add(QString("struct"));
    api->add(QString("switch"));
    api->add(QString("typedef"));
    api->add(QString("union"));
    api->add(QString("unsigned"));
    api->add(QString("void"));
    api->add(QString("volatile"));
    api->add(QString("while"));

    //头文件注释
    api->add(QString("stdio.h"));
    api->add(QString("stdlib.h"));
    api->add(QString("string.h"));
    api->add(QString("ctype.h"));
    api->add(QString("math.h"));

    api->prepare();
    textLexer->setColor(QColor(Qt::darkGreen),QsciLexerCPP::DoubleQuotedString);
    textLexer->setColor(QColor(Qt::darkRed),QsciLexerCPP::Keyword);
    textLexer->setColor(QColor(Qt::darkGreen),QsciLexerCPP::PreProcessor );
    textEdit->setAutoCompletionSource(QsciScintilla::AcsAll);
    textEdit->setAutoCompletionCaseSensitivity(true);   //设置大小写敏感
    textEdit->setAutoCompletionThreshold(1); //出现一个字符就提示补全信息
    textEdit->setAutoIndent(true); //开启自动缩进
    textEdit->setIndentationGuides(QsciScintilla::SC_IV_LOOKBOTH); //设置缩进的显示方式
    textEdit->setMarginsBackgroundColor(Qt::lightGray); //左侧行号显示的背景色
    textEdit->setBraceMatching(QsciScintilla::SloppyBraceMatch);//设置括号匹配
    textEdit->setMatchedBraceBackgroundColor(QColor(180,238,180));  //设置匹配色
    textEdit->setMatchedBraceForegroundColor(Qt::red);
}
#endif // API_ADD_H
