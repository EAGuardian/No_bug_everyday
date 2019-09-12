#ifndef FILEDEAL_H
#define FILEDEAL_H
#include "mainwindow.h"
#include "ui_mainwindow.h"
//新建文件函数
void MainWindow::NewFile(){
//    QString path="Unname"+QString::number(newfile++)+".c";   //临时创建新文件，路径在当前目录下
    if(textEdit->isEnabled())
    {
        if(myfile.at(currentfile).isnew==true)
        {
            int ret = QMessageBox::warning(this, tr("提示"),
                         tr("您新建的文件尚未保存，要保存文件吗？\n"),
                         QMessageBox::Yes | QMessageBox::Default,
                         QMessageBox::No,
                         QMessageBox::Cancel | QMessageBox::Escape);
            if (ret == QMessageBox::Yes)
            {
                if(!Save_asFile()) return ;
            }
            else if(ret == QMessageBox::No)
            {
                myfile.remove(currentfile);
                newfile--;
                ui->treeWidget->takeTopLevelItem(currentfile);
            }
            else
            {
                return ;
            }
        }
        else if(myfile.at(currentfile).issave==false)
        {
            int ret = QMessageBox::warning(this, tr("提示"),
                         tr("您之前的文件尚未保存，要保存文件吗？\n"),
                         QMessageBox::Yes | QMessageBox::Default,
                         QMessageBox::No,
                         QMessageBox::Cancel | QMessageBox::Escape);
            if (ret == QMessageBox::Yes)
            {
               SaveFile();
            }
            else if(ret == QMessageBox::Cancel||ret == QMessageBox::Escape)
            {
                return ;
            }
        }
    }
    textEdit->setEnabled(true);      //将文本编辑器设置为可用
    SetifEnabled();
    textEdit->clear();         //清空编辑区
    textcopy->clear();         //清空编辑区

    //设置新文件的基本属性
    MyFile temp;
    temp.no = myfile.count()+1;
    temp.name="Unnamed"+QString::number(newfile++)+".c";
    temp.Path = "";           //类成员存储文件路径
    temp.File=0; //生成QFile类文件
    temp.Info=0;
    temp.isnew=true;
    temp.issave=false;
    addFileList(temp);
    myfile.append(temp);
    currentfile=temp.no-1;
    qDebug()<<"新建文件成功";
    qDebug() <<"currentfile="<<currentfile;
}

