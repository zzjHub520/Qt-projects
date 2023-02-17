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
#include <QChartView>
#include <QLineSeries>

QT_CHARTS_USE_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    int intervalPushButton=106;
    int xStartPushButton=580;
    int yStartPushButton =910;
    QToolButton * tbtn[6];
    bool onOff[6];
    int outOfRanks=20;
    QLabel * labelSelected[6];
    QLabel * labelUnselected[6];
    QLabel * labelText[6];

    QLineSeries *series[6];
    QChart * chart[6];
    QChartView * viewChart[6];



public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void paintEvent(QPaintEvent*);
    void offAll(int duty);
    void tbtnClickedBody(int duty);

public slots:
    void test();
    void tbtnClicked_0();
    void tbtnClicked_1();
    void tbtnClicked_2();
    void tbtnClicked_3();
    void tbtnClicked_4();
    void tbtnClicked_5();

};

#endif // MAINWINDOW_H
