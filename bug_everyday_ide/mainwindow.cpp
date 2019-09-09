#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "filedeal.h"    //文件处理头文件(打开\保存)
#include "find_dialog.h"
#include "basicoperation.h"
#include<string>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->splitter_2->setStretchFactor(0,4);
    ui->splitter_2->setStretchFactor(1,1);          //设置splitter的两个框的比例为 ,0表示右（上），1表示左（下） 右边表示1:7
    textEdit = new QsciScintilla;
    textEdit->setObjectName(QStringLiteral("textEdit"));
    textEdit->setMarginType(0,QsciScintilla::NumberMargin);//设置编号为0的页边显示行号
    textEdit->setMarginWidth(0,25);//设置页边宽度
    ui->splitter->addWidget(textEdit);
    ui->splitter->setStretchFactor(0,1);
    ui->splitter->setStretchFactor(1,6);
    //设置textEdit的一些功能
    QsciLexerCPP *textLexer;
    textLexer = new QsciLexerCPP;
    textLexer->setColor(QColor(Qt::darkGreen),QsciLexerCPP::DoubleQuotedString);
    textLexer->setColor(QColor(Qt::darkRed),QsciLexerCPP::Keyword);
    textLexer->setColor(QColor(Qt::darkGreen),QsciLexerCPP::PreProcessor );
    textEdit->setLexer(textLexer);
    QsciAPIs *apis = new QsciAPIs(textLexer);
    //在这里可以添加自定义的自动完成函数
    //apis->add(QString("func_name(arg_1,arg_2) function info"));
    apis->prepare();
    //设置自动完成所有项
    textEdit->setAutoCompletionSource(QsciScintilla::AcsAll);
    //设置大小写敏感
    textEdit->setAutoCompletionCaseSensitivity(true);
    //每输入1个字符就出现自动完成的提示
    textEdit->setAutoCompletionThreshold(1);
    //开启自动缩进
    textEdit->setAutoIndent(true);
    //设置缩进的显示方式
    textEdit->setIndentationGuides(QsciScintilla::SC_IV_LOOKBOTH);
    //左侧行号显示的背景色
    textEdit->setMarginsBackgroundColor(Qt::lightGray);
    //设置括号匹配
    textEdit->setBraceMatching(QsciScintilla::SloppyBraceMatch);

    filenum=0;
    currentfile=0;
    cus=0;
    /*以下部分是函数信号槽，处理函数放在头文件basicoperation.h*/
    connect(ui->actioncpy,SIGNAL(triggered()),this,SLOT(on_copy()));        //复制
    connect(ui->actioncut,SIGNAL(triggered()),this,SLOT(cut_it()));             //剪切
    connect(ui->actionpst,SIGNAL(triggered()),this,SLOT(paste_it()));           //粘贴
    connect(ui->actionsal,SIGNAL(triggered()),this,SLOT(select_all()));         //全选
    connect(ui->action_redo,SIGNAL(triggered()),this,SLOT(redo_it()));      //重做
    connect(ui->actionreg,SIGNAL(triggered()),this,SLOT(reg_it()));             //恢复
    connect(ui->action_n,&QAction::triggered,this,[=](){NewFile();});           //新建文件
    connect(ui->action_op,&QAction::triggered,this,[=](){OpenFile();});         //打开文件
    connect(ui->action_save,&QAction::triggered,this,[=](){SaveFile(currentfile);});        //保存文件
    connect(ui->action_sava,&QAction::triggered,this,[=](){Save_asFile();});        //另存为文件
    connect(ui->action_close,&QAction::triggered,this,[=](){CloseFile();});        //关闭当前文件
    connect(ui->actionrun,&QAction::triggered,this,[=](){SaveFile(currentfile);if(edit_it())run_it();});      //运行
    connect(ui->actionedit,&QAction::triggered,this,[=](){SaveFile(currentfile);edit_it();});          //编译
//    connect(ui->actionann,SIGNAL(triggered()),this,SLOT(ann_it()));   //注释函数
//    connect(ui->actioncann,SIGNAL(triggered()),this,SLOT(cann_it()));   //取消注释函数
//    connect(ui->actionind,SIGNAL(triggered()),this,SLOT(ind_it()));   //缩进函数
//    connect(ui->actioncind,SIGNAL(triggered()),this,SLOT(cind_it()));   //取消缩进函数
    connect(ui->actionfs,SIGNAL(triggered()),this,SLOT(full_screen()));    //全屏信号槽
    connect(ui->actionsrh,&QAction::triggered,this,&MainWindow::search_show);   //搜索
//    connect(ui->actionsdfs,SIGNAL(triggered()),this,SLOT(mark_it()));   //替换
    connect(ui->treeWidget,SIGNAL(itemClicked(QTreeWidgetItem*,int)),this,SLOT(TreeWidgetClick(QTreeWidgetItem *,int)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

