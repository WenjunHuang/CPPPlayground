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
bool extractBidi(const char utf8[],
                 int utf8Units,
                 Unicode::TextDirection dir,
                 std::vector<Unicode::BidiRegion>* bidiRegions) {
  UErrorCode status = U_ZERO_ERROR;
  ConverterPtr converter(ucnv_open("utf-8", &status));
  if(U_FAILURE(status))
    return false;
  icu::UnicodeString unicodeString(utf8, utf8Units, converter.get(), status);
  if(U_FAILURE(status))
    return false;

  // Create bidi iterator
  UnicodeBidi bidi(ubidi_openSized(unicodeString.length(),0,&status));
  if (U_FAILURE(status)){
    SkDEBUGF("Bidi error: %s", u_errorName(status));
    return false;
  }
  SkASSERT(bidi);
  auto bidiLevel = (dir == Unicode::TextDirection::kLTR)?UBIDI_LTR:UBIDI_RTL;
  ubidi_setPara(bidi.get(),)



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
  return false;
}

void BidiIterator::ReorderVisual(const BidiIterator::Level* runLevels,
                                 int levelsCount,
                                 int32_t* logicalFromVisual) {
  ubidi_reorderVisual(runLevels, levelsCount, logicalFromVisual);
}