//打开文件函数
void MainWindow::OpenFile()
{
    //判断是否有文件打开
    if(textEdit->isEnabled())
    {
        //先判断上一个文件是否保存
        if(myfile.at(currentfile).isnew==true)   //判断是不是新文件
        {
            int ret = QMessageBox::warning(this, tr("提示"),
                         tr("您新建的文件尚未保存，要保存文件吗？\n"),
                         QMessageBox::Yes | QMessageBox::Default,
                         QMessageBox::No,
                         QMessageBox::Cancel | QMessageBox::Escape);
            if (ret == QMessageBox::Yes)
            {
                if(!Save_asFile()) return ;
            }
            else if(ret == QMessageBox::No)
            {
                myfile.remove(currentfile);
                newfile--;
                ui->treeWidget->takeTopLevelItem(currentfile);
            }
            else
            {
                return ;
            }
        }
        else if(myfile.at(currentfile).issave==false)
        {
            SaveFile();
        }
    }
    //选择打开的文件，path保存该文件路径
    QString path=QFileDialog::getOpenFileName(this, tr("打开文件"),"",tr("Text or C files (*.txt *.c *.cpp *.h);; " "All Files(*)"));
    if(!path.count()){
        qDebug()<<"用户取消";
        return ;
    }
    qDebug() << path;
    textEdit->setEnabled(true);   //将文本编辑器设置为可用
    SetifEnabled();
    MyFile temp;   //定义一个临时的MyFile类temp文件
    temp.no = myfile.count()+1;   //因为还未将temp临时文件加入到文件数组中，所以该文件的编号是当前打开文件数量+1
    temp.Path = path;           //存储文件路径
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");   //文件编码方式
    temp.File=new QFile(path); //生成QFile类文件
    temp.Info=new QFileInfo(path);  //存储文件信息
    temp.name=temp.Info->fileName();
    temp.File->open(QIODevice::ReadWrite);   //以读写方式打开
    temp.Array= temp.File->readAll();  //读写文件信息存入QByteArray类
    temp.issave=true;
    temp.isnew=false;


    isopenfile = true;
    textEdit->setText(codec->toUnicode(temp.Array));  //在编辑窗口显示
    textEdit->setCursorPosition(0,1);
    textcopy->setText(textEdit->text()); //复制文本到后台文本框
    textEdit->setCursorPosition(0,0);
    isopenfile =false;

    myfile.append(temp);   //将temp加入到打开文件的数组中
    addFileList(temp);  //添加到文件列表
    temp.File->close();  //关闭临时文件
    currentfile=myfile.count()-1;  //当前打开的文件编号是文件数组的文件数-1(因为下标从0开始)
}
//保存文件函数
bool MainWindow::SaveFile(){
    if(myfile.at(currentfile).isnew)
    {
       Save_asFile();
    }
    QFile file;
    qDebug() << myfile.at(currentfile).Path;
    file.setFileName(myfile.at(currentfile).Path); //存入打开的文件
    file.open(QIODevice::WriteOnly); //读写方式打开
    QString str=textEdit->text(); //读取编辑区文字
    file.write(str.toUtf8()); //以UTF-8编码方式写入文件
    qDebug()<<"save succeed!";

    //设置文件已保存标志
    MyFile temp=myfile.at(currentfile);
    temp.issave=true;
    myfile.replace(currentfile,temp);

    file.close(); //关闭临时文件
    return true;
}
//另存为文件函数
bool MainWindow::Save_asFile()   {
    QString path = QFileDialog::getSaveFileName(this, tr("保存文件"),"./home",tr("C files (*.c );;Text (*.txt)"));
    if(!path.count()){
        qDebug()<<"用户取消";
        return false;
    }
    QFile file;
    file.setFileName(path); //存入打开的文件
    file.open(QIODevice::WriteOnly); //以写方式打开
    QString str=textEdit->text(); //读取编辑区文字
    file.write(str.toUtf8()); //以UTF-8编码方式写入文件
    qDebug()<<"另存为文件成功";
    //判断是否是新文件，更改参数
    if(myfile.at(currentfile).isnew)
    {
        QFileInfo *info = new QFileInfo(path);
        MyFile temp=myfile.at(currentfile);
        temp.name=info->fileName();
        temp.File=new QFile(path);
        temp.Info=info;
        temp.Path=path;
        temp.Array=temp.File->readAll();
        temp.isnew=false;
        temp.issave=true;
        myfile.replace(currentfile,temp);
        QTreeWidgetItem *newfile=new QTreeWidgetItem();
        newfile->setText(0,temp.name);
        newfile->setText(1,temp.Path);
        newfile->setText(2,QString::number(temp.no));
        ui->treeWidget->insertTopLevelItem(temp.no-1,newfile);
        ui->treeWidget->takeTopLevelItem(temp.no);
        qDebug() << "currentfile="<<currentfile;
    }
    file.close(); //关闭文件
    return true;
}
//关闭文件函数
void MainWindow::CloseChooseFile(){
    if(CurrentChoose==currentfile)
    {
        CloseCurrenteFile();
        return;
    }
    if(CurrentChoose<currentfile) currentfile--;  //如果选择的文件在当前文件之前，currentfile需要自减
    ui->treeWidget->takeTopLevelItem(CurrentChoose);
    myfile.remove(CurrentChoose);

    for(int i=CurrentChoose;i<ui->treeWidget->topLevelItemCount();i++)
    {
        QString No=ui->treeWidget->topLevelItem(i)->text(2);
        int no=No.toInt()-1;
        ui->treeWidget->topLevelItem(i)->setText(2,QString::number(no));
    }
}
void MainWindow::CloseCurrenteFile(){
    if(currentfile>=myfile.size())
    {
        return ;
    }
    //判断是不是新文件，若是,提醒用户是否保存文件
    if(myfile.at(currentfile).isnew)
    {
        int ret = QMessageBox::warning(this, tr("提示"),
                     tr("改新建的文件尚未保存，要保存文件吗？\n"),
                     QMessageBox::Yes | QMessageBox::Default,
                     QMessageBox::No,
                     QMessageBox::Cancel | QMessageBox::Escape);
        if (ret == QMessageBox::Yes)
        {
            if(!Save_asFile()) return ;
            textEdit->clear();
            textEdit->setEnabled(false);
            SetifEnabled();
            return ;
        }
        else if(ret == QMessageBox::No)
        {
            myfile.remove(currentfile);
            newfile--;
            ui->treeWidget->takeTopLevelItem(currentfile);
            textEdit->clear();
            textEdit->setEnabled(false);
            SetifEnabled();
            return ;
        }
        else
        {
            return ;
        }
    }
    //判断文本信息是否有变动，若有，提醒用户是否保存文件
    if(textEdit->isModified())
    {
        int ret = QMessageBox::warning(this, tr("提示"),
                     tr("检测到文本有变动，要保存文件吗？\n"),
                     QMessageBox::Yes | QMessageBox::Default,
                     QMessageBox::No,
                     QMessageBox::Cancel | QMessageBox::Escape);
        if (ret == QMessageBox::Yes)
        {
            SaveFile();
            textEdit->clear();
        }
        else if(ret == QMessageBox::No)      
            textEdit->clear();     
        else if(ret == QMessageBox::Cancel)
            return ;
    }
    ui->treeWidget->takeTopLevelItem(currentfile);
    myfile.remove(currentfile);
    for(int i=currentfile;i<ui->treeWidget->topLevelItemCount();i++)
    {
        QString No=ui->treeWidget->topLevelItem(i)->text(2);
        int no=No.toInt()-1;
        ui->treeWidget->topLevelItem(i)->setText(2,QString::number(no));
    }
    textEdit->setEnabled(false);
    SetifEnabled();
}
//增加文件至窗口树
void MainWindow::addFileList(MyFile File){
    QTreeWidgetItem *newfile=new QTreeWidgetItem();
    newfile->setText(0,File.name);
    newfile->setText(1,File.Path);
    newfile->setText(2,QString::number(File.no));
    qDebug() << File.no <<File.name;
    ui->treeWidget->addTopLevelItem(newfile);
}
//点击切换文件
void MainWindow::TreeWidgetClick(QTreeWidgetItem *item,int column){
    //也先判断是不是新文件
    bool willsave=true;
    if(item->text(2).toInt()-1==currentfile) return ;
    if(currentfile>=myfile.size())
    {
        textEdit->setEnabled(true);
        SetifEnabled();
        QFile CurrentFile(item->text(1));
        currentfile=item->text(2).toInt()-1;
        CurrentFile.open(QIODevice::ReadWrite);
        QByteArray array= CurrentFile.readAll();
        QTextCodec *codec = QTextCodec::codecForName("UTF-8");   //文件编码方式
        isopenfile = true;
        textEdit->setText(codec->toUnicode(array));
        textEdit->setCursorPosition(0,1);
        textcopy->setText(textEdit->text()); //复制文本到后台文本框
        textEdit->setCursorPosition(0,0);
        isopenfile =false;
        CurrentFile.close();
    }
    if(myfile.at(currentfile).isnew==true)
    {
        int ret = QMessageBox::warning(this, tr("提示"),
                     tr("您新建的文件尚未保存，要保存文件吗？\n"),
                     QMessageBox::Yes | QMessageBox::Default,
                     QMessageBox::No,
                     QMessageBox::Cancel | QMessageBox::Escape);
        if (ret == QMessageBox::Yes)
        {
            if(!Save_asFile()) return ;
        }
        else if(ret == QMessageBox::No)
        {
            myfile.remove(currentfile);
            newfile--;
            ui->treeWidget->takeTopLevelItem(currentfile);
            willsave=false;
        }
        else
        {
            return ;
        }
    }
    //不是新文件，切换时保存
    if(textEdit->isEnabled()&&willsave)
        SaveFile();
    if(item->text(1)==""){return;}       //点击的文件路径为空，返回
    textEdit->setEnabled(true);
    SetifEnabled();
    QFile CurrentFile(item->text(1));
    currentfile=item->text(2).toInt()-1;
    CurrentFile.open(QIODevice::ReadWrite);
    QByteArray array= CurrentFile.readAll();
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");   //文件编码方式
    isopenfile = true;
    textEdit->setText(codec->toUnicode(array));
    textEdit->setCursorPosition(0,1);
    textcopy->setText(textEdit->text()); //复制文本到后台文本框
    textEdit->setCursorPosition(0,0);
    isopenfile =false;
    CurrentFile.close();
}
#endif // FILEDEAL_H
