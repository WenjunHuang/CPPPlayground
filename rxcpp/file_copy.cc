//
// Created by rick on 2021/5/26.
//
#include <QByteArray>
#include <QFile>
#include <rx.hpp>
#include <utility>

using namespace rxcpp;

int main(int argc, char* argv[])
{
    QString sourceFileName { QLatin1String(argv[1]) };
    QString targetFileName { QLatin1String(argv[2]) };

    auto source = observable<>::create<QByteArray>([sourceFileName](rxcpp::subscriber<QByteArray> s) {
        QFile sourceFile { sourceFileName };
        if (!sourceFile.open(QFile::ReadOnly)) {
            s.on_error(std::make_exception_ptr(std::invalid_argument("source file error")));
        }
        QByteArray bytes;
        while (!(bytes = sourceFile.read(1024)).isEmpty()) {
            if (!s.is_subscribed())
                break;
            s.on_next(std::move(bytes));
        }
        s.on_completed();
    }).subscribe()
}