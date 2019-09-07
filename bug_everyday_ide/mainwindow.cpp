#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "filedeal.h"    //文件处理头文件(打开\保存)
#include<string>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->splitter->setStretchFactor(0,1);
    ui->splitter->setStretchFactor(1,6);
    ui->splitter_2->setStretchFactor(0,4);
    ui->splitter_2->setStretchFactor(1,1);          //设置splitter的两个框的比例为 ,0表示右（上），1表示左（下） 右边表示1:7
    ui->EditWidget->setEnabled(false);
    ui->ResultWidget->setEnabled(false);
    filenum=0;
    currentfile=0;
    cus=0;
    /*以下部分是函数信号槽*/


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
    connect(ui->actionann,SIGNAL(triggered()),this,SLOT(ann_it()));   //注释函数
    connect(ui->actioncann,SIGNAL(triggered()),this,SLOT(cann_it()));   //取消注释函数
    connect(ui->actionind,SIGNAL(triggered()),this,SLOT(ind_it()));   //缩进函数
    connect(ui->actioncind,SIGNAL(triggered()),this,SLOT(cind_it()));   //取消缩进函数
    connect(ui->actionfs,SIGNAL(triggered()),this,SLOT(full_screen()));    //全屏信号槽
    connect(ui->treeWidget,SIGNAL(itemClicked(QTreeWidgetItem*,int)),this,SLOT(TreeWidgetClick(QTreeWidgetItem *,int)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::sleep(unsigned int tim)        //延时函数
{
       QTime reachtime = QTime::currentTime().addMSecs(tim);        //获取当前时间加上你需要延时的时间
       while(QTime::currentTime()<reachtime)
       {
           QCoreApplication::processEvents(QEventLoop::AllEvents,100);   // 最小循环时间是0.1秒
       }
}

void MainWindow::on_copy()          //调用Qt中自带的拷贝函数
{
    ui->EditWidget->copy();
    ui->statusBar->showMessage(tr("    拷贝成功"),1500);        //在状态栏中显示信息，时间为1.5秒
}
void MainWindow::select_all()       //调用Qt中自带的全选函数
{
    ui->EditWidget->selectAll();
    ui->statusBar->showMessage(tr("    已经全部选择"),1500);
}
void MainWindow::cut_it()              //调用Qt中自带的剪切函数
{
    ui->EditWidget->cut();
    ui->statusBar->showMessage(tr("    剪切成功"),1500);
}
void MainWindow::paste_it()         //调用Qt中自带的粘贴函数
{
    ui->EditWidget->paste();
}
void MainWindow::redo_it()          //调用Qt中自带的重做函数
{
    ui->EditWidget->redo();
    ui->statusBar->showMessage(tr("    成功还原上一步操作"),1500);
}
void MainWindow::reg_it()           //调用Qt中自带的恢复函数
{
    ui->EditWidget->undo();
    ui->statusBar->showMessage(tr("    已经回到后一步操作"),1500);
}

void MainWindow::full_screen()
{
    showFullScreen();
    Fullsize = true;
    ui->statusBar->showMessage(tr("    已成功进入全屏模式"),1500);
    sleep(1500);
    ui->statusBar->showMessage(tr("    按下ESC按键退出全屏"));
}
bool MainWindow::edit_it()//编译
{
    QString destname = myfile[currentfile].Path;
    QString fpath = myfile[currentfile].Path;
    destname.replace(".c",".exe");
    QString command = "gcc -o " + destname +" "+ fpath + " 2> error.txt";
    system(command.toStdString().data());
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QFile file("error.txt");
    file.open(QIODevice::ReadWrite);
    QByteArray array = file.readAll();
    ui->ResultWidget->setText(codec->toUnicode(array));
    file.close();
    if(!array.isEmpty()){//编译错误
        qDebug()<<"edit error!";
        return false;
//        ui->setupUi(editerror);
    }
    else//编译成功
    {
        qDebug()<<"edit success!";
        return true;
    }
}
void MainWindow::run_it()//运行
{
    QString destname = myfile[currentfile].Path;
    destname.replace(".c",".exe");
    system(destname.toStdString().data());
    qDebug()<<"run success!";
}
void MainWindow::ann_it()     //添加注释的功能
{
    QTextCursor cursor;
    cursor = ui->EditWidget->textCursor();
    QString str=cursor.selectedText();
    cursor.insertText("/*"+str+"*/");
}
void MainWindow::cann_it()    //取消注释
{

}
void MainWindow::ind_it()     //添加缩进的功能
{
    QTextCursor cursor;
    cursor = ui->EditWidget->textCursor();
    QString str=cursor.selectedText();
    //str.replace("\\n","\n");
    cursor.insertText("\t"+str);
    qDebug()<<str;
   // cursor.insertText("\t"+str);

}
void MainWindow::cind_it()   //取消缩进
{

}
void   MainWindow:: keyPressEvent(QKeyEvent *event)  //实现一些键盘操作
   {
       if(event->key()==Qt::Key_Escape&&Fullsize)
       {
           Fullsize = false;
           showNormal();
            ui->statusBar->showMessage(" ");   //清空状态栏中的信息
       }
       if(event->modifiers()==Qt::ControlModifier)
       {
           if(event->key()==Qt::Key_Slash)
                ann_it();
       }

   }

void MainWindow::on_treeWidget_customContextMenuRequested(const QPoint &pos)           //右键文件菜单栏关闭(目前只实现关闭当前打开的文件，无法关闭在文件列表中的其它文件)
{
    QTreeWidgetItem* curItem=ui->treeWidget->itemAt(pos);  //获取当前被点击的节点
    if(curItem==NULL)return;           //这种情况是右键的位置不在treeItem的范围内，即在空白位置右击
    QVariant var = curItem->data(0,Qt::UserRole);
    if(0 == var)      //data(...)返回的data已经在之前建立节点时用setdata()设置好
    {
        QMenu *popMenu =new QMenu(this);//定义一个右键弹出菜单
        popMenu->addAction(ui->action_close);//往菜单内添加QAction   该action在前面用设计器定义了
        popMenu->exec(QCursor::pos());//弹出右键菜单，菜单位置为光标位置
    }
    else
    {
        QMenu *popMenu =new QMenu(this);//定义一个右键弹出菜单
        popMenu->addAction(ui->action_close);//往菜单内添加QAction   该action在前面用设计器定义了
        popMenu->exec(QCursor::pos());//弹出右键菜单，菜单位置为光标位置
    }

}
