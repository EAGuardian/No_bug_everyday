#include "find_dialog.h"
#include "ui_find_dialog.h"

find_dialog::find_dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::find_dialog)
{
    ui->setupUi(this);
}

find_dialog::~find_dialog()
{
    delete ui;
}

void find_dialog::on_find_next_button_clicked()//向后搜索
{
    find_text = ui->find_lineedit->text();
    distinguish = ui->distinguish_check->isChecked();//区分大小写，true区分，false不区分
    emit start_search();
}

void find_dialog::on_replace_button_clicked()
{
    replace_text = ui->replace_lineedit->text();
    find_text = ui->replace_find_lineedit->text();
    emit start_replace();
}

void find_dialog::on_replace_all_button_clicked()
{
    replace_text = ui->replace_lineedit->text();
}
