#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->setWindowTitle("ocean");
    //this->resize(1931,1087);
    int rsz=1800;
    this->resize(rsz,1087.0*rsz/1931.0);

    QLineSeries *series=new QLineSeries;

    series->append(0,2);
    series->append(QPointF(2,6));
    series->append(3,8);
    series->append(7,9);
    series->append(11,3);

    *series << QPointF(11,2) << QPointF(15,5) << QPointF(18,4) << QPointF(19,2);

    QChart * chart = new QChart();
    chart->legend()->hide();
    chart->addSeries(series);
    series->setUseOpenGL(true);
    chart->createDefaultAxes();
    chart->setTitle(QStringLiteral("Qt line chart example"));

    QChartView * view = new QChartView(chart);
    view->setParent(this);
    view->setRenderHint(QPainter::Antialiasing);
    view->resize(400,300);
    view->move(440,423);
    view->show();

}

MainWindow::~MainWindow()
{

}

void MainWindow::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    QPixmap pix;
    pix.load(":/icon/ImageIcon/155.png");//图片的位置
    painter.drawPixmap(0,0,this->width(),this->height(),pix);//根据窗口的宽高来将图片画在窗口上


}
