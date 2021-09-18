//
// Created by rick on 2021/9/18.
//

#include "unicode.h"

namespace {
using UnicodeBidi =
    std::unique_ptr<UBiDi,
                    SkFunctionWrapper<decltype(ubidi_close), ubidi_close>>;
}
class BidiIterator_icu : public BidiIterator {
 public:
  explicit BidiIterator_icu(UnicodeBidi&& bidi) : fBidi(std::move(bidi)) {}
  Position getLength() override { return ubidi_getLength(fBidi.get()); }
  Level getLevelAt(Position position) override {
    return ubidi_getLevelAt(fBidi.get(), position);
  }

  static std::unique_ptr<BidiIterator> makeBidiIterator(const uint16_t utf16[],
                                                        int utf16Units,
                                                        Direction dir) {
    UErrorCode status = U_ZERO_ERROR;
    UnicodeBidi bidi(ubidi_openSized(utf16Units,0,&status));
    if (U_FAILURE(status)) {
      SkDEBUGF("Bidi error: %s",u_errorName(status));
      return nullptr;
    }
    SkASSERT(bidi);
    uint8_t bidiLevel = (dir == BidiIterator::kLTR) ? UBIDI_LTR:UBIDI_RTL;
    ubidi_setPara(bidi.get(),(const UChar*)utf16,utf16Units,bidiLevel,nullptr,&status);
  }

 private:
  UnicodeBidi fBidi;
};
std::unique_ptr<BidiIterator> Unicode::makeBidiIterator(
    const uint16_t* text,
    int count,
    BidiIterator::Direction) {
  return std::unique_ptr<BidiIterator>();
}
