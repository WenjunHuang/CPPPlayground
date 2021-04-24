//
// Created by rick on 2021/4/23.
//
#include <QTest>
#include <QDebug>
class TestLifecycle: public QObject{
    Q_OBJECT
private:
    bool myCondition(){
        return true;
    }
private slots:
    void initTestCase() {
        qDebug() << "Called before everything else.";
    };

    void myFirstTest(){
        QVERIFY2(true,"true forever");
        QCOMPARE(1,1);
    }
    void mySecondTest(){
        QVERIFY2(myCondition(),"my condition is good");
        QVERIFY2(1 == 2,"1 is not equal to 2");
    }

    void cleanupTestCase() {
        qDebug() << "Called after myFirstTest and mySecondTest";
    }

};
QTEST_MAIN(TestLifecycle)

#include "test_lifecycle.moc"