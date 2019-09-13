#ifndef BASICOPERATION_H
#define BASICOPERATION_H
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<string>
#include<qstring.h>
//修改工具栏可否使用状态
void MainWindow::SetifEnabled(){
    bool Type;
    if(textEdit->isEnabled()) Type=true;
    else Type=false;
    ui->action_save->setEnabled(Type);
    ui->action_sava->setEnabled(Type);
    ui->action_close->setEnabled(Type);
    ui->actionrun->setEnabled(Type);
    ui->actionedit->setEnabled(Type);
    ui->action_hide->setEnabled(Type);
    ui->action_display->setEnabled(Type);
    ui->actionsrh->setEnabled(Type);
    ui->actionrep->setEnabled(Type);
    ui->actioncpr->setEnabled(Type);
    ui->actioncpy->setEnabled(Type);
    ui->actioncut->setEnabled(Type);
    ui->actionpst->setEnabled(Type);
    ui->action_redo->setEnabled(Type);
    ui->actionreg->setEnabled(Type);
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
    QString destname = myfile.at(currentfile).Path;
    QString fpath = myfile.at(currentfile).Path;
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
    }
    else//编译成功
    {
        qDebug()<<"edit success!";
        ui->ResultWidget->setText("edit success!");
        return true;
    }
}
void MainWindow::run_it()//运行
{
    QString destname = myfile.at(currentfile).Path;
    destname.replace(".c",".exe");
    system(destname.toStdString().data());
    qDebug()<<"run success!";
}




void MainWindow::ann_it()     //行类注释的功能
{
    int selectionStart =textEdit->SendScintilla(QsciScintillaBase::SCI_GETSELECTIONSTART);     //获取当前选择开头部分的位置
    int selectionEnd = textEdit->SendScintilla(QsciScintillaBase::SCI_GETSELECTIONEND);         //获取当前选择结束部分的位置
    QString str = textEdit->selectedText();
    QString first = "/*";
    QString last ="*/";
    int length = str.length();
    if(str.mid(0,2) == first && str.mid(length-2) == last)     //判断是否存在注释
    {
        textEdit->SendScintilla(QsciScintillaBase::SCI_DELETERANGE,selectionStart , 2);
         textEdit->SendScintilla(QsciScintillaBase::SCI_DELETERANGE,selectionEnd-4 , 2);
    }
    else
    {
        textEdit->SendScintilla(QsciScintillaBase::SCI_INSERTTEXT, selectionStart, "/*");
        textEdit->SendScintilla(QsciScintillaBase::SCI_INSERTTEXT, selectionEnd+2, "*/");
    }
    //末尾位置-4与+2是由于增加或删除了“/*”导致末尾位置发生变化做出的调整
}



