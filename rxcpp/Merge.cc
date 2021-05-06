//
// Created by HUANG WEN JUN on 2020/11/17.
//

#include <QDebug>
#include <QString>
#include <array>
#include <chrono>
#include <iostream>
#include <rx.hpp>
#include <sstream>
#include <thread>

inline QString currentThreadId()
{
    auto tid = std::this_thread::get_id();
    std::stringbuf buf;
    std::ostream out { &buf };
    out << tid;
    return QString { buf.str().data() };
}

int main()
{
    qDebug() << QString("[thread %1] Start task").arg(currentThreadId());

    auto o1 = rxcpp::observable<>::timer(std::chrono::milliseconds(10),rxcpp::identity_current_thread()).map([](int) {
        qDebug() << QString("[thread %1] Timer1 fired").arg(currentThreadId());
        return 1;
    });
    auto o2 = rxcpp::observable<>::timer(std::chrono::milliseconds(20)).map([](int) {
        qDebug() << QString("[thread %1] Timer2 fired").arg(currentThreadId());
        return 2;
    });
    auto o3 = rxcpp::observable<>::timer(std::chrono::milliseconds(30)).map([](int) {
        qDebug() << QString("[thread %1] Timer3 fired").arg(currentThreadId());
        return 3;
    });

    auto base = rxcpp::observable<>::from(o1.as_dynamic(), o2, o3);
    auto values = base.merge(rxcpp::observe_on_new_thread());
//    auto values = base.merge();
    values.as_blocking()
        .subscribe([](int v) { qDebug() << QString("[thread %1] OnNext: %2").arg(currentThreadId()).arg(v); },
            []() {
                qDebug() << QString("[thread %1] OnCompleted").arg(currentThreadId());
            });
    qDebug() << QString("[thread %1] Finish task").arg(currentThreadId());
}