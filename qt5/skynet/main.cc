#include "asyncfuture.h"
#include <QVector>
#include <QtConcurrent>
#include <QtCore>
#include <chrono>
#include <functional>
#include <utility>

class FlatMap : public QObject {
    Q_OBJECT
  public:
    FlatMap(QVector<QFuture<quint64>>&& outer) : _outer{std::move(outer)} {
        _promise.setExpectedResultCount(1);
        _result = new QVector<quint64>();
    }

    QFuture<QVector<quint64> *> start() {
        for (auto f : _outer) {
            AsyncFuture::observe(f)
                .subscribe([this](quint64 result) mutable {
                    _result->append(result);
                    if (_result->size() == _outer.size()) {
                        _promise.reportFinished(&_result);
                    }
                })
                .future();
        }
        return _promise.future();
    }

  private:
    QVector<QFuture<quint64>> _outer;
    QVector<quint64> *_result;
    QFutureInterface<QVector<quint64>*> _promise;
};

QFuture<quint64> skynetAsync(quint64 num, quint64 size, quint64 div) {
    if (size == 1) {
        QFutureInterface<quint64> promise;
        promise.setExpectedResultCount(1);
        promise.reportFinished(&num);
        auto f = promise.future();
        return f;
    } else {
        QVector<QFuture<quint64>> result;
        for (auto i = 0; i < div; i++)
            result.append(skynetAsync(num + i * size / div, size / div, div));

        auto flatMap = new FlatMap(std::move(result));
        return AsyncFuture::observe(flatMap->start())
            .subscribe([](QVector<quint64>* r) {
                return QtConcurrent::run([=]() {
                    quint64 sum = 0;
                    for (const auto& i : *r) {
                        sum += i;
                    }
                    return sum;
                });
            })
            .future();
    }
}

int skyNet1(QCoreApplication& app) {

    QFutureInterface<quint64> promise;
    promise.setExpectedResultCount(1);

    auto start = std::chrono::high_resolution_clock::now();

    auto result = skynetAsync(0, 1000000, 10);
    AsyncFuture::observe(result).subscribe([&](quint64 result) {
      qDebug() << result;
      auto end = std::chrono::high_resolution_clock::now();
      qDebug() << std::chrono::duration_cast<std::chrono::milliseconds>(end -
                                                                        start)
          .count()
               << " milliseconds";
      app.quit();
    });
    //    qDebug() << result.result();
    return app.exec();
}

//int skyNet2(QCoreApplication& app) {
//    QtConcurrent::mappedR
//}

int main(int argc, char* argv[]) {
    QCoreApplication app(argc, argv);
    return skyNet1(app);

}
#include "main.moc"