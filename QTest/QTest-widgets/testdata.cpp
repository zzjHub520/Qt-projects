#include "testdata.h"

TestData::TestData(QObject *parent) : QObject(parent)
{

}

void TestData::writeText(){
    QString text = "A";
    text.toLower();
}
void TestData::testDataIsSame(){
    // 获取测试数据
    QFETCH(int, value1);
    QFETCH(int, value2);
    // 如果输入的两个参数不同，则会被认为测试失败
    QCOMPARE(value1, value2);
}

void TestData::testDataIsSame_data(){
    // 添加数据列
    QTest::addColumn<int>("value1");
    QTest::addColumn<int>("value2");
    // 添加测试数据
    QTest::newRow("v1") << 1 << 1;
    QTest::newRow("v2") << 2 << 2;
    QTest::newRow("v3") << 1 << 2;
}

void TestData::testDataIsStrSame(){
    // 获取测试数据
    QFETCH(QString, value1);
    QFETCH(QString, value2);
    // 如果输入的两个参数不同，则会被认为测试失败
    QCOMPARE(value1.toLower(), value2);
}

void TestData::testDataIsStrSame_data(){
    // 添加数据列
    QTest::addColumn<QString>("value1");
    QTest::addColumn<QString>("value2");
    // 添加测试数据
    QTest::newRow("lower") << "hello" << "hello";
    QTest::newRow("mixed") << "HeLlOL" << "hello";
    QTest::newRow("upper") << "HELLO" << "hello";
}

void TestData::testGui(){
    QFETCH(QTestEventList, events);
    QFETCH(QString, expected);
    QLineEdit lineEdit;
    // 模拟按键事件，并比较结果
    events.simulate(&lineEdit);
    /*测试两个数据是否相同*/
    QCOMPARE(lineEdit.text(), expected);
}
void TestData::testGui_data(){
    //添加两列的数据
    QTest::addColumn<QTestEventList>("events");
    QTest::addColumn<QString>("expected");

    /*没有肯定失败*/
    // 添加按键事件
    QTestEventList list1;
    //添加模拟按键a
    list1.addKeyClick('a');
    QTest::newRow("测试是否有按键aaaaa:") << list1 << "aaaaa";

    /*有空格键可以成功*/
    QTestEventList list2;
    list2.addKeyClick('a');
    list2.addKeyClick(Qt::Key_Backspace);
    QTest::newRow("测试是否有空格键：") << list2 << "";
}
