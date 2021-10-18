//
// Created by rick on 2021/10/15.
//
#include <gtest/gtest.h>
#include <vector>
#include "log/log.h"
#include "utils/WindowsUtils.h"
#include "minikin/SparseBitSet.h"
#include "minikin/cmap_coverage.h"

namespace minikin {
static size_t writeU16(uint16_t x, uint8_t* out, size_t offset) {
  out[offset] = x >> 8;
  out[offset + 1] = x;
  return offset + 2;
}

static size_t writeI16(int16_t sx, uint8_t* out, size_t offset) {
  return writeU16(static_cast<uint16_t>(sx), out, offset);
}

size_t writeU32(uint32_t x, uint8_t* out, size_t offset) {
  out[offset] = x >> 24;
  out[offset + 1] = x >> 16;
  out[offset + 2] = x >> 8;
  out[offset + 3] = x;
  return offset + 4;
}

// Returns valid cmap format 4 table contents. All glyph ID is same value as
// code point. (e.g. 'a' (U+0061) is mapped to Glyph ID = 0x0061). 'range'
// should be specified with inclusive-inclusive values.
static std::vector<uint8_t> buildCmapFormat4Table(
    const std::vector<uint16_t>& ranges) {
  uint16_t segmentCount = ranges.size() / 2 + 1 /* +1 for end marker */;

  const size_t numOfUint16 =
      8 /* format, length, languages, segCountX2, searchRange, entrySelector,
             rangeShift, pad */
      + segmentCount * 4 /* endCount, startCount, idRange, idRangeOffset */;
  const size_t finalLength = sizeof(uint16_t) * numOfUint16;

  std::vector<uint8_t> out(finalLength);
  size_t head = 0;
  head = writeU16(4, out.data(), head);            // format
  head = writeU16(finalLength, out.data(), head);  // length
  head = writeU16(0, out.data(), head);            // language

  const uint16_t searchRange =
      2 * (1 << static_cast<int>(floor(log2(segmentCount))));

  head = writeU16(segmentCount * 2, out.data(), head);  // segCountX2
  head = writeU16(searchRange, out.data(), head);       // searchRange
#if defined(_WIN32)
  head = writeU16(ctz_win(searchRange) - 1, out.data(), head);
#else
  head = writeU16(__builtin_ctz(searchRange) - 1, out.data(),
                  head);  // entrySelector
#endif
  head =
      writeU16(segmentCount * 2 - searchRange, out.data(), head);  // rangeShift

  size_t endCountHead = head;
  size_t startCountHead =
      head + segmentCount * sizeof(uint16_t) + 2 /* padding */;
  size_t idDeltaHead = startCountHead + segmentCount * sizeof(uint16_t);
  size_t idRangeOffsetHead = idDeltaHead + segmentCount * sizeof(uint16_t);

  for (size_t i = 0; i < ranges.size() / 2; ++i) {
    const uint16_t begin = ranges[i * 2];
    const uint16_t end = ranges[i * 2 + 1];
    startCountHead = writeU16(begin, out.data(), startCountHead);
    endCountHead = writeU16(end, out.data(), endCountHead);
    // map glyph ID as the same value of the code point.
    idDeltaHead = writeU16(0, out.data(), idDeltaHead);
    idRangeOffsetHead =
        writeU16(0 /* we don't use this */, out.data(), idRangeOffsetHead);
  }

  // fill end marker
  endCountHead = writeU16(0xFFFF, out.data(), endCountHead);
  startCountHead = writeU16(0xFFFF, out.data(), startCountHead);
  idDeltaHead = writeU16(1, out.data(), idDeltaHead);
  idRangeOffsetHead = writeU16(0, out.data(), idRangeOffsetHead);
  LOG_ALWAYS_FATAL_IF(endCountHead > finalLength);
  LOG_ALWAYS_FATAL_IF(startCountHead > finalLength);
  LOG_ALWAYS_FATAL_IF(idDeltaHead > finalLength);
  LOG_ALWAYS_FATAL_IF(idRangeOffsetHead != finalLength);
  return out;
}

// Returns valid cmap format 4 table contents. All glyph ID is same value as
// code point. (e.g. 'a' (U+0061) is mapped to Glyph ID = 0x0061). 'range'
// should be specified with inclusive-inclusive values.
static std::vector<uint8_t> buildCmapFormat12Table(
    const std::vector<uint32_t>& ranges) {
  uint32_t numGroups = ranges.size() / 2;

  const size_t finalLength =
      2 /* format */ + 2 /* reserved */ + 4 /* length */ + 4 /* languages */ +
      4 /* numGroups */ + 12 /* size of a group */ * numGroups;

  std::vector<uint8_t> out(finalLength);
  size_t head = 0;
  head = writeU16(12, out.data(), head);           // format
  head = writeU16(0, out.data(), head);            // reserved
  head = writeU32(finalLength, out.data(), head);  // length
  head = writeU32(0, out.data(), head);            // language
  head = writeU32(numGroups, out.data(), head);    // numGroups

  for (uint32_t i = 0; i < numGroups; ++i) {
    const uint32_t start = ranges[2 * i];
    const uint32_t end = ranges[2 * i + 1];
    head = writeU32(start, out.data(), head);
    head = writeU32(end, out.data(), head);
    // map glyph ID as the same value of the code point.
    // TODO: Use glyph IDs lower than 65535.
    // Cmap can store 32 bit glyph ID but due to the size of numGlyph, a font
    // file can contain up to 65535 glyphs in a file.
    head = writeU32(start, out.data(), head);
  }

  LOG_ALWAYS_FATAL_IF(head != finalLength);
  return out;
}

class CmapBuilder {
 public:
  static constexpr size_t kEncodingTableHead = 4;
  static constexpr size_t kEncodingTableSize = 8;

