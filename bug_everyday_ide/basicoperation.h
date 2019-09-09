#ifndef BASICOPERATION_H
#define BASICOPERATION_H
#include "mainwindow.h"
#include "ui_mainwindow.h"
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
    textEdit->copy();
    ui->statusBar->showMessage(tr("    拷贝成功"),1500);        //在状态栏中显示信息，时间为1.5秒
}
void MainWindow::select_all()       //调用Qt中自带的全选函数
{
    textEdit->selectAll();
    ui->statusBar->showMessage(tr("    已经全部选择"),1500);
}
void MainWindow::cut_it()              //调用Qt中自带的剪切函数
{
    textEdit->cut();
    ui->statusBar->showMessage(tr("    剪切成功"),1500);
}
void MainWindow::paste_it()         //调用Qt中自带的粘贴函数
{
    textEdit->paste();
}
void MainWindow::redo_it()          //调用Qt中自带的重做函数
{
    textEdit->redo();
    ui->statusBar->showMessage(tr("    成功还原上一步操作"),1500);
}
void MainWindow::reg_it()           //调用Qt中自带的恢复函数
{
    textEdit->undo();
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

void MainWindow::ann_it()     //行类注释的功能
{
//    QTextCursor cursor;
//    cursor = ui->EditWidget->textCursor();
//    QString str=cursor.selectedText();
//    int length = str.length();
//    QString first = "/*";
//    QString last ="*/";
//    if(str.mid(0,2) == first && str.mid(length-2) == last)      //判断是否存在注释
//  //mid（n,m）函数截取从第n个字符后的m个字符，如果无m则截取到结尾
//    {
//        cursor.insertText(str.mid(2,length-4));
//    }
//    else  cursor.insertText("/*"+str+"*/");
}
void MainWindow::mark_it()     //注释
{
//    QTextCursor str = ui->EditWidget->textCursor();


}
void MainWindow::cann_it()    //取消注释
{

}

void MainWindow::ind_it()     //添加缩进的功能
{
//    QTextCursor cursor;
//    cursor = ui->EditWidget->textCursor();
//    QString str=cursor.selectedText();
//    QString cmp = "\n";
//    int length = str.length();
//    for(int i = 0; i < length ;i++)
//    {
//        qDebug()<<str.mid(i,1);
//        qDebug()<<cmp;
//        if(str.mid(i,1)==10)
//        {
//            str = str.mid(0,i+1)+"\t"+str.mid(i+1);
//            i++;
//            length = length+1;
//        }
//    }
//    qDebug()<<length;
//    qDebug()<<str;
//   // cursor.insertText("\t"+str);

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
//       if(event->modifiers()==Qt::ControlModifier)
//       {
//           if(event->key()==Qt::Key_Slash)
//                ann_it();
//       }

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

void MainWindow::search_show(){  //搜索窗口
    find_dialog *find_dlg = new find_dialog();
    find_dlg->setAttribute(Qt::WA_DeleteOnClose);
    find_dlg->setWindowTitle(tr("查找"));
    connect(find_dlg,&find_dialog::start_search,this,[=](){search(find_dlg);});
    connect(find_dlg,&find_dialog::start_replace,this,[=](){search(find_dlg);replace(find_dlg);});
    find_dlg->show();
}

void MainWindow::search(find_dialog *find_dlg)   //搜索
{
    QString str = find_dlg->find_text;
    bool isfind;
    if(find_dlg->distinguish)   //true区分大小写，false不区分
        isfind = textEdit->findFirst(str,false,true,false,false);
    else
        isfind = textEdit->findFirst(str,false,false,false,false);
    if(!isfind)//查找不到
        qDebug()<<QString(tr("查找%1失败").arg(str));
    else
        qDebug()<<QString(tr("查找%1成功").arg(str));

}

void MainWindow::replace(find_dialog *find_dlg) //替换
{
//    QString str = find_dlg->replace_text;
//    QTextCursor replace_cursor = ui->EditWidget->textCursor();
//    replace_cursor.removeSelectedText();
//    replace_cursor.insertText(str);
//    //设置光标
//    int end = replace_cursor.position();
//    int start = replace_cursor.position()-str.length();
//    replace_cursor.setPosition(start,QTextCursor::MoveAnchor);
//    replace_cursor.setPosition(end,QTextCursor::KeepAnchor);
//    ui->EditWidget->setTextCursor(replace_cursor);
//    //ui->EditWidget->find(str,QTextDocument::FindBackward);
}
#endif // BASICOPERATION_H
