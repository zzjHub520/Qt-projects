#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
    class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget {
Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);

    ~Widget();

private slots:

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();


private:
    Ui::Widget *ui;
    const QString STATUS_NAME = "status";
    const QString COLOR_RED = "red";
    const QString COLOR_BLUE = "blue";
    bool flag;
    bool flag2;
    bool flag3;

    void SetStyleSheet();


};

#endif // WIDGET_H
