#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("bug_everyday");
    w.setWindowIcon(QIcon(" bug.ico"));
    w.show();

    return a.exec();
}
