#include <QTest>
#include <QApplication>
#include <QtDebug>
#include <QPushButton>
#include "widget.h"
#include "Attach.h"
using namespace swp;

class QTestDemo: public QObject
{
    Q_OBJECT

private slots:
    void toUpper_data();
    void toUpper();
};

void QTestDemo::toUpper_data()
{
    QTest::addColumn<QString>("string");
    QTest::addColumn<QString>("result");

    QTest::newRow("all lower") << "hello" << "HELLO";
    QTest::newRow("mixed")     << "Hello" << "HELLO";
    QTest::newRow("all upper") << "HELLO" << "HELLO";
}

void QTestDemo::toUpper()
{
    QFETCH(QString, string);
    QFETCH(QString, result);
    QPushButton btn;
    Widget w;
    QCOMPARE(string.toUpper(), result);
    Attach::AA();
}

QTEST_MAIN(QTestDemo)
#include "QTestDemo.moc"
