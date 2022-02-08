//
// Created by rick on 2022/2/8.
//

#include <benchmark/benchmark.h>
#include <QString>
#include <QSharedDataPointer>
#include <array>
struct DataNormal {
    std::array<char,100> data{};
};

struct DataWithSharedDataData: public QSharedData{
    std::array<char,100> data{};
};

struct DataWithSharedData {
    DataWithSharedData() {
      d = new DataWithSharedDataData{};
    }

    QSharedDataPointer<DataWithSharedDataData> d;
};

static void BM_NormalPOCOPropertyAccess(benchmark::State &state) {
  DataNormal normal;
  for (auto _: state)
    benchmark::DoNotOptimize(normal.data);
}

BENCHMARK(BM_NormalPOCOPropertyAccess);

static void BM_SharedDataAccess(benchmark::State &state) {
  DataWithSharedData data{};
  for (auto _: state)
    benchmark::DoNotOptimize(data.d->data);
}

BENCHMARK(BM_SharedDataAccess);

static void BM_CopyNormalPOCO(benchmark::State& state) {
  DataNormal normal;
  for (auto _: state){
    DataNormal copy = normal;
    benchmark::DoNotOptimize(copy);
  }
}
BENCHMARK(BM_CopyNormalPOCO);

static void BM_CopySharedData(benchmark::State& state) {
  DataWithSharedData normal;
  for (auto _: state){
    DataWithSharedData copy = normal;
    benchmark::DoNotOptimize(copy);
  }
}
BENCHMARK(BM_CopySharedData);
BENCHMARK_MAIN();