#ifndef FIND_DIALOG_H
#define FIND_DIALOG_H

#include <QDialog>

namespace Ui {
class find_dialog;
}

class find_dialog : public QDialog
{
    Q_OBJECT

public:
    explicit find_dialog(QWidget *parent = 0,int t=0);
    QString find_text;
    QString replace_text;
    bool distinguish;   //区分大小写，true区分，false不区分
    ~find_dialog();
signals:
    void start_search();
    void start_replace();
    void start_replace_all();

private slots:
    void on_find_next_button_clicked();

    void on_replace_button_clicked();

    void on_replace_all_button_clicked();

private:
    Ui::find_dialog *ui;
    int tab;
};

#endif // FIND_DIALOG_H
