/*
 * Copyright (C) 2017 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <random>

#include <gtest/gtest.h>
#include <minikin/SparseBitSet.h>

namespace minikin {

TEST(SparseBitSetTest, calcNumPages) {
  std::vector<uint32_t> singleInFirstPage;
  singleInFirstPage.push_back(SparseBitSet::kPageMask / 5);
  singleInFirstPage.push_back(SparseBitSet::kPageMask / 3);
  EXPECT_EQ(1,SparseBitSet::calcNumPages(singleInFirstPage.data(),1));
  SparseBitSet sbs1(singleInFirstPage.data(),1);
  EXPECT_EQ(singleInFirstPage[0],sbs1.nextSetBit(0));

  std::vector<uint32_t> singleRangeFromBegin;
  singleRangeFromBegin.push_back(0 + SparseBitSet::kPageMask / 5);
  singleRangeFromBegin.push_back(SparseBitSet::kPageMask +
                                 SparseBitSet::kPageMask / 5);
  EXPECT_EQ(2, SparseBitSet::calcNumPages(singleRangeFromBegin.data(), 1));

  std::vector<uint32_t> singleRangeNotFromBegin;
  singleRangeNotFromBegin.push_back(SparseBitSet::kPageMask * 3 +
                                    SparseBitSet::kPageMask / 5);
  singleRangeNotFromBegin.push_back(SparseBitSet::kPageMask * 4 +
                                    SparseBitSet::kPageMask / 5);
  EXPECT_EQ(3, SparseBitSet::calcNumPages(singleRangeNotFromBegin.data(), 1));

  std::vector<uint32_t> twoRangesNotFromBeginAndNotContinue;
  twoRangesNotFromBeginAndNotContinue.push_back(SparseBitSet::kPageMask * 3 +
                                                SparseBitSet::kPageMask / 5);
  twoRangesNotFromBeginAndNotContinue.push_back(SparseBitSet::kPageMask * 4 +
                                                SparseBitSet::kPageMask / 5);
  twoRangesNotFromBeginAndNotContinue.push_back(SparseBitSet::kPageMask * 5 +
                                                SparseBitSet::kPageMask / 5);
  twoRangesNotFromBeginAndNotContinue.push_back(SparseBitSet::kPageMask * 6 +
                                                SparseBitSet::kPageMask / 5);
  EXPECT_EQ(5, SparseBitSet::calcNumPages(
                   twoRangesNotFromBeginAndNotContinue.data(), 2));
}

TEST(SparseBitSetTest, my) {
  std::vector<uint32_t> data;
  data.push_back(32);
  data.push_back(65);
  SparseBitSet sbs(data.data(),data.size()/2);
  for (auto ch=32;ch < 65;ch++){
    ASSERT_TRUE(sbs.get(ch)) << ch;
  }
}
TEST(SparseBitSetTest, randomTest) {
  const uint32_t kTestRangeNum = 4096;

  std::mt19937 mt;  // Fix seeds to be able to reproduce the result.
  std::uniform_int_distribution<uint16_t> distribution(1, 512);

  std::vector<uint32_t> range{distribution(mt)};
  for (size_t i = 1; i < kTestRangeNum * 2; ++i) {
    range.push_back((range.back() - 1) + distribution(mt));
  }

  SparseBitSet bitset(range.data(), range.size() / 2);

//  uint32_t ch = 0;
//  for (size_t i = 0; i < range.size() / 2; ++i) {
//    uint32_t start = range[i * 2];
//    uint32_t end = range[i * 2 + 1];
//
//    for (; ch < start; ch++) {
//      ASSERT_FALSE(bitset.get(ch)) << std::hex << ch;
//    }
//    for (; ch < end; ch++) {
//      ASSERT_TRUE(bitset.get(ch)) << std::hex << ch;
//    }
//  }
//  for (; ch < 0x1FFFFFF; ++ch) {
//    ASSERT_FALSE(bitset.get(ch)) << std::hex << ch;
//  }
}

}  // namespace minikin
