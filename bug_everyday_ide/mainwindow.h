#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QFileDialog>
#include <QDebug>
#include <QMessageBox>
#include <QTreeWidgetItem>
#include<QKeyEvent>
#include<QTime>
#include<QApplication>
#include "myfile.h"
#include "find_dialog.h"
#include <Qsci/qsciscintilla.h>
//Lua语言的词法分析器
#include <Qsci/qscilexerlua.h>
//自动补全的apis
#include <Qsci/qsciapis.h>
#include<Qsci/qscilexercpp.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void SetifEnabled();
    void NewFile();   //新建文件
    void OpenFile();  //打开文件
    bool SaveFile();  //保存文件
    bool Save_asFile(); //另存为文件
    void InitFileList(); //初始化文件列表
    void addFileList(MyFile); //添加文件列表
    void CloseChooseFile();         //关闭选择的文件
    void CloseCurrenteFile();       //关闭当前打开的文件
    void keyPressEvent(QKeyEvent * event);  //获取键盘按键函数
    void sleep(unsigned int);   //延时运行函数
    bool Fullsize = false;   //用于记录是否处于全屏模式

public slots:
    void TreeWidgetClick(QTreeWidgetItem *item,int column);

private:
    Ui::MainWindow *ui;
    QsciScintilla *textEdit;
    QsciScintilla *textcopy;
    QVector<MyFile> myfile;  //我的文件类数组，包含文件各种信息,具体定义在头文件
    QAction *action_closeChoose;
    int newfile;       //新建文件的个数
    int filenum;       //文件总数
    int cus;           //记录文件编号
    int currentfile;   //当前打开文件编号
    int CurrentChoose; //右键选中的文件编号
    int bfx;
    int bfy;
    bool ischanged;
    bool select_change;
    bool ischanged_s;
    int s_bfx,s_bfy,s_x,s_y;
    bool isopenfile;
    bool isdelete;

    QString sss;

private slots:
        void on_copy();       //声明复制函数
        void select_all();     //声明全选函数函数
        void paste_it();       //声明粘贴函数
        void cut_it();           //声明剪切函数
        void redo_it();         //声明重做函数
        void reg_it();          //声明恢复函数
        bool edit_it();         //声明编译函数
        void run_it();           //声明运行函数
        void ann_it();      //声明添加行类注释函数
        void cann_it();     //声明取消注释函数
        void ind_it();      //声明缩进函数
        void cind_it();      //声明取消缩进函数
        void full_screen(); //声明全屏函数
        void hide_comment();  //隐藏全部注释
        void display_comment();  //展示全部注释
        void change_comment(int); //改变/*类注释的换行符至后台文本框的$$,或者将后台文本框的换行符$$转变成\r\n
        void on_treeWidget_customContextMenuRequested(const QPoint &pos);
        void mark_it();     //注释
        void search_show();      //声明搜索窗口函数
        void replace_show();
        void search(find_dialog *find_dlg);      //声明搜索函数
        void replace(find_dialog *find_dlg); //替换
        void text_change();
        void cursor_change(int x,int y);
        void selection_change();
};

#endif // MAINWINDOW_H
