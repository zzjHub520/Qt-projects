#ifndef CONNECTION_H
#define CONNECTION_H

#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

static bool createConnection()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("test.db");
    if (!db.open())
    {
        return false;
    }

    QSqlQuery query;
    query.exec("create table student(id int primary key, name varchar(20))");
    query.exec("insert into student values(1, ' 王鹏')");
    query.exec("insert into student values(2, '李刚')");
    query.exec("insert into student values(3, '李强')");
    query.exec("insert into student values(4, '王强')");
    query.exec("insert into student values(5, '黎明朗')");

    return true;
}

#endif // CONNECTION_H
