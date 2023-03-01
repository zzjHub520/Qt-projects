#ifndef MYSQLQUERYMODEL_H
#define MYSQLQUERYMODEL_H

#include <QSqlQueryModel>

class MySqlQueryModel : public QSqlQueryModel
{
public:
    explicit MySqlQueryModel(QObject *parent = 0);

    // QAbstractItemModel interface
public:
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    Qt::ItemFlags flags(const QModelIndex &index) const;


private:
    bool setName(int id, const QString &name);
    void refresh();
};

#endif // MYSQLQUERYMODEL_H
