#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QtDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL");
    db.setHostName("127.0.0.1");
    db.setDatabaseName("postgres");
    db.setPort(5432);
    db.setUserName("postgres");
    db.setPassword("123456");
    bool ok = db.open();

    qDebug() << ok;
    QSqlQuery query;
    query.exec("Select * from \"Pipes\"");
    while (query.next())
    {
        QString id = query.value(1).toString();
        qDebug() << id;
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