  CmapBuilder(int numTables) : mNumTables(numTables), mCurrentTableIndex(0) {
    const size_t headerSize =
        2 /* version */ + 2 /* numTables */ + kEncodingTableSize * numTables;
    out.resize(headerSize);
    writeU16(0, out.data(), 0);
    writeU16(numTables, out.data(), 2);
  }

  void appendTable(uint16_t platformId,
                   uint16_t encodingId,
                   const std::vector<uint8_t>& table) {
    appendEncodingTable(platformId, encodingId, out.size());
    out.insert(out.end(), table.begin(), table.end());
  }

  // TODO: Introduce Format 14 table builder.

  std::vector<uint8_t> build() {
    LOG_ALWAYS_FATAL_IF(mCurrentTableIndex != mNumTables);
    return out;
  }

  // Helper functions.
  static std::vector<uint8_t> buildSingleFormat4Cmap(
      uint16_t platformId,
      uint16_t encodingId,
      const std::vector<uint16_t>& ranges) {
    CmapBuilder builder(1);
    builder.appendTable(platformId, encodingId, buildCmapFormat4Table(ranges));
    return builder.build();
  }

  static std::vector<uint8_t> buildSingleFormat12Cmap(
      uint16_t platformId,
      uint16_t encodingId,
      const std::vector<uint32_t>& ranges) {
    CmapBuilder builder(1);
    builder.appendTable(platformId, encodingId, buildCmapFormat12Table(ranges));
    return builder.build();
  }

 private:
  void appendEncodingTable(uint16_t platformId,
                           uint16_t encodingId,
                           uint32_t offset) {
    LOG_ALWAYS_FATAL_IF(mCurrentTableIndex == mNumTables);

    const size_t currentEncodingTableHead =
        kEncodingTableHead + mCurrentTableIndex * kEncodingTableSize;
    size_t head = writeU16(platformId, out.data(), currentEncodingTableHead);
    head = writeU16(encodingId, out.data(), head);
    head = writeU32(offset, out.data(), head);
    LOG_ALWAYS_FATAL_IF((head - currentEncodingTableHead) !=
                        kEncodingTableSize);
    mCurrentTableIndex++;
  }

  int mNumTables;
  int mCurrentTableIndex;
  std::vector<uint8_t> out;
};
TEST(CmapCoverageTest, SingleFormat4_brokenCmap) {
  bool has_cmap_format_14_subtable = false;
  {
    SCOPED_TRACE("Reading beyond buffer size - Too small cmap size");
    std::vector<uint8_t> cmap = CmapBuilder::buildSingleFormat4Cmap(
        0, 0, std::vector<uint16_t>({'a', 'a'}));

    SparseBitSet coverage = CmapCoverage::getCoverage(
        cmap.data(),3 /* too small */,&has_cmap_format_14_subtable);
    EXPECT_EQ(0U,coverage.length());
    EXPECT_FALSE(has_cmap_format_14_subtable);
  }

  {
    SCOPED_TRACE(
        "Reading beyond buffer size - space needed for tables goes beyond cmap "
        "size");
    std::vector<uint8_t> cmap = CmapBuilder::buildSingleFormat4Cmap(
        0, 0, std::vector<uint16_t>({'a', 'a'}));

    writeU16(1000,cmap.data(),2/* offset of num tables in cmap header */);
  }
}
}