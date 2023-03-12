#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    testDataIsSame("1", "2");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::testDataIsSame(QString value1, QString value2) {
    TestData tq;
    //这句是执行测试的最重要的地方
    QTest::qExec(&tq);
    //用户测试一个方法或者函数执行的时间
}
