//
// Created by rick on 2021/9/18.
//

#include "unicode.h"
#include <unicode/ucnv.h>
#include <unicode/unistr.h>
#include "bidi_iterator_icu.h"
#include "break_iterator_icu.h"
#include "script_iterator_icu.h"

namespace {
using ConverterPtr =
    std::unique_ptr<UConverter,
                    SkFunctionWrapper<decltype(ucnv_close), ucnv_close>>;

bool extractWords(const char16_t utf16[],
                  int utf16Units,
                  std::vector<Unicode::Position>* results) {
  UErrorCode status = U_ZERO_ERROR;
  UText localText = UTEXT_INITIALIZER;
  ICUUText text(utext_openUChars(&localText, utf16, utf16Units, &status));
  if (U_FAILURE(status)) {
    SkDEBUGF("Break error: %s", u_errorName(status));
    return false;
  }

  ICUBreakIterator iterator = IcuBreakIteratorCache::get().makeBreakIterator(
      Unicode::BreakType::kWords);
  if (!iterator) {
    SkDEBUGF("Break error: cant create iterator");
    return false;
  }
  SkASSERT(iterator);

  ubrk_setUText(iterator.get(), text.get(), &status);
  if (U_FAILURE(status)) {
    SkDEBUGF("Break error: %s", u_errorName(status));
    return false;
  }

  // Get the words
  int32_t pos = ubrk_first(iterator.get());
  while (pos != UBRK_DONE) {
    results->emplace_back(pos);
    pos = ubrk_next(iterator.get());
  }
  return true;
}

bool extractBidi(const char utf8[],
                 int utf8Units,
                 Unicode::TextDirection dir,
                 std::vector<Unicode::BidiRegion>* bidiRegions) {
  UErrorCode status = U_ZERO_ERROR;
  auto utf16 = std::make_unique<char16_t[]>(utf8Units + 1);
  int32_t utf16Units = 0;
  u_strFromUTF8(utf16.get(), utf8Units + 1, &utf16Units, utf8, utf8Units,
                &status);
  if (U_FAILURE(status)) {
    SkDEBUGF("Failed to convert to utf16: %s", u_errorName(status));
    return false;
  }

  // Create bidi iterator
  UnicodeBidi bidi(ubidi_openSized(utf16Units, 0, &status));
  if (U_FAILURE(status)) {
    SkDEBUGF("Bidi error: %s", u_errorName(status));
    return false;
  }
  SkASSERT(bidi);
  auto bidiLevel =
      (dir == Unicode::TextDirection::kLTR) ? UBIDI_LTR : UBIDI_RTL;
  ubidi_setPara(bidi.get(), utf16.get(), utf16Units, bidiLevel, nullptr,
                &status);
  if (U_FAILURE(status)) {
    SkDEBUGF("Bidi error: %s", u_errorName(status));
    return false;
  }

  // Iterate through bidi regions and the result positions into utf8
  Unicode::Position posStart8 = 0;
  Unicode::Position pos8 = 0;
  const char* end8 = utf8 + utf8Units;
  Unicode::BidiLevel currentLevel = 0;

  Unicode::Position pos16 = 0;
  Unicode::Position end16 = ubidi_getLength(bidi.get());
  while (pos16 < end16) {
    auto level = ubidi_getLevelAt(bidi.get(), pos16);
    if (pos16 == 0) {
      currentLevel = level;
    } else if (level != currentLevel) {
      bidiRegions->emplace_back(posStart8, pos8, currentLevel);
      posStart8 = pos8;
    }
    UChar32 u;
    U8_NEXT(utf8, pos8, utf8Units, u);
    pos16 += U16_LENGTH(u);
  }
  return true;
}

bool isHardLineBreak(UChar32 codePoint) {
  auto property = u_getIntPropertyValue(codePoint, UCHAR_LINE_BREAK);
  return property == U_LB_LINE_FEED || property == U_LB_MANDATORY_BREAK;
}

bool extractPositions(const char utf8[],
                      int utf8Units,
                      Unicode::BreakType type,
                      std::function<void(int, int)> setBreak) {
  UErrorCode status = U_ZERO_ERROR;
  UText t = UTEXT_INITIALIZER;
  ICUUText text(utext_openUTF8(&t, utf8, utf8Units, &status));

  if (U_FAILURE(status)) {
    SkDEBUGF("Break error: %s", u_errorName(status));
    return false;
  }
  SkASSERT(text);

  ICUBreakIterator iterator =
      IcuBreakIteratorCache::get().makeBreakIterator(type);
  if (!iterator) {
    return false;
  }

  ubrk_setUText(iterator.get(), text.get(), &status);
  if (U_FAILURE(status)) {
    SkDEBUGF("Break error: %s", u_errorName(status));
    return false;
  }

  int32_t pos = ubrk_first(iterator.get());
  while (pos != UBRK_DONE) {
    auto status = type == Unicode::BreakType::kLines
                      ? UBRK_LINE_SOFT
                      : ubrk_getRuleStatus(iterator.get());
    setBreak(pos, status);
    pos = ubrk_next(iterator.get());
  }

  if (type == Unicode::BreakType::kLines) {
    // This is a workaround for
    // https://bugs.chromium.org/p/skia/issues/detail?id=10715 (ICU line break
    // iterator does not work correctly on Thai text with new lines) So, we only
    // use the iterator to collect soft line breaks and scan the text for all
    // hard line breaks ourselves
    const char* end = utf8 + utf8Units;
    int pos = 0;
    while (pos < utf8Units) {
      UChar32 unichar;
      U8_NEXT(utf8, pos, utf8Units, unichar);
      if (isHardLineBreak(unichar))
        setBreak(pos, UBRK_LINE_HARD);
    }
  }
  return true;
}
}  // namespace

