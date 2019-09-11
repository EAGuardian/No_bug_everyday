#ifndef FILEDEAL_H
#define FILEDEAL_H
#include "mainwindow.h"
#include "ui_mainwindow.h"
//新建文件函数
void MainWindow::NewFile(){
    QString path="Unname"+QString::number(newfile++)+".c";   //临时创建新文件，路径在当前目录下
    if(!path.count()){
        qDebug()<<"用户取消";
        return ;
    }
    textEdit->setEnabled(true);      //将文本编辑器设置为可用
    QFile file;            //定义一个file文件类型
    file.setFileName(path); //存入打开的文件
    file.open(QIODevice::WriteOnly); //以写方式打开
    QString str="";
    file.write(str.toUtf8()); //以UTF-8编码方式写入文件
    file.close(); //关闭文件
    textEdit->setEnabled(true);
    ui->ResultWidget->setEnabled(true);
    MyFile temp;
    temp.no = myfile.count()+1;
    temp.Path = path;           //类成员存储文件路径
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");   //文件编码方式
    temp.File=new QFile(path); //生成QFile类文件
    temp.Info=new QFileInfo(path);
    temp.File->open(QIODevice::ReadWrite);   //以读写方式打开
    temp.Array= temp.File->readAll();  //读写文件信息存入QByteArray类
    temp.isnew=true;

    isopenfile = true;
    textEdit->setText(codec->toUnicode(temp.Array));  //在编辑窗口显示
    textEdit->setCursorPosition(0,1);
    textcopy->setText(textEdit->text()); //复制文本到后台文本框
    textEdit->setCursorPosition(0,0);
    isopenfile =false;

    myfile.append(temp);
    addFileList(temp);
    temp.File->close();  //关闭文件
    currentfile=myfile.count()-1;
    qDebug()<<"新建文件成功";
    qDebug() <<"currentfile="<<currentfile;
}

//打开文件函数
void MainWindow::OpenFile()
{
    //选择打开的文件，path保存该文件路径
    QString path=QFileDialog::getOpenFileName(this, tr("打开文件"),"",tr("Text or C files (*.txt *.c *.cpp *.h);; " "All Files(*)"));
    if(!path.count()){
        qDebug()<<"用户取消";
        return ;
    }
    qDebug() << path;
    textEdit->setEnabled(true);   //将文本编辑器设置为可用
    ui->ResultWidget->setEnabled(true); //将结果输出器设置为可用
    MyFile temp;   //定义一个临时的MyFile类temp文件
    temp.no = myfile.count()+1;   //因为还未将temp临时文件加入到文件数组中，所以该文件的编号是当前打开文件数量+1
    temp.Path = path;           //存储文件路径
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");   //文件编码方式
    temp.File=new QFile(path); //生成QFile类文件
    temp.Info=new QFileInfo(path);  //存储文件信息
    temp.File->open(QIODevice::ReadWrite);   //以读写方式打开
    temp.Array= temp.File->readAll();  //读写文件信息存入QByteArray类
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
    //判断是否有文件打开，没有，错误警告
    if(!textEdit->isEnabled())
    {
        QMessageBox::warning(this, tr("提示"),
                            tr("你还没有打开任何文件"),
                            QMessageBox::Yes | QMessageBox::Default);
        return false;
    }
    if(myfile.at(currentfile).isnew)
    {
       Save_asFile();
       MyFile temp=myfile.at(currentfile);
       temp.isnew=false;
       myfile.replace(currentfile,temp);
    }
    QFile file;
    qDebug() << myfile.at(currentfile).Path;
    file.setFileName(myfile.at(currentfile).Path); //存入打开的文件
    file.open(QIODevice::WriteOnly); //读写方式打开
    QString str=textEdit->text(); //读取编辑区文字
    file.write(str.toUtf8()); //以UTF-8编码方式写入文件
    qDebug()<<"save succeed!";
    file.close(); //关闭文件
}
//另存为文件函数
void MainWindow::Save_asFile()   {
    if(!textEdit->isEnabled())
    {
        QMessageBox::warning(this, tr("提示"),
                            tr("你还没有打开任何文件"),
                            QMessageBox::Yes | QMessageBox::Default);
        return ;
    }
    QString path = QFileDialog::getSaveFileName(this, tr("保存文件"),"./home",tr("C files (*.c );;C++ files (*.cpp );;Text (*.txt)"));
    if(!path.count()){
        qDebug()<<"用户取消";
        return ;
    }
    QFile file;
    file.setFileName(path); //存入打开的文件
    file.open(QIODevice::WriteOnly); //以写方式打开
    QString str=textEdit->text(); //读取编辑区文字
    file.write(str.toUtf8()); //以UTF-8编码方式写入文件
    qDebug()<<"另存为文件成功";
    file.close(); //关闭文件
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
    if(!textEdit->isEnabled())
    {
        QMessageBox::warning(this, tr("提示"),
                            tr("你还没有打开任何文件"),
                            QMessageBox::Yes | QMessageBox::Default);
        return ;
    }
    if(currentfile>=myfile.size())
    {
        return ;
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
}
//增加文件至窗口树
void MainWindow::addFileList(MyFile File){
    QTreeWidgetItem *newfile=new QTreeWidgetItem();
    newfile->setText(0,File.Info->fileName());
    newfile->setText(1,File.Path);
    newfile->setText(2,QString::number(File.no));
    qDebug() << File.no <<File.Info->fileName();
    ui->treeWidget->addTopLevelItem(newfile);
}
//点击切换文件
void MainWindow::TreeWidgetClick(QTreeWidgetItem *item,int column){
    if(textEdit->isEnabled())
            SaveFile();             //切换文件时需要保存
    if(item->text(1)==""){return;}       //点击的文件路径为空，返回
    textEdit->setEnabled(true);
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
//文件信息是否改动
#endif // FILEDEAL_H
