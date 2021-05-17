//
// Created by rick on 2021/4/28.
//
#include <QtCore/QtCore>
#include <rx.hpp>
#include <string>
namespace Rx {
using namespace rxcpp;
using namespace rxcpp::operators;
}
using namespace Rx;

int main(int argc, char* argv[])
{
    auto coordination = observe_on_new_thread();
    auto obs = observable<>::create<QString>([](subscriber<QString> out) {
        while (out.is_subscribed()) {
            out.on_next(QString("From thread: %1\n").arg((int)QThread::currentThreadId()));
            QThread::sleep(1000);
        }
        out.on_completed();
    })
        | subscribe_on(coordination)
        | publish()
        | connect_forever();
}