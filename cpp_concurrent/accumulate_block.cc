//
// Created by HUANG WEN JUN on 2020/1/7.
//
#include <numeric>
#include <thread>
#include <vector>

template <typename Iterator, typename T> struct AccumulateBlock {
    void operator()(Iterator first, Iterator last, T& result) {
        result = std::accumulate(first, last, result);
    }
};

template <typename Iterator, typename T>
T parallelAccumulate(Iterator first, Iterator last, T init) {
    auto dif = std::distance(first, last);
    if (!dif)
        return init;
    auto const minPerThread    = 25;
    auto const maxThreads      = (dif + minPerThread - 1) / minPerThread;
    auto const hardwareThreads = std::thread::hardware_concurrency();
    auto const numThreads =
        std::min(hardwareThreads != 0 ? hardwareThreads : 2, maxThreads);
    auto const blockSize = dif / numThreads;

    std::vector<T> results(numThreads);
    std::vector<std::thread> threads(numThreads - 1);

    auto blockStart = first;
    for (auto i = 0; i < (numThreads - 1); ++i) {
        auto blockEnd = blockStart;
        std::advance(blockEnd, blockSize);
        threads[i] = std::thread(AccumulateBlock<Iterator, T>(), blockStart,
                                 blockEnd, std::ref(results[i]));
        blockStart = blockEnd;
    }
    AccumulateBlock<Iterator,T>()(blockStart,last,results[numThreads - 1]);

    for (auto& entry:threads)
        entry.join();
    return std::accumulate(results.begin(),results.end(),init);
}