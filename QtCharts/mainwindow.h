#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QPaintEvent>
#include <QToolButton>
#include <QLabel>
#include <QPainter>
#include <QDebug>
#include <QMovie>
#include <QTextDocument>
#include <QMainWindow>
#include <QChartView>
#include <QLineSeries>

QT_CHARTS_USE_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void paintEvent(QPaintEvent*);
};

#endif // MAINWINDOW_H
