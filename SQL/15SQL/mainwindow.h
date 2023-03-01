#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlTableModel>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private slots:
    void on_pushButton_clicked(); 		// 提交修改

    void on_pushButton_2_clicked(); 	// 撤销修改

    void on_pushButton_3_clicked(); 	// 添加记录

    void on_pushButton_4_clicked(); 	// 删除选中行

    void on_pushButton_5_clicked(); 	// 查询

    void on_pushButton_6_clicked(); 	// 显示全表

    void on_pushButton_7_clicked(); 	// 按ID升序排序

    void on_pushButton_8_clicked(); 	// 按ID降序排序

private:
    Ui::MainWindow *ui;
    QSqlTableModel *model;
};
#endif // MAINWINDOW_H
