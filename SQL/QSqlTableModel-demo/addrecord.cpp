#include "addrecord.h"
#include "ui_addrecord.h"

AddRecord::AddRecord(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddRecord)
{
    ui->setupUi(this);
    setWindowTitle("插入");
}

AddRecord::~AddRecord()
{
    delete ui;
}

void AddRecord::on_pushButton_clicked()
{
    isClicked = true;
    str[0] = ui->lineEdit->text();
    str[1] = ui->lineEdit_2->text();
    str[2] = ui->lineEdit_3->text();
    close();
}

