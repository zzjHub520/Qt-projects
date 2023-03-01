#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QSqlTableModel>
#include <QSqlDatabase>
#include <QDataWidgetMapper>
#include "ComboBoxDelegate.h"
#include "SpinBoxDelegate.h"
#include <QModelIndex>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private:
    void initTableView();

private slots:
    void on_btnOpen_clicked();

    void on_rBMan_clicked();

    void on_rBWoman_clicked();

    void on_rBAll_clicked();

    void on_rBUp_clicked();

    void on_rBDown_clicked();

    void on_cbxSort_currentIndexChanged(int index);

    void on_tableView_clicked(const QModelIndex &index);

    void on_btnAdd_clicked();

    void on_btnDel_clicked();

    void on_btnCancel_clicked();

    void on_btnSave_clicked();

private:
    Ui::Widget *ui;

private:
    QSqlTableModel *m_model = nullptr;

    ComboBoxDelegate m_cbxDelegate;

    SpinBoxDelegate m_spinDelegate;

    QSqlDatabase m_db;

    QDataWidgetMapper *m_dataMapper = nullptr;
};

#endif // WIDGET_H