std::unique_ptr<BidiIterator> Unicode::makeBidiIterator(
    const char16_t* text,
    int count,
    BidiIterator::Direction direction) {
  return BidiIterator_icu::makeBidiIterator(text, count, direction);
}
std::unique_ptr<BreakIterator> Unicode::makeBreakIterator(
    const char* locale,
    Unicode::BreakType breakType) {
  UErrorCode status = U_ZERO_ERROR;
  ICUBreakIterator iterator(
      ubrk_open(convertType(breakType), locale, nullptr, 0, &status));
  if (U_FAILURE(status)) {
    SkDEBUGF("Break error: %s", u_errorName(status));
    return nullptr;
  }
  return std::unique_ptr<BreakIterator>(
      new BreakIterator_icu(std::move(iterator)));
}

std::unique_ptr<BidiIterator> Unicode::makeBidiIterator(
    const char* text,
    int count,
    BidiIterator::Direction direction) {
  return BidiIterator_icu::makeBidiIterator(text, count, direction);
}
std::unique_ptr<BreakIterator> Unicode::makeBreakIterator(
    Unicode::BreakType type) {
  return makeBreakIterator(uloc_getDefault(), type);
}
std::unique_ptr<ScriptIterator> Unicode::makeScriptIterator() {
  return ScriptIterator_icu::makeScriptIterator();
}
bool Unicode::getBidiRegions(const char* utf8,
                             int utf8Units,
                             Unicode::TextDirection dir,
                             std::vector<BidiRegion>* results) {
  return extractBidi(utf8, utf8Units, dir, results);
}
bool Unicode::getLineBreaks(const char* utf8,
                            int utf8Units,
                            std::vector<LineBreakBefore>* results) {
  return extractPositions(
      utf8, utf8Units, BreakType::kLines, [results](int pos, int status) {
        results->emplace_back(pos, status == UBRK_LINE_HARD
                                       ? LineBreakType::kHardLineBreak
                                       : LineBreakType::kSoftLineBreak);
      });
}
bool Unicode::getWords(const char* utf8,
                       int utf8Units,
                       std::vector<Position>* results) {
  // Convert to UTF16 since we want the results in utf16
  auto utf16 = std::make_unique<char16_t[]>(utf8Units + 1);
  int32_t length = 0;
  UErrorCode status = U_ZERO_ERROR;
  u_strFromUTF8(utf16.get(), utf8Units + 1, &length, utf8, utf8Units, &status);
  if (U_FAILURE(status)) {
    SkDEBUGF("Error in convert to UTF16: %s", u_errorName(status));
    return false;
  }
  return extractWords(utf16.get(), length, results);
}
bool Unicode::getGraphemes(const char* utf8,
                           int utf8Units,
                           std::vector<Position>* results) {
  return extractPositions(
      utf8, utf8Units, BreakType::kGraphemes,
      [results](int pos, int status) { results->emplace_back(pos); });
}
std::unique_ptr<Unicode> Unicode::Make() {
  return std::make_unique<Unicode>();
}

void Unicode::reorderVisual(const Unicode::BidiLevel runLevels[],
                   int levelsCount,
                   int32_t logicalFromVisual[]) {
  ubidi_reorderVisual(runLevels, levelsCount, logicalFromVisual);
}
void BidiIterator::ReorderVisual(const BidiIterator::Level* runLevels,
                                 int levelsCount,
                                 int32_t* logicalFromVisual) {
  ubidi_reorderVisual(runLevels, levelsCount, logicalFromVisual);
}
