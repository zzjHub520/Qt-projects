#include "mainwindow.h"
#include <QApplication>
#include <QPalette>
#include <QPixmap>
#include <QBrush>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();




    return a.exec();
}