void MainWindow::mark_it()     //注释&取消注释
{
    bool judge = true;
    int selectionStart =textEdit->SendScintilla(QsciScintillaBase::SCI_GETSELECTIONSTART);     //获取当前选择开头部分的位置
    int selectionEnd = textEdit->SendScintilla(QsciScintillaBase::SCI_GETSELECTIONEND);         //获取当前选择结束部分的位置
    int caretPosition = textEdit->SendScintilla(QsciScintillaBase::SCI_GETCURRENTPOS);      //获取当前位置

    qDebug()<<selectionEnd<<selectionStart<<caretPosition;

    int startLine = textEdit->SendScintilla(QsciScintillaBase::SCI_LINEFROMPOSITION,selectionStart);    //行开始的位置    从0作为第一排
    int endLine = textEdit->SendScintilla(QsciScintillaBase::SCI_LINEFROMPOSITION,selectionEnd);    //行结束的位置

     qDebug()<<startLine<<endLine;

    textEdit->SendScintilla(QsciScintillaBase::SCI_BEGINUNDOACTION);    //记录操作，可以被undo（）函数恢复

    for(int i = startLine; i <= endLine ; ++i)
    {
        int lineStart  = textEdit->SendScintilla(QsciScintillaBase::SCI_POSITIONFROMLINE,i);
        int lineEnd    = textEdit->SendScintilla(QsciScintillaBase::SCI_GETLINEENDPOSITION,i);
        int lineBufferSize = lineEnd - lineStart +1;       //获取第i行的字符个数，便于建立数组
        char *buf =new char[lineBufferSize];
        textEdit->SendScintilla(QsciScintillaBase::SCI_GETTEXTRANGE, lineStart, lineEnd,buf);//把字符串拷贝进buf这个字符数组
        QString str = QString(QLatin1String(buf)); //将buf转换为Qstring类型；
        if(str.mid(0,2) == "//")
            judge = true;
        else
        {
            judge = false;
            break;
        }
    }
    for(int i = startLine; i <= endLine ; ++i)
     {
         int lineStart  = textEdit->SendScintilla(QsciScintillaBase::SCI_POSITIONFROMLINE,i);       //获取这一行中的开头位置
        //int lineIndent = textEdit->SendScintilla(QsciScintillaBase::SCI_GETLINEINDENTPOSITION,i);  //获取这一行光标当前位置
         int lineEnd    = textEdit->SendScintilla(QsciScintillaBase::SCI_GETLINEENDPOSITION,i); //获取这一行中结束位置

         qDebug()<<"jdx"<<lineStart<<lineEnd;

         //lineIndent = lineStart;
         if(!judge) //判断选中的代码是否被注释过
         {
             textEdit->SendScintilla(QsciScintillaBase::SCI_INSERTTEXT, lineStart, "//");  //在这一行的lineIndent的位置插入“//”
         }
         else   textEdit->SendScintilla(QsciScintillaBase::SCI_DELETERANGE, lineStart, 2); //删除此行前两个字符
    }
}




void MainWindow::cann_it()    //取消注释
{
    bool judge = true;
    int selectionStart =textEdit->SendScintilla(QsciScintillaBase::SCI_GETSELECTIONSTART);     //获取当前选择开头部分的位置
    int selectionEnd = textEdit->SendScintilla(QsciScintillaBase::SCI_GETSELECTIONEND);         //获取当前选择结束部分的位置

    int startLine = textEdit->SendScintilla(QsciScintillaBase::SCI_LINEFROMPOSITION,selectionStart);    //行开始的位置    从0作为第一排
    int endLine = textEdit->SendScintilla(QsciScintillaBase::SCI_LINEFROMPOSITION,selectionEnd);    //行结束的位置

    textEdit->SendScintilla(QsciScintillaBase::SCI_BEGINUNDOACTION);    //记录操作，可以被undo（）函数恢复

    for(int i = startLine; i <= endLine ; ++i)
    {
        int lineStart  = textEdit->SendScintilla(QsciScintillaBase::SCI_POSITIONFROMLINE,i);
        int lineEnd    = textEdit->SendScintilla(QsciScintillaBase::SCI_GETLINEENDPOSITION,i);
        int lineBufferSize = lineEnd - lineStart +1;       //获取第i行的字符个数，便于建立数组
        char *buf =new char[lineBufferSize];
        textEdit->SendScintilla(QsciScintillaBase::SCI_GETTEXTRANGE, lineStart, lineEnd,buf);//把字符串拷贝进buf这个字符数组
        QString str = QString(QLatin1String(buf)); //将buf转换为Qstring类型；
        if(str.mid(0,2) == "//")
            judge = true;
        else
        {
            judge = false;
            break;
        }
    }
    for(int i = startLine; i <= endLine ; ++i)
     {
         int lineStart  = textEdit->SendScintilla(QsciScintillaBase::SCI_POSITIONFROMLINE,i);       //获取这一行中的开头位置
         if(judge) //判断选中的代码是否被注释过
            textEdit->SendScintilla(QsciScintillaBase::SCI_DELETERANGE, lineStart, 2); //删除此行前两个字符
    }
}


