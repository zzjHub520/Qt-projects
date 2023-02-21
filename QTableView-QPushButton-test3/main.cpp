#include "widget.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.resize(960,640);
    w.setWindowTitle(QString::fromUtf8("表格控件QTableView用法示例"));
    w.show();
    return a.exec();
}
