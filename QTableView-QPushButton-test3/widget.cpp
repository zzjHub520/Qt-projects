#include "widget.h"
#include "./ui_widget.h"
#include <QtDebug>
#include <QModelIndex>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    CreateView();
}

Widget::~Widget()
{
    delete ui;
}


void Widget::CreateView()
{
    mainLayout = new QVBoxLayout;       //垂直布局
    mainLayout->setSpacing(10);         //设置控件间距
    mainLayout->setMargin(10);          //设置边缘间距
    testBtn = new QPushButton("Test");
    m_PushButton = new QPushButton("PushButton");

    //添加QTableView代码
    tableView = new QTableView;
    standItemModel = new QStandardItemModel();

    //添加表头
    standItemModel->setColumnCount(3);
    standItemModel->setHeaderData(0,Qt::Horizontal,QStringLiteral("序号"));   //设置表头内容
    standItemModel->setHeaderData(1,Qt::Horizontal,QStringLiteral("名称"));
    standItemModel->setHeaderData(2,Qt::Horizontal,QStringLiteral("地址"));

    //向表格添加100行内容
    for(int i=0;i<100;i++)
    {
        QStandardItem *standItem1 = new QStandardItem(tr("%1").arg(i+1));
        QStandardItem *standItem2 = new QStandardItem(tr("第%1行").arg(i+1));
        standItemModel->setItem(i,0,standItem1);                                //表格第i行，第0列添加一项内容
        standItemModel->item(i,0)->setForeground(QBrush(QColor(255,0,0)));      //设置字符颜色
        standItemModel->item(i,0)->setTextAlignment(Qt::AlignCenter);           //设置表格内容居中
        standItemModel->setItem(i,1,standItem2);                                //表格第i行，第1列添加一项内容
    }

    tableView->setModel(standItemModel);    //挂载表格模型

    //设置表格属性
    tableView->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);        //表头信息显示居中
    tableView->horizontalHeader()->setSectionResizeMode(0,QHeaderView::Fixed);  //设定表头列宽不可变
    tableView->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Fixed);
    tableView->horizontalHeader()->setSectionResizeMode(2,QHeaderView::Stretch);//设定第2列表头弹性拉伸

    tableView->setColumnWidth(0,100);       //设定表格第0列宽度
    tableView->setColumnWidth(1,200);

    tableView->verticalHeader()->hide();    //隐藏默认显示的行头
    tableView->setSelectionBehavior(QAbstractItemView::SelectRows); //设置选中时整行选中
    tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);  //设置表格属性只读，不能编辑
//    tableView->setContextMenuPolicy(Qt::CustomContextMenu);         //需要在表格使用右键菜单，需要启动该属性
//    tableView->sortByColumn(0,Qt::AscendingOrder);                 //表格第0列，按降序排列

    tableView->setIndexWidget(standItemModel->index(10,1),m_PushButton);    //向表格单元添加一个控件
    tableView->setIndexWidget(standItemModel->index(10,2),testBtn);         //向表格单元添加一个控件
    tableView->setIndexWidget(standItemModel->index(11,2),new QPushButton("Test1"));         //向表格单元添加一个控件
    tableView->setShowGrid(false);

//  设置单元信息
//    int row = 0;
//    int column = 0;
//    QModelIndex index = tableView->model()->index(row,column);
//    tableView->model()->setData(index,"输入你想输入的内容");

//  读取单元信息
    QModelIndex  index = tableView->indexAt(QPoint(39, 1));
    int row = index.row();
   int column = index.column();
   qDebug() << "row:" << row << "column:" << column;
    //根据行列号获取指定内容
   qDebug() << "data:" <<tableView->model()->index(2,column ).data().toString();



//    standItemModel->removeRows(0,10);                               //删除从第0行开始的连续10行

//    mainLayout->addWidget(testBtn);
    mainLayout->addWidget(tableView);    //添加控件
    this->setLayout(mainLayout);        //显示垂直布局
}




















