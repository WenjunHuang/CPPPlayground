//
// Created by HUANG WEN JUN on 2021/9/20.
//

#ifndef CPPPLAYGROUND_BIDI_ITERATOR_ICU_H
#define CPPPLAYGROUND_BIDI_ITERATOR_ICU_H
#include <skia/core/SkSpan.h>
#include <skia/core/SkString.h>
#include <skia/core/SkTypes.h>
#include <unicode/ubidi.h>
#include <unicode/ubrk.h>
#include <unicode/ucnv.h>
#include <unicode/uscript.h>
#include <unicode/ustring.h>
#include <unicode/utext.h>
#include <unicode/utf16.h>
#include <unicode/utf8.h>
#include <unicode/utypes.h>
#include <memory>
#include "unicode.h"

namespace {
using UnicodeBidi =
    std::unique_ptr<UBiDi,
                    SkFunctionWrapper<decltype(ubidi_close), ubidi_close>>;
using Converter =
    std::unique_ptr<UConverter,
                    SkFunctionWrapper<decltype(ucnv_close), ucnv_close>>;
}  // namespace

class BidiIterator_icu : public BidiIterator {
 public:
  explicit BidiIterator_icu(UnicodeBidi&& bidi) : fBidi(std::move(bidi)) {}
  Position getLength() override { return ubidi_getLength(fBidi.get()); }
  Level getLevelAt(Position position) override {
    return ubidi_getLevelAt(fBidi.get(), position);
  }

  static std::unique_ptr<BidiIterator> makeBidiIterator(const char16_t utf16[],
                                                        int utf16Units,
                                                        Direction dir) {
    UErrorCode status = U_ZERO_ERROR;
    UnicodeBidi bidi(ubidi_openSized(utf16Units, 0, &status));
    if (U_FAILURE(status)) {
      SkDEBUGF("Bidi error: %s", u_errorName(status));
      return nullptr;
    }
    SkASSERT(bidi);
    uint8_t bidiLevel = (dir == BidiIterator::kLTR) ? UBIDI_LTR : UBIDI_RTL;
    ubidi_setPara(bidi.get(), (const UChar*)utf16, utf16Units, bidiLevel,
                  nullptr, &status);
    if (U_FAILURE(status)) {
      SkDEBUGF("Bidi error: %s", u_errorName(status));
      return nullptr;
    }
    return std::unique_ptr<BidiIterator>(new BidiIterator_icu(std::move(bidi)));
  }

  static std::unique_ptr<BidiIterator> makeBidiIterator(const char utf8[],
                                                        int utf8Units,
                                                        Direction direction) {
    // Convert utf8 into utf16 since ubidi only accepts utf16
    if (!SkTFitsIn<int32_t>(utf8Units)) {
      SkDEBUGF("Bidi error: text too long");
      return nullptr;
    }
    UErrorCode status = U_ZERO_ERROR;
    Converter converter(ucnv_open("utf-8", &status));
    if (U_FAILURE(status)) {
      SkDEBUGF("Bidi error: can not create utf8 converter: %s",
               u_errorName(status));
      return nullptr;
    }
    icu::UnicodeString unicodeStr(utf8, utf8Units, converter.get(), status);
    if (U_FAILURE(status)) {
      SkDEBUGF("Bidi error: can not create unicode string from utf-8 string:%s",
               u_errorName(status));
      return nullptr;
    }

    return makeBidiIterator(unicodeStr.getBuffer(), unicodeStr.length(),
                            direction);
  }

  // This method returns the final results only: a list of bidi regions
  static std::vector<Region> getBidiRegions(const char utf8[],
                                            int utf8Units,
                                            Direction direction) {
    auto bidiIterator = makeBidiIterator(utf8, utf8Units, direction);
    std::vector<Region> bidiRegions;
    const char* start8 = utf8;
    const char* end8 = utf8 + utf8Units;
    BidiIterator::Level currentLevel = 0;

    Position pos8 = 0;
    Position pos16 = 0;
    Position end16 = bidiIterator->getLength();

    while (pos16 < end16) {
      auto level = bidiIterator->getLevelAt(pos16);
      if (pos16 == 0) {
        currentLevel = level;
      } else if (level != currentLevel) {
        auto end = start8 - utf8;
        bidiRegions.emplace_back(pos8, end, currentLevel);
        currentLevel = level;
        pos8 = end;
      }
      int i = 0;
      SkUnichar u;
      U8_NEXT(start8, i, end8 - start8, u);
      start8 += i;
      pos16 += U16_LENGTH(u);
    }
    auto end = start8 - utf8;
    if (end != pos8) {
      bidiRegions.emplace_back(pos8,end,currentLevel);
    }
    return bidiRegions;
  }

 private:
  UnicodeBidi fBidi;
};

#endif  // CPPPLAYGROUND_BIDI_ITERATOR_ICU_H
