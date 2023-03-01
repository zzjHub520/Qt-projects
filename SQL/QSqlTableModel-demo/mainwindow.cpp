#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle(tr("测试"));
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("TEST");
    if(!db.open())
    {
        QMessageBox::warning(this,"wrong",db.lastError().text());
        return;
    }
    QSqlQuery query;
    query.exec("create table EXAMPLE("
                   "id integer primary key autoincrement,"
                   "name varchar,"
                   "age integer,"
                   "city varchar"
                   ")");//第一次运行时需要创建一个表，之后的运行可以删除

//    if(!query.exec("insert into EXAMPLE (name,age,city) values ('李四',21,'扬州')"))
//        QMessageBox::warning(this,"wrong",query.lastError().text());

    QSqlTableModel * model = new QSqlTableModel(this,db);
    model->setTable("EXAMPLE");
    ui->tableView->setModel(model);
    model->select();

    QStringList tables;
    tables << "id" << "姓名" << "年龄" << "城市";
    for(int i = 0 ; i < tables.count(); i++)
        model->setHeaderData(i,Qt::Horizontal,tables[i]);//设置显示框表头显示
    model->setSort(0,Qt::AscendingOrder);//设置按照第0列排序
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);//设置单元格不可编辑
    ui->tableView->horizontalHeader()->setStretchLastSection(true);//设置最后一列填充后面表格
    ui->tableView->setColumnHidden(0,true);//设置第0行隐藏
    ui->tableView->setColumnWidth(1,50);//设置列宽，界面看起来更舒适
    ui->tableView->setColumnWidth(2,30);//设置列宽，界面看起来更舒适
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    AddRecord * add = new AddRecord(this);
    add->exec();
    if(!add->isClicked)
        return;
    QString *str = add->str;
    QSqlTableModel * model = new QSqlTableModel(this,db);
    model->setTable("EXAMPLE");
    if(!ui->lineEdit->text().isEmpty())//搜索栏为空
        model->setFilter("city ='"+ui->lineEdit->text()+"'");//model过滤表中符合的数据
    int row = model->rowCount();
    model->insertRow(row);
    model->setData(model->index(row,1),str[0]);
    model->setData(model->index(row,2),str[1].toInt());
    model->setData(model->index(row,3),str[2]);
    model->submitAll();//提交插入的数据
    ui->tableView->setModel(model);//设置model模型
    model->select();//显示
}

void MainWindow::on_pushButton_2_clicked()
{
    int row = ui->tableView->currentIndex().row();//记录当前选择行
    if(QMessageBox::Yes == QMessageBox::question(this,"Make Sure","确定删除第"+QString::number(row+1)+"行吗？",QMessageBox::Yes | QMessageBox::No , QMessageBox::Yes))
    {//提示框，防止误操作，是/否，按下是的操作后
        QSqlTableModel * model = new QSqlTableModel(this,db);//新建model
        model->setTable("EXAMPLE");//设置表
        if(!ui->lineEdit->text().isEmpty())//搜索栏为空
            model->setFilter("city ='"+ui->lineEdit->text()+"'");//model过滤表中符合的数据
        if(model->select())//判断是否为空表
            model->removeRow(row);//删除当前行
        model->submitAll();//提交修改的数据
        ui->tableView->setModel(model);//设置模型
        model->select();//显示修改后的数据
    }
}

void MainWindow::on_tableView_doubleClicked(const QModelIndex &index)
{
    QSqlTableModel * model = new QSqlTableModel(this,db);//新建model
    model->setTable("EXAMPLE");//设置表
    if(!ui->lineEdit->text().isEmpty())//搜索栏为空
        model->setFilter("city ='"+ui->lineEdit->text()+"'");//model过滤表中符合的数据
    bool isOK;//定义bool类型
    QString text = QInputDialog::getText(this,"Update","修改当前内容",QLineEdit::Normal,index.data().toString(),&isOK);//弹出修改框，默认显示当前内容，修改后点击按钮，是则isOK为true，否则isOK为false
    if(!isOK)//判断isOKbool类型
        return ;
    if(model->select())//判断是否空表
        model->setData(model->index(index.row(),index.column()),text);//重新设置当前单元格内容
    model->submitAll();//提交修改
    ui->tableView->setModel(model);
    model->select();//显示修改后的数据
}

void MainWindow::on_lineEdit_textChanged(const QString &arg1)
{
    QSqlTableModel * model = new QSqlTableModel(this,db);//新建model
    model->setTable("EXAMPLE");//设置表
    if(!arg1.isEmpty())//搜索栏为空
        model->setFilter("city ='"+arg1+"'");//model过滤表中符合的数据
    ui->tableView->setModel(model);
    model->select();//显示修改后的数据
}

