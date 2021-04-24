//
// Created by rick on 2021/4/23.
//
#include <QtTest/QtTest>
#include <QtWidgets>
class TestBenchmark: public QObject{
    Q_OBJECT
private slots:
    void multiple_data()
    {
        QTest::addColumn<bool>("useLocaleCompare");
        QTest::newRow("locale aware compare") << true;
        QTest::newRow("standard compare") << false;
    }
    void multiple()
    {
        QFETCH(bool, useLocaleCompare);
        QString str1 = QLatin1String("This is a test string");
        QString str2 = QLatin1String("This is a test string");

        int result;
        if (useLocaleCompare) {
            QBENCHMARK {
                result = str1.localeAwareCompare(str2);
            }
        } else {
            QBENCHMARK {
                result = (str1 == str2);
            }
        }
        Q_UNUSED(result);
    }
    void simple() {
        QString str1 = QLatin1String("This is a test string");
        QString str2 = QLatin1String("This is a test string");

        QCOMPARE(str1.localeAwareCompare(str2), 0);

        QBENCHMARK {
            str1.localeAwareCompare(str2);
        }
    }
};
QTEST_MAIN(TestBenchmark)
#include "test_benchmark.moc"