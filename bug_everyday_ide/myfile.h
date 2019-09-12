#ifndef MYFILE_H
#define MYFILE_H
#include <QFile>
#include <QFileInfo>
#include <QByteArray>
#include <QTextCodec>
#include <QString>
class MyFile{
public:
    MyFile(){}
    QString name;     //文件名
    QString Path;      //文件路径
    QFile *File;       //Q文件指针
    QFileInfo *Info;   //文件信息指针
    QByteArray Array;   //文件存储
    int no;
    bool isnew;
    bool issave;
};
#endif // MYFILE_H
