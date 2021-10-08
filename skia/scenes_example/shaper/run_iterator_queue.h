//
// Created by rick on 2021/9/27.
//

#pragma once
#include <range/v3/algorithm.hpp>
#include "shaper.h"

using namespace ranges;

class RunIteratorQueue {
 public:
  void inset(Shaper::RunIterator* runIterator, int priority) {
    fEntries.insert({runIterator, priority});
  }

  bool advanceRuns() {
    auto leastRun = (*fEntries.begin()).runIterator;
    if (leastRun->atEnd()) {
      SkASSERT(this->allRunsAreAtEnd());
      return false;
    }

    const auto leastEnd = leastRun->endOfCurrentRun();
    Shaper::RunIterator* currentRun = nullptr;
    SkDEBUGCODE(size_t previousEndOfCurrentRun);
    while (
        (currentRun = std::begin(fEntries)->runIterator)->endOfCurrentRun() <=
        leastEnd) {
      auto priority = std::begin(fEntries)->priority;
      fEntries.erase(std::begin(fEntries));

      SkDEBUGCODE(previousEndOfCurrentRun = currentRun->endOfCurrentRun());
      currentRun->consume();
      SkASSERT(previousEndOfCurrentRun < currentRun->endOfCurrentRun());
      fEntries.insert({currentRun, priority});
    }
    return true;
  }

  size_t endOfCurrentRun() const {
    return std::begin(fEntries)->runIterator->endOfCurrentRun();
  }

 private:
  bool allRunsAreAtEnd() const {
    return all_of(fEntries,
                  [](const auto& entry) { return entry.runIterator->atEnd(); });
  }

  struct Entry {
    Shaper::RunIterator* runIterator;
    int priority;
    bool operator<(const Entry& other) const {
      auto myEnd = runIterator->endOfCurrentRun();
      auto otherEnd = other.runIterator->endOfCurrentRun();
      return myEnd < otherEnd ||
             (myEnd == otherEnd && priority < other.priority);
    }
  };
  std::set<Entry> fEntries;
};
