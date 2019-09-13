#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "filedeal.h"    //文件处理头文件(打开\保存)
#include "find_dialog.h"
#include "basicoperation.h"
#include"api_add.h"
#include<string>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->splitter_2->setStretchFactor(0,4);
    ui->splitter_2->setStretchFactor(1,1);          //设置splitter的两个框的比例为 ,0表示右（上），1表示左（下） 右边表示1:7
    textEdit = new QsciScintilla;

    //copy的文本框初值
    textcopy = new QsciScintilla;
    bfx=0; bfy=0;
    ischanged = false;
    select_change=true;
    isdelete = false;
    s_bfx=0;s_bfy=0;s_x=0;s_y=0;
    isopenfile = false;
    //copy的文本框初值
    textEdit->setObjectName(QStringLiteral("textEdit"));
    textEdit->setMarginType(0,QsciScintilla::NumberMargin);//设置编号为0的页边显示行号
    textEdit->setMarginWidth(0,25);//设置页边宽度
    ui->splitter->addWidget(textEdit);
    ui->splitter->addWidget(textcopy);
    ui->splitter->setStretchFactor(0,1);
    ui->splitter->setStretchFactor(1,6);
    //设置textEdit的一些功能
    QsciLexerCPP *textLexer;
    textLexer = new QsciLexerCPP;
    apiadd(textLexer,textEdit);
    textEdit->setLexer(textLexer);
    //文件信息初值
    currentfile=0;
    newfile=1;
    //文件信息初值
    //工具栏初始状态为不能操作
    textEdit->setEnabled(false);

    //工具栏初始状态为不能操作
    action_closeChoose = new QAction("关闭",this);
    /*以下部分是函数信号槽，处理函数放在头文件basicoperation.h*/
    connect(ui->actioncpy,SIGNAL(triggered()),this,SLOT(on_copy()));        //复制
    connect(ui->actioncut,SIGNAL(triggered()),this,SLOT(cut_it()));             //剪切
    connect(ui->actionpst,SIGNAL(triggered()),this,SLOT(paste_it()));           //粘贴
    connect(ui->actionsal,SIGNAL(triggered()),this,SLOT(select_all()));         //全选
    connect(ui->action_redo,SIGNAL(triggered()),this,SLOT(redo_it()));      //重做
    connect(ui->actionreg,SIGNAL(triggered()),this,SLOT(reg_it()));             //恢复
    connect(ui->action_n,&QAction::triggered,this,[=](){NewFile();});           //新建文件
    connect(ui->action_op,&QAction::triggered,this,[=](){OpenFile();});         //打开文件
    connect(ui->action_save,&QAction::triggered,this,[=](){SaveFile();});        //保存文件
    connect(ui->action_sava,&QAction::triggered,this,[=](){Save_asFile();});        //另存为文件
    connect(ui->action_close,&QAction::triggered,this,[=](){CloseCurrenteFile();});        //关闭当前打开的文件
    connect(this,&MainWindow::WindowClose,this,[=](){CloseCurrenteFile();});        //关闭当前打开的文件
    connect(action_closeChoose,&QAction::triggered,this,[=](){CloseChooseFile();});        //关闭选择的文件
    connect(ui->actionrun,&QAction::triggered,this,[=](){if(SaveFile())if(edit_it())run_it();});      //运行
    connect(ui->actionedit,&QAction::triggered,this,[=](){if(SaveFile())edit_it();});          //编译
    connect(ui->actioncpr,&QAction::triggered,this,[=](){if(SaveFile())edit_it();});          //编译运行
    connect(ui->actionann,SIGNAL(triggered()),this,SLOT(ann_it()));   //行内注释
    connect(ui->actioncann,SIGNAL(triggered()),this,SLOT(cann_it()));   //取消注释函数
    connect(ui->actionind,SIGNAL(triggered()),this,SLOT(ind_it()));   //缩进函数
    connect(ui->actioncind,SIGNAL(triggered()),this,SLOT(cind_it()));   //取消缩进函数
    connect(ui->action_hide,SIGNAL(triggered()),this,SLOT(hide_comment()));   //隐藏所有注释
    connect(ui->action_display,SIGNAL(triggered()),this,SLOT(display_comment()));   //显示所有注释
    connect(ui->actionfs,SIGNAL(triggered()),this,SLOT(full_screen()));    //全屏信号槽
    connect(ui->actionsrh,&QAction::triggered,this,&MainWindow::search_show);   //搜索
    connect(ui->actionrep,&QAction::triggered,this,&MainWindow::replace_show);  //替换
    connect(ui->actionsdfs,SIGNAL(triggered()),this,SLOT(mark_it()));   //注释



    connect(ui->treeWidget,SIGNAL(itemClicked(QTreeWidgetItem*,int)),this,SLOT(TreeWidgetClick(QTreeWidgetItem *,int)));   //选择并切换文件
    connect(textEdit,SIGNAL(cursorPositionChanged(int,int)),this,SLOT(cursor_change(int,int)));     //光标位置改变
    connect(textEdit,SIGNAL(textChanged()),this,SLOT(text_change()));   //文本变动
    connect(textEdit,SIGNAL(selectionChanged()),this,SLOT(selection_change()));   //选取变动

}

MainWindow::~MainWindow()
{
    delete ui;
}

