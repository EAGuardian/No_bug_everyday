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
    explicit find_dialog(QWidget *parent = 0);
    QString find_text;
    bool distinguish;   //区分大小写，true区分，false不区分
    ~find_dialog();
signals:
    void start_search();

private slots:
    void on_find_next_button_clicked();

private:
    Ui::find_dialog *ui;
};

#endif // FIND_DIALOG_H
