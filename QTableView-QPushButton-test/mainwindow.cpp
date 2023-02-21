#include <QTableView>
#include <QHeaderView>
#include <QStandardItemModel>
#include <QStringList>
#include <QString>
#include <QPushButton>
#include <QDebug>
#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setFixedSize(700, 500);

    // add tableview
    QTableView *tableView = new QTableView(this);
    tableView->setMinimumSize(700, 500);
    tableView->verticalHeader()->hide(); // hide row number

    QStandardItemModel *tableModel = new QStandardItemModel(this);
    tableView->setModel(tableModel); // recommend to set model before detail settings

    // set columns
    QStringList columnTitles;
    columnTitles << "id" << "name" << "status" << "action";
    tableModel->setHorizontalHeaderLabels(columnTitles);
    //    tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents); // column fit the contents

    // add contents
    for(int i = 0; i < 7; i++)
    {
        tableModel->setItem(i, 0, new QStandardItem(QString::number(i + 1)));
        tableModel->setItem(i, 1, new QStandardItem(QString("hello qt tablview %1").arg(i)));
        tableModel->setItem(i, 2, new QStandardItem("normal"));

        // add button to the last column
        QPushButton *button = new QPushButton("Start");

        // set custom property
        button->setProperty("id", i); // set custom property
        button->setProperty("name", QString("hello qt tablview %1").arg(i));
        button->setProperty("status", "normal");

        // set click event
        connect(button, SIGNAL(clicked()), this, SLOT(onTableBtnClicked()));

        // notice every time insert the button at the last line
        tableView->setIndexWidget(tableModel->index(tableModel->rowCount() - 1, 3), button);
    }
}

void MainWindow::onTableBtnClicked()
{
    QPushButton *button = (QPushButton *)sender();
    qDebug() << button->property("id").toInt() << endl;
    qDebug() << button->property("name").toString() << endl;
    qDebug() << button->property("status").toString() << endl;
}

MainWindow::~MainWindow()
{
    delete ui;
}

