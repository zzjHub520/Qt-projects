#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QTreeView>
#include <QTableView>
#include <QHeaderView>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
    void CreateView();
private:
    Ui::Widget *ui;
    QPushButton *testBtn;
    QPushButton *m_PushButton;
    QVBoxLayout *mainLayout;
//    QTreeView *treeView;
    QStandardItemModel *standItemModel;
    QTableView *tableView;
};
#endif // WIDGET_H