void MainWindow::ind_it()     //添加缩进的功能
{
    int selectionStart =textEdit->SendScintilla(QsciScintillaBase::SCI_GETSELECTIONSTART);     //获取当前选择开头部分的位置
    int selectionEnd = textEdit->SendScintilla(QsciScintillaBase::SCI_GETSELECTIONEND);         //获取当前选择结束部分的位置

    qDebug()<<selectionEnd<<selectionStart;

    int startLine = textEdit->SendScintilla(QsciScintillaBase::SCI_LINEFROMPOSITION,selectionStart);    //行开始的位置    从0作为第一排
    int endLine = textEdit->SendScintilla(QsciScintillaBase::SCI_LINEFROMPOSITION,selectionEnd);    //行结束的位置

     qDebug()<<startLine<<endLine;

    textEdit->SendScintilla(QsciScintillaBase::SCI_BEGINUNDOACTION);    //记录操作，可以被undo（）函数恢复

    for(int i = startLine; i <= endLine ; ++i)
     {
         int lineStart  = textEdit->SendScintilla(QsciScintillaBase::SCI_POSITIONFROMLINE,i);       //获取这一行中的开头位置
          textEdit->SendScintilla(QsciScintillaBase::SCI_INSERTTEXT, lineStart, "\t");  //在这一行的lineIndent的位置插入“//”
    }

}



void MainWindow::cind_it()   //取消缩进
{
    bool judge = true;
    bool flag = true;
    int selectionStart =textEdit->SendScintilla(QsciScintillaBase::SCI_GETSELECTIONSTART);     //获取当前选择开头部分的位置
    int selectionEnd = textEdit->SendScintilla(QsciScintillaBase::SCI_GETSELECTIONEND);         //获取当前选择结束部分的位置

    int startLine = textEdit->SendScintilla(QsciScintillaBase::SCI_LINEFROMPOSITION,selectionStart);    //行开始的位置    从0作为第一排
    int endLine = textEdit->SendScintilla(QsciScintillaBase::SCI_LINEFROMPOSITION,selectionEnd);    //行结束的位置

     qDebug()<<startLine<<endLine;

    textEdit->SendScintilla(QsciScintillaBase::SCI_BEGINUNDOACTION);    //记录操作，可以被undo（）函数恢复

    for(int i = startLine; i <= endLine ; ++i)
     {

         int lineStart  = textEdit->SendScintilla(QsciScintillaBase::SCI_POSITIONFROMLINE,i);       //获取这一行中的开头位置
         int lineEnd    = textEdit->SendScintilla(QsciScintillaBase::SCI_GETLINEENDPOSITION,i); //获取这一行中结束位置

         qDebug()<<"jdx"<<lineStart<<lineEnd;

         int lineBufferSize = lineEnd - lineStart +1;       //获取第i行的字符个数，便于建立数组

         char *buf =new char[lineBufferSize];
         textEdit->SendScintilla(QsciScintillaBase::SCI_GETTEXTRANGE, lineStart, lineEnd,buf);//把字符串拷贝进buf这个字符数组
         QString str = QString(QLatin1String(buf)); //将buf转换为Qstring类型；
         qDebug()<<str.mid(0,1);
         if(str.mid(0,1) == "\t" ||str.mid(0,8) == "        ")
         {
             if(str.mid(0,8) == "        ")
                 flag = true;
             else flag = false;
             judge = true;
         }
         else
             judge = false;
         if(judge) //判断选中代码是否有缩进并判断是哪种类型的缩进
         {
             if(flag)
                 textEdit->SendScintilla(QsciScintillaBase::SCI_DELETERANGE, lineStart, 8); //删除此行前8个字符
            else
                 textEdit->SendScintilla(QsciScintillaBase::SCI_DELETERANGE, lineStart, 1);
         }
    }
}




void MainWindow::change_comment(int Type){            //改变/*注释在后台编辑区中的形式，替换换行符
    if(!Type)            //Type为0表示将原先的\r\n换为$$,再拷贝至后台编辑区
    {
        QString Str=textEdit->text();
        int start=0,linefeed_start,linefeed_end,end;
        while(1)
        {
            start=Str.indexOf("/*",start);

            if(start!=-1)
            {
                end=Str.indexOf("*/",start);
                if(end==-1) end=2147483647;
                linefeed_start=start;
                while(1)
                {
                    linefeed_end=Str.indexOf("\r\n",linefeed_start);
                    if(linefeed_end>=end||linefeed_end==-1)
                    {
                        break;
                    }
                    if(linefeed_end!=-1)
                    {
                        linefeed_start=linefeed_end;
                        Str.replace(linefeed_end,2,"$$");
                    }
                }
                start=end;
            }
            else break;
        }
        textcopy->setText(Str);
    }
    else   //Type为1表示将后台编辑区的$$换为\r\n,再拷贝至前台编辑区
    {
        isopenfile=true;
        QString Str=textcopy->text();
        int start=0,linefeed_start,linefeed_end,end;
        while(1)
        {
            start=Str.indexOf("/*",start);

            if(start!=-1)
            {
                end=Str.indexOf("*/",start);
                if(end==-1) end=2147483647;
                linefeed_start=start;
                while(1)
                {
                    linefeed_end=Str.indexOf("$$",linefeed_start);
                    if(linefeed_end>=end||linefeed_end==-1)
                    {
                        break;
                    }
                    if(linefeed_end!=-1)
                    {
                        linefeed_start=linefeed_end;
                        Str.replace(linefeed_end,2,"\r\n");
                    }
                }
                start=end;
            }
            else break;
        }
        textEdit->setText(Str);
        isopenfile=false;
    }
}


