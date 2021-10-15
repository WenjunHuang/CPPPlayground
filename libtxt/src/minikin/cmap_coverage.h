//
// Created by rick on 2021/10/15.
//

#pragma once
#include "SparseBitSet.h"

namespace minikin {
class CmapCoverage {
 public:
  static SparseBitSet getCoverage(const uint8_t* cmap_data,
                                  size_t cmap_size,
                                  bool* has_cmap_format14_subtable);
};
}
