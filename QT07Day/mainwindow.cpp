#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QLabel>
#include <QMovie>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->actionopen->setIcon(QIcon(":/icon/iconImg/1.png"));
    ui->toolButton_1->setStyleSheet("border:none;");
    ui->toolButton_2->setStyleSheet("border:none;");
    ui->toolButton_3->setStyleSheet("border:none;");
    ui->toolButton_4->setStyleSheet("border:none;");
    ui->toolButton_5->setStyleSheet("border:none;");
    ui->toolButton_6->setStyleSheet("border:none;");

    QMovie *movie=new QMovie("C:/Users/20200062/Documents/Qt_Programme/Qt_test/QT07Day/iconImg/66.gif");
    ui->label->setMovie(movie);
    movie->start();

//    movie->setPaused(true);
//    movie->stop();

    //ui->label->hide();

//    ui->pushButton->setIcon(QIcon(":/icon/iconImg/1.png"));

//    ui->toolButton_1->


}

MainWindow::~MainWindow()
{
    delete ui;
}
