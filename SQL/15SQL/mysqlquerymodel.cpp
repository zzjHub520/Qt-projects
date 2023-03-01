#include "mysqlquerymodel.h"
#include <QSqlQuery>
#include <QColor>
#include <QModelIndex>

MySqlQueryModel::MySqlQueryModel(QObject *parent):QSqlQueryModel(parent)
{

}

//添加数据
bool MySqlQueryModel::setData(const QModelIndex &index, const QVariant &value, int role)
{

    Q_UNUSED(role)

    if (index.column() < 1 || index.column() > 2)
    {
        return false;
    }

    QModelIndex primaryKeyIndex = QSqlQueryModel::index(index.row(), 0);
    //获取ID号
    int id = data(primaryKeyIndex).toInt();

    clear();

    bool ret = false;
    //设置第二个字段可以更改
    if (index.column() == 1)
    {
        ret = setName(id, value.toString());
    }

    refresh();

    return ret;
}

//返回表格是否可以被更改的标志
Qt::ItemFlags MySqlQueryModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags flags = QSqlQueryModel::flags(index);

    //第二个字段可以更改
    if (index.column() == 1)
    {
        flags |= Qt::ItemIsEditable;
    }

    return flags;
}


//更改数据显示样式
QVariant MySqlQueryModel::data(const QModelIndex &index, int role) const
{
    QVariant value = QSqlQueryModel::data(index, role);

    //第一个字段的字体颜色为红色
    if (role == Qt::TextColorRole && index.column() == 0)
    {
        return QVariant::fromValue(QColor(Qt::red));
    }

    return value;
}

//添加name字段的值
bool MySqlQueryModel::setName(int id, const QString &name)
{
    QSqlQuery query;

    query.prepare("update student set name = ? where id = ?");
    query.addBindValue(name);
    query.addBindValue(id);

    return query.exec();
}

//更新显示
void MySqlQueryModel::refresh()
{
    setQuery("select * from student");
    setHeaderData(0, Qt::Horizontal, QObject::tr("id"));
    setHeaderData(1, Qt::Horizontal, QObject::tr("name"));
}
