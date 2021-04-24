//
// Created by rick on 2021/4/23.
//
#include <QtTest/QtTest>
#include <QtWidgets>

class TestGui : public QObject {
    Q_OBJECT

private slots:
    void testGui_data()
    {
        QTest::addColumn<QTestEventList>("events");
        QTest::addColumn<QString>("expected");

        QTestEventList list1;
        list1.addKeyClick('a');
        QTest::newRow("char") << list1 << "a";

        QTestEventList list2;
        list2.addKeyClick('a');
        list2.addKeyClick(Qt::Key_Backspace);
        QTest::newRow("there and back again") << list2 << "";
    }
    void testGui()
    {
        QFETCH(QTestEventList, events);
        QFETCH(QString, expected);

        QLineEdit lineEdit;

        events.simulate(&lineEdit);

        QCOMPARE(lineEdit.text(), expected);
    }
};
QTEST_MAIN(TestGui)
#include "test_gui.moc"