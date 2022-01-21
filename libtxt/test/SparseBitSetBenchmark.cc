//
// Created by rick on 2021/11/1.
//
#include <benchmark/benchmark.h>
#include <iostream>
#include <random>
#include <string>
#include <vector>
#include "minikin/SparseBitSet.h"
using namespace minikin;
class SparseDataFixture : public benchmark::Fixture {
 public:
  void SetUp(const ::benchmark::State& state) {
    const uint32_t kTestRangeNum = 4096;

    std::mt19937 mt;  // Fix seeds to be able to reproduce the result.
    std::uniform_int_distribution<uint16_t> distribution(1, 512);
    ranges_.reserve(distribution(mt));
    for (size_t i = 1; i < kTestRangeNum * 2; ++i) {
      ranges_.push_back((ranges_.back() - 1) + distribution(mt));
    }
  }

  void TearDown(const ::benchmark::State& state) {}
  std::vector<uint32_t> ranges_;
};

BENCHMARK_F(SparseDataFixture, initFromRangesTest)(benchmark::State& state) {
  while (state.KeepRunning()) {
    SparseBitSet bs(ranges_.data(), ranges_.size() / 2);
  }
}

BENCHMARK_MAIN();