#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QSqlQueryModel>
#include <QTableView>
#include <QMessageBox>

#include <QDebug>

#include "mysqlquerymodel.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    model = new QSqlTableModel(this);
    model->setTable("student");
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);

    //不显示第二列
    //model->removeColumn(1);

    //查询整张表
    model->select();

    ui->tableView->setModel(model);

    //设置视图不可编辑
    //ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

MainWindow::~MainWindow()
{
    delete ui;
}

//void MainWindow::on_pushButton_clicked()
//{
//    QSqlQueryModel *model = new QSqlQueryModel(this);

//    model->setQuery("select * from student");
//    model->setHeaderData(0, Qt::Horizontal, tr("ID"));
//    model->setHeaderData(1, Qt::Horizontal, tr("Name"));

//    QTableView *view = new QTableView;

//    view->setModel(model);

//    view->show();

//    //创建自己的模型

//    MySqlQueryModel *model1 = new MySqlQueryModel(this);

//    model1->setQuery("select * from student");
//    model1->setHeaderData(0, Qt::Horizontal, tr("ID"));
//    model1->setHeaderData(1, Qt::Horizontal, tr("Name"));

//    QTableView *view1 = new QTableView;

//    view1->setModel(model1);

//    view1->show();

//}



//提交修改
void MainWindow::on_pushButton_clicked()
{
    //开启事务
    model->database().transaction();
    if (model->submitAll())
    {
        //提交事务
        model->database().commit();
    }
    else
    {
        //回滚事务
        model->database().rollback();
        QMessageBox::warning(this, tr("TableView"), tr("数据库错误: %1").arg(model->lastError().text()));
    }
}

//撤销修改
void MainWindow::on_pushButton_2_clicked()
{
    model->revertAll();
}

//查询按钮
void MainWindow::on_pushButton_5_clicked()
{
    QString name = ui->lineEdit->text();

    //根据姓名进行筛选  单引号不能少
    model->setFilter(QString("name = '%1'").arg(name));

    //显示结果
    model->select();
}

//显示全表按钮槽函数
void MainWindow::on_pushButton_6_clicked()
{
    //设置要查询的表
    model->setTable("student");
    //执行查询操作
    model->select();
}

//根据ID进行升序排序操作
void MainWindow::on_pushButton_7_clicked()
{
    model->setSort(0, Qt::AscendingOrder);

    model->select();
}

//根据ID进行降序排序操作
void MainWindow::on_pushButton_8_clicked()
{
    model->setSort(0, Qt::DescendingOrder);

    model->select();
}

//删除选中行
void MainWindow::on_pushButton_4_clicked()
{
    //获取选中的行
    int curRow = ui->tableView->currentIndex().row();

    //删除该行
    model->removeRow(curRow);

    int ret = QMessageBox::warning(this, tr("删除当前行"), tr("你确定要删除该行吗"), QMessageBox::Yes | QMessageBox::No);
    if (ret == QMessageBox::Yes)
    {
        //删除该行 提交到数据库
        model->submitAll();
    }
    else
    {
        //不删除  撤销操作
        model->revertAll();
    }
}


//添加记录
void MainWindow::on_pushButton_3_clicked()
{
    //获取表的行数
    int row = model->rowCount();

    int id = 18;
    //添加一行
    model->insertRow(row);

    model->setData(model->index(row, 0), id);
    model->setData(model->index(row, 1), tr("李华"));

    //提交
    //model->submitAll();
}
