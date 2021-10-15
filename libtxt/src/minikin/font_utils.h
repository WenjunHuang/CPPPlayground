//
// Created by rick on 2021/10/14.
//

#pragma once
#include <unordered_set>
namespace minikin {
bool analyzeStyle(const uint8_t* os2_data,
                  size_t os2_size,
                  int* weight,
                  bool* italic);

void analyzeAxes(const uint8_t* fvar_data,
                 size_t fvar_size,
                 std::unordered_set<uint32_t>* axes);

}