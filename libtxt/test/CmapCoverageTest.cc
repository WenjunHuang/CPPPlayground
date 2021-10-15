//
// Created by rick on 2021/10/15.
//
#include <gtest/gtest.h>
#include <vector>

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

  head = writeU16(segmentCount * 2, out.data(), head);
  head = writeU16(searchRange, out.data(), head);
  head = writeU16(0, out.data(), head);
}

class CmapBuilder {
 public:
  static constexpr size_t kEncodingTableHead = 4;
  static constexpr size_t kEncodingTableSize = 8;

  CmapBuilder(int numTables) : num_tables_(numTables), current_table_index_(0) {
    const size_t headerSize =
        2 /* version */ + 2 /* numTables */ + kEncodingTableSize * num_tables_;
    out_.resize(headerSize);
    writeU16(0, out_.data(), 0);
    writeU16(num_tables_, out_.data(), 2);
  }

 private:
  int num_tables_;
  int current_table_index_;
  std::vector<uint8_t> out_;
};
TEST(CmapCoverageTest, SingleFormat4_brokenCmap) {}