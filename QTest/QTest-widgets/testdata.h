#ifndef TESTDATA_H
#define TESTDATA_H

#include <QObject>
#include <QtTest>
#include <QLineEdit>

class TestData : public QObject
{
    Q_OBJECT
public:
    explicit TestData(QObject *parent = nullptr);

    ~TestData()=default;
//每个private的slot都会被测试函数自动调用
private slots:
    /*测试int类型数据*/
    //对数据进行检测
    void testDataIsSame();
    // 以“_data”结尾的函数向对应的测试函数提供测试数据
    void testDataIsSame_data();

    /*测试QString类型的数据*/
    //对数据进行检测
    void testDataIsStrSame();
    // 以“_data”结尾的函数向对应的测试函数提供测试数据
    void testDataIsStrSame_data();

    /*测试GUI*/
    //对数据进行检测
    void testGui();
    // 以“_data”结尾的函数向对应的测试函数提供测试数据
    void testGui_data();

public:
    //测试运行速度
    void writeText();
signals:

};

#endif // TESTDATA_H
