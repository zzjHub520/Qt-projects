Qt QSqlTableModel 使用心得

连接数据库

执行sql查询，条件显示，排序

获取记录数，列数以及记录内容，字段内容

新增，修改，删除，恢复

其它



1---------------连接数据库（我用的access2003做实验）

在.pro文件添加

QT +=SQL

2-----执行sql查询，条件显示，排序

```cpp
model = newQSqlTableModel(this);
model->setTable(“student”);
model->setEditStrategy(QSqlTableModel::OnManualSubmit);
model->select(); //选取整个表的所有行
// model->removeColumn(1); //不显示第二列,如果这时添加记录，则该属性的值添加不上
QTableView *view= new QTableView;
view->setModel(model);
view->show();
//View->setEditTriggers(QAbstractItemView::NoEditTriggers); 使其不可编辑
条件：（等价于SQL语句的where）
model->setFilter(QObject::tr(“name= ‘admin’”).arg(name)); //根据姓名进行筛选
model->select(); //显示结果
排序:(等价于ORDERBY)
model->setSort(0,Qt::AscendingOrder); //id属性，即第0列，升序排列
model->select();
 
model->setSort(0,Qt::DescendingOrder); //id属性，即第0列，降序排
model->select();
```

3------获取记录数，列数以及记录内容，字段内容

```
记录数： Model->rowcount
记录值：
值=model.record(num).value(1).toString();
Qrecord record= QSqlRecord record = tablemodel.record(记录行数);
值=Record.value(“字段名或索引”)
字段数：
字段值：
```

4------新增，修改，删除,提交，撤销

```cpp
新增：
int rowNum = model->rowCount();//获得表的行数
int id = 10;
model->insertRow(rowNum); //添加一行
model->setData(model->index(rowNum,0),id);
//model->submitAll(); //可以直接提交
或者用record添加
QSqlRecord record = tablemodel.record();
record.setValue("nam","new");
model.insertRecord(1,record);
  
修改：
首先用条件找到某一条记录
QSqlRecord record = tablemodel.record(num);
record.setValue("name",record.value("name").toString()+"2");
tablemodel.setRecord(num,record);
if(tablemodel.submitAll())
"成功！"
else
"失败！"
或者用下面方法用setData()来修改，代码如下： 
model.setData(model.index(1,1),"new"); 
if(tablemodel.submitAll()) 
"成功！"
else 
"失败！"
  
删除：
int curRow = ui->tableView->currentIndex().row();
//获取选中的行
model->removeRow(curRow);
//删除该行
int ok = QMessageBox::warning(this,tr(“删除当前行!”),tr(“你确定”
“删除当前行吗？”),
QMessageBox::Yes,QMessageBox::No);
if(ok == QMessageBox::No)
{
model->revertAll();//如果不删除，则撤销
}
else model->submitAll(); //否则提交，在数据库中删除该行
如果没有table
tablemodel.removeRows(起始行, 要删除的总行数); 
model.submitAll();
  
提交
model->database().transaction();//开始事务操作
if (model->submitAll()) {
model->database().commit();//提交
} else {
model->database().rollback();//回滚
QMessageBox::warning(this,tr(“tableModel”),
tr(“数据库错误: %1″)
.arg(model->lastError().text()));
}
撤销
model->revertAll();
```

5------其它

```cpp
//一、在QTableView中显示数据库中表的数据
QSqlTableModel *model = new QSqlTableModel(parentObject, database); // 摘抄自帮助文档
model->setTable("employee");
model->setEditStrategy(QSqlTableModel::OnManualSubmit);
model->select();
model->removeColumn(0); // don't show the ID
model->setHeaderData(0, Qt::Horizontal, tr("Name"));
model->setHeaderData(1, Qt::Horizontal, tr("Salary"));
QTableView *view = new QTableView;
view->setModel(model);
view->show();
//二、修改QTableView中数据后的提交，加入事务处理
model->database().transaction(); //开始事务操作
if (model->submitAll()) // 提交所有被修改的数据到数据库中
{
model->database().commit(); //提交成功，事务将真正修改数据库数据
} else {
model->database().rollback(); //提交失败，事务回滚
QMessageBox::warning(this, tr(“tableModel”),tr(“数据库错误: %1″).arg(model->lastError().text()));
}
model->revertAll(); //撤销修改
//三、查询操作,相当于SQL语句:SELECT * FROM 表名 WHERE name = "name变量"
 
model->setFilter(QObject::tr(“name = ‘%1′”).arg(name)); //根据姓名进行筛选
model->select(); //显示结果
for (int i = 0; i < model.rowCount(); ++i)
{
QString name = model.record(i).value("name").toString();
// ... 在此处理每一条的记录
}
// 在操作大数据集时，建议通过索引指定字段
int primaryKeyIndex = model.record().indexOf("id");
for (int i = 0; i < model.rowCount(); ++i)
{
QSqlRecord record = model.record(i);
QString name = record.value("name").toString();
// ... 在此处理每一条的记录
}
 
// 四、排序操作
model->setSort(0,Qt::AscendingOrder); //id属性，即第0列，升序排列,Qt::DescendingOrder为降序排序
model->select();
 
//五、插入操作
int rowNum = model->rowCount(); //获得表的行数
int id = 最后一个ID+1;
model->insertRow(rowNum); //添加一行,或者用insertRows(0,1),在0行添加1条记录，根据表的排序规则，可能移到与指定行不同的行位置上
model->setData(model->index(rowNum,0),id); //因为这里设置了ID为主键，所以必须给新行添加id属性值,id字段在第0列上
model->submitAll(); //可以直接提交
 
// 六、删除一条记录
model.setFilter("id = 10");
model.select();
if (model.rowCount() == 1)
{
model.removeRows(0,1) // 如果要删除所有满足条件的记录则把1改成model.rowCount()
model.submitAll();
}
 
// 在QTableView中删除选中的一行
int curRow = tableView->currentIndex().row();
model->removeRow(curRow); //删除一行
 
// 在QTableView中删除选中的多行
QAbstractItemView::SelectionModeselectionMode()const // 原型
QModelIndexListQItemSelectionModel::selectedIndexes()const //原型
QItemSelectionModel *selections = tableView->selectionModel(); //返回当前的选择模式
QModelIndexList selecteds = selections->selectedIndexes(); //返回所有选定的模型项目索引列表
foreach (QModelIndex index, selecteds)
{
int curRow = index.row(); //删除所有被选中的行
model->removeRow(curRow);
}
int ok = QMessageBox::warning(this,tr("删除选中的行!"),tr("你确定删除当前选取中的行吗?"),QMessageBox::Yes,QMessageBox::No);
if(ok == QMessageBox::Yes)
{
model->submitAll(); //提交，在数据库中删除该行
} else {
model->revertAll(); //如果不删除，则撤销
}
 
// 七、更新记录
model.setFilter("id = 10");
model.select();
if (model.rowCount() == 1)
{
model.setData(model.index(0,1),QObject::tr("小王"));
model.submitAll();
}
```

