#include "widget.h"
#include "./ui_widget.h"
#include <QPushButton>
#include <QFile>
#include <QDebug>

Widget::Widget(QWidget *parent)
        : QWidget(parent), ui(new Ui::Widget) {
    ui->setupUi(this);

    SetStyleSheet();

    ui->pushButton_2->setProperty(STATUS_NAME.toStdString().c_str(), QVariant::fromValue(COLOR_RED));
    ui->pushButton_3->setProperty(STATUS_NAME.toStdString().c_str(), QVariant::fromValue(COLOR_BLUE));

}

Widget::~Widget() {
    delete ui;
}

void Widget::SetStyleSheet(){
    QFile file(":/stylesheet.css");
    if (file.open(QIODevice::ReadOnly)) {
        setStyleSheet(QLatin1String(file.readAll()));
        file.close();
    }
}

void Widget::on_pushButton_clicked() {

}

void Widget::on_pushButton_2_clicked() {
    flag2 ^= 1;
    if (flag2) {
        ui->pushButton_5->setProperty(STATUS_NAME.toStdString().c_str(), QVariant::fromValue(COLOR_RED));
        SetStyleSheet();
        qDebug() << __FUNCTION__ << true;
    } else {
        ui->pushButton_5->setProperty(STATUS_NAME.toStdString().c_str(), QVariant::fromValue(nullptr));
        SetStyleSheet();
        qDebug() << __FUNCTION__ << false;
    }
}

void Widget::on_pushButton_3_clicked() {
    flag3 ^= 1;
    if (flag3) {
        ui->pushButton_6->setProperty(STATUS_NAME.toStdString().c_str(), QVariant::fromValue(COLOR_BLUE));
        SetStyleSheet();
        qDebug() << __FUNCTION__ << true;
    } else {
        ui->pushButton_6->setProperty(STATUS_NAME.toStdString().c_str(), QVariant::fromValue(nullptr));
        SetStyleSheet();
        qDebug() << __FUNCTION__ << false;
    }
}
