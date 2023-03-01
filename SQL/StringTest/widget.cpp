#include "widget.h"
#include "ui_widget.h"
#include <QFileDialog>
#include <QDebug>
#include <QSqlRecord>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    ui->tableView->verticalHeader()->setHidden(true);

    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);

    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);

    ui->tableView->setAlternatingRowColors(true);

    this->resize(1000,600);

    ui->rBUp->setChecked(true);

    ui->rBAll->setChecked(true);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::initTableView()
{
    //数据表model
    m_model = new QSqlTableModel(this, m_db);

    //设置数据表
    m_model->setTable("students");

    //查询数据
    if(!m_model->select())
    {
        qDebug()<<"查询失败";
        return;
    }

    m_model->setHeaderData(m_model->fieldIndex("id"),Qt::Horizontal,"编号");
    m_model->setHeaderData(m_model->fieldIndex("name"),Qt::Horizontal,"姓名");
    m_model->setHeaderData(m_model->fieldIndex("sex"),Qt::Horizontal,"性别");
    m_model->setHeaderData(m_model->fieldIndex("age"),Qt::Horizontal,"年龄");
    m_model->setHeaderData(m_model->fieldIndex("addr"),Qt::Horizontal,"地址");
    m_model->setHeaderData(m_model->fieldIndex("height"),Qt::Horizontal,"身高");

    //设置模型
    ui->tableView->setModel(m_model);

    //设置性别代理
    ui->tableView->setItemDelegateForColumn(m_model->fieldIndex("sex"),&m_cbxDelegate);

    //设置年龄代理
    ui->tableView->setItemDelegateForColumn(m_model->fieldIndex("age"),&m_spinDelegate);

    //创建界面组件与数据模型的字段之间的数据映射
    m_dataMapper = new QDataWidgetMapper(this);

    //设置数据模型
    m_dataMapper->setModel(m_model);

    m_dataMapper->setSubmitPolicy(QDataWidgetMapper::AutoSubmit);

    //界面组件与tabModel的具体字段之间的联系
    m_dataMapper->addMapping(ui->spinBoxNum,m_model->fieldIndex("id"));
    m_dataMapper->addMapping(ui->lineEditName,m_model->fieldIndex("name"));
    m_dataMapper->addMapping(ui->cbxSex,m_model->fieldIndex("sex"));
    m_dataMapper->addMapping(ui->spinBoxAge,m_model->fieldIndex("age"));
    m_dataMapper->addMapping(ui->lineEditAddr,m_model->fieldIndex("addr"));
    m_dataMapper->addMapping(ui->doubleSpinBoxHeight,m_model->fieldIndex("height"));

    //移动到首记录
    m_dataMapper->toFirst();


    QSqlRecord record = m_model->record();
    for(int i=0;i<record.count();i++)
    {
        ui->cbxSort->addItem(record.fieldName(i));
    }
}


void Widget::on_btnOpen_clicked()
{
    QString file = QFileDialog::getOpenFileName(this,"选择数据库文件","",
                                               "SQLite数据库(*.db *.db3)");
    if(file.isEmpty())
        return;

    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(file);

    if(!m_db.open())
    {
        qDebug()<<"打开失败";
        return;
    }

    initTableView();
}

void Widget::on_rBMan_clicked()
{
    m_model->setFilter("sex='男'");
}

void Widget::on_rBWoman_clicked()
{
    m_model->setFilter("sex='女'");
}

void Widget::on_rBAll_clicked()
{
    m_model->setFilter("");
}

void Widget::on_rBUp_clicked()
{
    m_model->setSort(ui->cbxSort->currentIndex(),Qt::AscendingOrder);
    m_model->select();
}

void Widget::on_rBDown_clicked()
{
    m_model->setSort(ui->cbxSort->currentIndex(),Qt::DescendingOrder);
    m_model->select();
}

void Widget::on_cbxSort_currentIndexChanged(int index)
{
    if(ui->rBUp->isChecked())
    {
        m_model->setSort(index,Qt::AscendingOrder);
    }
    else
    {
        m_model->setSort(index,Qt::DescendingOrder);
    }
    m_model->select();
}

void Widget::on_tableView_clicked(const QModelIndex &index)
{
    m_dataMapper->setCurrentIndex(index.row());
}

void Widget::on_btnAdd_clicked()
{
    int rowCount = m_model->rowCount();
    m_model->insertRow(rowCount);

    QModelIndex curIndex = m_model->index(m_model->rowCount()-1,1);

    int curRow = curIndex.row();
    //QModelIndex curIndex = m_model->index(m_model->rowCount()-1,1);
    m_model->setData(m_model->index(curRow,0),rowCount+1);
    m_model->setData(m_model->index(curRow,1),"马超");
    m_model->setData(m_model->index(curRow,2),"男");
    m_model->setData(m_model->index(curRow,3),27);
    m_model->setData(m_model->index(curRow,4),"蜀国");
    m_model->setData(m_model->index(curRow,5),1.76);
}

void Widget::on_btnDel_clicked()
{
    QModelIndex curIndex = ui->tableView->currentIndex();
    m_model->removeRow(curIndex.row());
}

void Widget::on_btnCancel_clicked()
{
    m_model->revertAll();
}

void Widget::on_btnSave_clicked()
{
    bool bRet = m_model->submitAll();
}
