#include "widget.h"
#include "./ui_widget.h"
#include <QFile>

Widget::Widget(QWidget *parent)
        : QWidget(parent), ui(new Ui::Widget) {
    ui->setupUi(this);
    QFile styleSheet(":/stylesheet.css");
    if (styleSheet.open(QIODevice::ReadOnly)) {
        setStyleSheet(QLatin1String(styleSheet.readAll()));
        styleSheet.close();
    }

    ui->comboBox->addItem("select1");
    ui->comboBox->addItem("select2");
    ui->comboBox->addItem("select3");
    ui->comboBox->addItem("select4");
    ui->comboBox->addItem("select5");
    ui->comboBox->addItem("select6");
    ui->comboBox->addItem("select7");
    ui->comboBox->addItem("select8");
}

Widget::~Widget() {
    delete ui;
}
