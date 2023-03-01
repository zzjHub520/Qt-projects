#include "widget.h"
#include <QApplication>
#include <QFile>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.show();

//    QString fileName = "test.dat";

//    QFile file(fileName);

//    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
//        return false;

//    //流读取文件
//    QDataStream stream(&file);

//    //windows平台
//    stream.setByteOrder(QDataStream::LittleEndian);

//    //写入流
//    stream<<QString("张三");
//    stream<<qint32(18);

//    //关闭文件
//    file.close();

//    QString fileName = "test.dat";

//    QFile file(fileName);

//    if (!file.open(QIODevice::ReadOnly))
//        return false;

//    //流读取文件
//    QDataStream stream(&file);

//    //windows平台
//    stream.setByteOrder(QDataStream::LittleEndian);

//    QString name;
//    int age = 0;

//    //输出流
//    stream>>name;
//    stream>>age;

//    //关闭文件
//    file.close();

    return a.exec();
}
