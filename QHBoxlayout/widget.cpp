#include "widget.h"
#include <QPushButton>
#include <QHBoxLayout>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    this->setWindowTitle("ocean");
    //this->resize(1931,1087);
    int rsz=1800;
    this->resize(rsz,1087.0*rsz/1931.0);

    /* ========================================== */

    QHBoxLayout* pLayout = new QHBoxLayout();//水平布局

    QPushButton* p1 = new QPushButton("p1");
    QPushButton* p2 = new QPushButton("p2");
    QPushButton* p3 = new QPushButton("p3");
    QPushButton* p4 = new QPushButton("p4");

    pLayout->addWidget(p1);

    pLayout->addWidget(p2);

    pLayout->addWidget(p3);

    pLayout->addWidget(p4);

    pLayout->setStretchFactor(p1,1);//设置p1的拉伸系数为1

    pLayout->setStretchFactor(p2,2);//设置p1的拉伸系数为2

    this->setLayout(pLayout);

}

Widget::~Widget()
{

}
