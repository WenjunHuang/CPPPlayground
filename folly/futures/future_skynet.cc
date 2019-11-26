//
// Created by HUANG WEN JUN on 2019/11/26.
//
#include <chrono>
#include <folly/executors/GlobalExecutor.h>
#include <folly/executors/CPUThreadPoolExecutor.h>
#include <folly/futures/Future.h>
#include <iostream>
#include <numeric>
#include <vector>
#include <thread>
using namespace folly;
using namespace std;

SemiFuture<uint64_t> skynetAsync(uint64_t num, uint64_t size, uint64_t div,folly::Executor* executor) {
    if (size == 1) {
        return folly::makeSemiFuture(num);
    } else {
        vector<SemiFuture<uint64_t>> futures;
        for (auto i = 0; i < div; i++)
            futures.push_back(
                skynetAsync(num + i * size / div, size / div, div,executor));

        auto f = collectAllSemiFuture(futures)
                     .via(executor)
                     .thenValue([](const vector<Try<uint64_t>>& result) {
                         auto threadId = std::this_thread::get_id();
//                         std::cout << threadId << std::endl;
                         return std::accumulate(
                             result.cbegin(), result.cend(), uint64_t(0),
                             [](uint64_t accum, const Try<uint64_t>& item) {
                                 if (item.hasValue())
                                     return accum + item.value();
                                 else
                                     return accum;
                             });
                     });

        return f;

        //        return AsyncFuture::observe(flatMap->start())
        //            .subscribe([](QVector<quint64>* r) {
        //              return QtConcurrent::run([=]() {
        //                quint64 sum = 0;
        //                for (const auto& i : *r) {
        //                    sum += i;
        //                }
        //                return sum;
        //              });
        //            })
        //            .future();
    }
}

int skyNet1() {
    CPUThreadPoolExecutor executor(6);

    auto start = std::chrono::high_resolution_clock::now();


    auto result = skynetAsync(0, 1000000, 10,&executor);
    std::move(result)
        .via(&executor)
        .thenValue([&](uint64_t result) {
            auto end = std::chrono::high_resolution_clock::now();
            std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(
                             end - start)
                             .count()
                      << " milliseconds" << std::endl;
            std::cout << result << std::endl;
        });
    //    qDebug() << result.result();
}

int main(int argc, char* argv[]) { skyNet1(); }