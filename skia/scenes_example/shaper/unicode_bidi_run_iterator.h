//
// Created by rick on 2021/9/22.
//

#pragma once
#include "shaper.h"

using UnicodeBidi = std::unique_ptr<BidiIterator>;
class UnicodeBidiRunIterator final : public Shaper::BiDiRunIterator {
 public:
  UnicodeBidiRunIterator(const char* utf8, const char* end, UnicodeBidi bidi)
      : fBidi(std::move(bidi)),
        fEndOfCurrentRun(utf8),
        fBegin(utf8),
        fEnd(end),
        fUTF16LogicalPosition(0),
        fLevel(BidiIterator::kLTR) {}
  void consume() override {
    SkASSERT(fUTF16LogicalPosition < fBidi->getLength());
    int32_t endPosition = fBidi->getLength();
    fLevel = fBidi->getLevelAt(fUTF16LogicalPosition);
    SkUnichar u;
    int i = 0;
    U8_NEXT_OR_FFFD(fEndOfCurrentRun, i, fEnd - fEndOfCurrentRun, u);
    fEndOfCurrentRun += i;
    fUTF16LogicalPosition += U16_LENGTH(u);

    BidiIterator::Level level;
    while (fUTF16LogicalPosition < endPosition) {
      level = fBidi->getLevelAt(fUTF16LogicalPosition);
      if (level != fLevel) {
        break;
      }
      i = 0;
      U8_NEXT_OR_FFFD(fEndOfCurrentRun, i, fEnd - fEndOfCurrentRun, u);
      fEndOfCurrentRun += i;
      fUTF16LogicalPosition += U16_LENGTH(u);
    }
  }
  size_t endOfCurrentRun() const override { return fEndOfCurrentRun - fBegin; }
  bool atEnd() const override {
    return fUTF16LogicalPosition == fBidi->getLength();
  }
  uint8_t currentLevel() const override { return fLevel; }

 private:
  UnicodeBidi fBidi;
  const char* fEndOfCurrentRun;
  const char* const fBegin;
  const char* const fEnd;
  int32_t fUTF16LogicalPosition;
  BidiIterator::Level fLevel;
};