void MainWindow::hide_comment(){    //隐藏注释
    change_comment(0);
    isopenfile=true;
    textEdit->setCursorPosition(0,1);
    int i,index_quotation_front,index_slash,index_quotation_back;
    QString LineStr;
    for(i=0;1;i++)    //遍历文本每一行
    {
        LineStr=textEdit->text(i);  //将每一行文本存入字符串
        if(LineStr.isEmpty()) break;  //文本为空，结束
        int quotation_start=0;   //寻找第一个双引号的起点
        while(1)
        {
            index_quotation_front=LineStr.indexOf("\"",quotation_start);  //先寻找第一个双引号
            if(index_quotation_front!=-1)     //第一个双引号存在
            {
                index_quotation_back=LineStr.indexOf("\"",index_quotation_front+1);  //寻找第二个双引号
                qDebug() << "index_quotation_back=" << index_quotation_back;
                if(index_quotation_back!=-1)  //第二个双引号存在，继续寻找双引号组，将寻找起点更新
                {
                    quotation_start=index_quotation_back+1;
                    qDebug() << "quotation_start=" << quotation_start;
                }
                else //如果第二个双引号不存在，该行代码是不规范的代码，注释无效，不隐藏
                    break;

            }
            else //没有双引号,直接找注释
            {
                index_slash=LineStr.indexOf("//",quotation_start);
                if(index_slash!=-1)   //有注释存在
                {
                      if(LineStr.indexOf("\n")==-1)
                      {
                          textEdit->setSelection(i,index_slash,i,LineStr.count());
                          textEdit->replaceSelectedText("");
                      }
                      else
                      {
                          textEdit->setSelection(i,index_slash,i,LineStr.count()-1);
                          textEdit->replaceSelectedText("\r\n");
                      }

                      qDebug() << LineStr;
                      qDebug () << textEdit->selectedText();
                      break;
                }
                else  //没有注释存在，继续下一行搜索
                    break;
            }
        }
    }
    //寻找/*类注释并隐藏
     QString Str=textEdit->text();
     int start,end;
     while(1)
     {
         start=Str.indexOf("/*");
         if(start!=-1)
         {
             end=Str.indexOf("*/",start);
             if(end!=-1)
                  Str.remove(start,end-start+2);
             else
                  Str.remove(start,Str.count()-start);
         }
         else break;
     }
     textEdit->setText(Str);
     isopenfile=false;
     ui->action_hide->setEnabled(false);
     ui->action_display->setEnabled(true);

}
void MainWindow::display_comment(){             //显示注释
    change_comment(1);
    ui->action_hide->setEnabled(true);
    ui->action_display->setEnabled(false);
}
void   MainWindow:: keyPressEvent(QKeyEvent *event)  //实现一些键盘操作
   {
       if(event->key()==Qt::Key_Escape&&Fullsize)
       {
           Fullsize = false;
           showNormal();
            ui->statusBar->showMessage("   ");   //清空状态栏中的信息
            qDebug()<<"1";
       }

   }

