#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->setWindowTitle("ocean");
    //this->resize(1931,1087);
    int rsz=1800;
    this->resize(rsz,1087.0*rsz/1931.0);
    //this->setStyleSheet("background-color:rgb(234,234,234)");

    for(int i=0; i<6; i++)
    {
        onOff[i]=0;
    }

    intervalPushButton=106;
    xStartPushButton=580;
    yStartPushButton =910;

    for(int i=0; i<6; i++)
    {
        tbtn[i]=new QToolButton;
        tbtn[i]->setParent(this);
    //    QFont font("华文行楷", 20, 10, 1); // 创建字体对象，（字体，大小，加粗，倾斜）
    //    tbtn[i]->setFont(font);
        //tbtn[i]->setText("xiaf");
        tbtn[i]->setIcon(QIcon(":/icon/ImageIcon/20.png"));
        tbtn[i]->setIconSize(QSize(90,90));
        tbtn[i]->move(xStartPushButton+i*intervalPushButton,yStartPushButton);
        qDebug() << xStartPushButton+i*intervalPushButton << " " << yStartPushButton;
        tbtn[i]->resize(100,100);
        //tbtn[i]->setWindowTitle("ocean");
        tbtn[i]->setStyleSheet("border:none;");
        tbtn[i]->show();
    }

    int iconArray[6][2]={{792,810},{501,341},{341,431},
                         {680,280},{581,481},{581,401}};

    QString strLabelUnselected[6]={":/icon/ImageIcon/30.png", ":/icon/ImageIcon/30.png", ":/icon/ImageIcon/30.png",
                                   ":/icon/ImageIcon/30.png", ":/icon/ImageIcon/30.png", ":/icon/ImageIcon/30.png"};

    for(int i=0;i<6;i++)
    {
        QMovie *movie=new QMovie("C:/Users/20200062/Documents/Qt_Programme/Qt_test/oceanMainWindow/ImageIcon/66.gif");
        labelSelected[i]=new QLabel;
        labelSelected[i]->setParent(this);
        labelSelected[i]->move(iconArray[i][0],iconArray[i][1]);
        labelSelected[i]->resize(100,100);
        labelSelected[i]->setMovie(movie);
        movie->start();
        labelSelected[i]->hide();

        labelUnselected[i]=new QLabel;
        QImage *img1=new QImage;
        img1->load(strLabelUnselected[i]);
        labelUnselected[i]->setParent(this);
        labelUnselected[i]->setPixmap(QPixmap::fromImage(*img1));
        labelUnselected[i]->move(iconArray[i][0],iconArray[i][1]);
        labelUnselected[i]->resize(100,100);
        labelUnselected[i]->show();
    }

    QString textData[6]={"hello world 1", "hello world 2", "hello world 3",
                        "hello world 4", "hello world 5", "hello world 6"};
    for(int i=0;i<6;i++)
    {
        labelText[i] = new QLabel;
        labelText[i]->setParent(this);
        labelText[i]->move(1200,205);
        labelText[i]->resize(300,300);
        labelText[i]->setText(textData[i]);
        labelText[i]->setFont(QFont( "微软雅黑",20,5,0));
        labelText[i]->hide();
    }




    connect(tbtn[0], &QToolButton::clicked, this, &MainWindow::tbtnClicked_0);
    connect(tbtn[1], &QToolButton::clicked, this, &MainWindow::tbtnClicked_1);
    connect(tbtn[2], &QToolButton::clicked, this, &MainWindow::tbtnClicked_2);
    connect(tbtn[3], &QToolButton::clicked, this, &MainWindow::tbtnClicked_3);
    connect(tbtn[4], &QToolButton::clicked, this, &MainWindow::tbtnClicked_4);
    connect(tbtn[5], &QToolButton::clicked, this, &MainWindow::tbtnClicked_5);

}

MainWindow::~MainWindow()
{

}

void MainWindow::test()
{
    qDebug() << "hello";
}

void MainWindow::offAll(int duty)
{
    for(int i=0;i<6;i++)
    {
        if(duty!=i)
        {
            onOff[i] = 0;
            tbtn[i]->move(xStartPushButton+i*intervalPushButton,yStartPushButton);
            labelSelected[i]->hide();
            labelUnselected[i]->show();
            labelText[i]->hide();
        }
    }
}

void MainWindow::tbtnClickedBody(int duty)
{
    offAll(duty);
    if(onOff[duty]==0)
    {
        tbtn[duty]->move(xStartPushButton+duty*intervalPushButton,yStartPushButton-outOfRanks);
        labelSelected[duty]->show();
        labelUnselected[duty]->hide();
        labelText[duty]->show();
    }
    else
    {
        tbtn[duty]->move(xStartPushButton+duty*intervalPushButton,yStartPushButton);
        labelSelected[duty]->hide();
        labelUnselected[duty]->show();
        labelText[duty]->hide();
    }
    onOff[duty]=!onOff[duty];
}

void MainWindow::tbtnClicked_0()
{
    tbtnClickedBody(0);
}

void MainWindow::tbtnClicked_1()
{
    tbtnClickedBody(1);
}

void MainWindow::tbtnClicked_2()
{
    tbtnClickedBody(2);
}

void MainWindow::tbtnClicked_3()
{
    tbtnClickedBody(3);
}

void MainWindow::tbtnClicked_4()
{
    tbtnClickedBody(4);
}

void MainWindow::tbtnClicked_5()
{
    tbtnClickedBody(5);
}

void MainWindow::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    QPixmap pix;
    pix.load(":/icon/ImageIcon/155.png");//图片的位置
    painter.drawPixmap(0,0,this->width(),this->height(),pix);//根据窗口的宽高来将图片画在窗口上


}






