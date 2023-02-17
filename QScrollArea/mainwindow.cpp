#include "mainwindow.h"
#include <QGridLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QScrollArea>
#include <QLabel>
#include <QDebug>

typedef void (MainWindow::*pFuns)(void);

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{

    this->setWindowTitle("ocean");
    //this->resize(1931,1087);
    int rsz=800;
    this->resize(rsz,1087.0*rsz/1931.0);


    QScrollArea * scrollArea = new QScrollArea(this);
    QWidget * pWgt = new QWidget;

    QHBoxLayout *pLayout = new QHBoxLayout();//网格布局
    QPushButton *pBtn[100];
    for(int i = 0; i < 100; i++)
    {
        pBtn[i] = new QPushButton();
        pBtn[i]->setText(QString("按钮%1").arg(i));
        pBtn[i]->setMinimumSize(QSize(60,30));  //width height
        pLayout->addWidget(pBtn[i]);//把按钮添加到布局控件中
    }
    pWgt->setLayout(pLayout);

    //这一句setWidget必须放在pWgt里面的内容都准备完毕之后，否则显示有问题
    scrollArea->setWidget(pWgt);
    setCentralWidget(scrollArea);

    pFuns testFuns[2]={&MainWindow::test0, &MainWindow::test1};

    for(int i=0; i<2; i++)
        connect(pBtn[i], &QPushButton::clicked, this, testFuns[i]);


}


void MainWindow::test0()
{
    qDebug() << "test0";
}

void MainWindow::test1()
{
    qDebug() << "test1";
}

MainWindow::~MainWindow()
{

}