void MainWindow::on_treeWidget_customContextMenuRequested(const QPoint &pos)           //右键文件菜单栏关闭(目前只实现关闭当前打开的文件，无法关闭在文件列表中的其它文件)
{
    QTreeWidgetItem* curItem=ui->treeWidget->itemAt(pos);  //获取当前被点击的节点
    if(curItem==NULL)return;           //这种情况是右键的位置不在treeItem的范围内，即在空白位置右击
    CurrentChoose=curItem->text(2).toInt()-1;
    QMenu *popMenu =new QMenu(this);//定义一个右键弹出菜单
    popMenu->addAction(action_closeChoose);//往菜单内添加QAction   该action在前面用设计器定义了
    popMenu->exec(QCursor::pos());//弹出右键菜单，菜单位置为光标位置
}


void MainWindow::search_show(){  //搜索窗口
    find_dialog *find_dlg = new find_dialog(0,0);
    find_dlg->setAttribute(Qt::WA_DeleteOnClose);
    find_dlg->setWindowTitle(tr("查找"));
    connect(find_dlg,&find_dialog::start_search,this,[=](){search(find_dlg);});
    find_dlg->show();
}
void MainWindow::replace_show(){  //替换窗口
    find_dialog *find_dlg = new find_dialog(0,1);
    find_dlg->setAttribute(Qt::WA_DeleteOnClose);
    find_dlg->setWindowTitle(tr("查找"));
    connect(find_dlg,&find_dialog::start_replace,this,[=](){replace(find_dlg);});
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
    QString find_text = find_dlg->find_text;
    QString replace_text = find_dlg->replace_text;
    if(find_text.isEmpty()||replace_text.isEmpty()) return;
    textEdit->findFirst(find_text,false,false,false,false);
    textEdit->replaceSelectedText(replace_text);
    int line,index;
    int *l=&line,*i=&index;
    textEdit->getCursorPosition(l,i);
    textEdit->setSelection(line,index-replace_text.count(),line,index);
}
void MainWindow::text_change()
{
    if(!isopenfile)
    {
        ischanged = true;
    }


}

void MainWindow::cursor_change(int x,int y)
{
    QString line = textEdit->text(x);//统计缩进数量
    int t = line.count("\t");
    y -= t*7;
    if(ischanged)//文本变动
    {


        if( x<bfx || (x==bfx&&y<bfy) )//删除
        {
            textcopy->setSelection(x,y,bfx,bfy);
            textcopy->removeSelectedText();
        }
        else
        {
            select_change = false;

            textEdit->setSelection(bfx,bfy,x,y);
            QString str = textEdit->selectedText();
            QString after="";
            if(str=="(") after=")";
            if(str=="[") after="]";
            if(str=="{") after="}";
            if(str=="<") after=">";
            if(!after.isEmpty())
            {
                textEdit->append(after);
                textcopy->append(after);
            }


            if((s_bfx!=s_x || s_bfy!=s_y) && !isdelete)//说明发生覆盖
            {
                textcopy->setSelection(s_bfx,s_bfy,s_x,s_y);
                textcopy->removeSelectedText();
                textcopy->getSelection(&s_bfx,&s_bfy,&s_x,&s_y);
            }
            textcopy->insertAt(str,bfx,bfy);
            textEdit->setCursorPosition(x,y);
            select_change = true;
            isdelete = false;
            textEdit->getSelection(&s_bfx,&s_bfy,&s_x,&s_y);
//            qDebug()<<"添加成功";
        }
    }
    bfx = x;//记录上一次坐标
    bfy = y;
    ischanged = false;//文本变动解除
}

void MainWindow::selection_change()
{
    if(!ischanged)//文本不变动时记录选区
    {
        textEdit->getSelection(&s_bfx,&s_bfy,&s_x,&s_y);
//        qDebug()<<"s_bfx:"<<s_bfx<<", s_bfy"<<s_bfy<<", s_x"<<s_x<<", s_y"<<s_y;
    }
    if(select_change && ischanged && (s_bfx!=s_x || s_bfy!=s_y))//发生选中删除操作
    {
         textcopy->setSelection(s_bfx,s_bfy,s_x,s_y);
         textcopy->removeSelectedText();
//         qDebug()<<"选中删除";
         ischanged = false; //文本变动解除
         isdelete = true;   //是否发生删除
    }
}
void MainWindow::closeEvent(QCloseEvent *event)
{
    if(textEdit->isEnabled())
        emit WindowClose();
}

#endif // BASICOPERATION_H
