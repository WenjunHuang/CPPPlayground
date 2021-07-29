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

    observable<>::scope([sourceFileName]() {
        QFile* file = new QFile(sourceFileName);
        if (!file->open(QIODevice::ReadOnly))
            return resource<QFile*>(nullptr);
        else {
            composite_subscription sub;
            sub.add([file]() { file->deleteLater(); });
            return resource<QFile*> { file, sub };
        }
    },[](resource<QFile*> resource)->observable<QByteArray>{
            if (resource.get() == nullptr)
                return observable<>::error<QByteArray>(QString{"error"});
            else {
                return observable<>::create<QByteArray>([resource](subscriber<QByteArray> subscriber)mutable{
                    auto file = resource.get();
                    QByteArray buffer;
                    while (!(buffer = file->read(1024 * 1024)).isEmpty()) {
                        subscriber.on_next(buffer);
                    }
                    subscriber.on_completed();
                });
            }
        });
    observable<>::create<QByteArray>([sourceFileName](rxcpp::subscriber<QByteArray> s) {
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