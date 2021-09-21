//
// Created by HUANG WEN JUN on 2021/9/20.
//

#ifndef CPPPLAYGROUND_BREAK_ITERATOR_ICU_H
#define CPPPLAYGROUND_BREAK_ITERATOR_ICU_H
#include <map>
#include <mutex>
#include "unicode.h"
namespace {
using ICUBreakIterator =
    std::unique_ptr<UBreakIterator,
                    SkFunctionWrapper<decltype(ubrk_close), ubrk_close>>;
using ICUUText =
    std::unique_ptr<UText,
                    SkFunctionWrapper<decltype(utext_close), utext_close>>;

UBreakIteratorType convertType(Unicode::BreakType type) {
  switch (type) {
    case Unicode::BreakType::kLines:
      return UBRK_LINE;
    case Unicode::BreakType::kGraphemes:
      return UBRK_CHARACTER;
    case Unicode::BreakType::kWords:
      return UBRK_WORD;
    default:
      return UBRK_CHARACTER;
  }
}
}  // namespace

class BreakIterator_icu : public BreakIterator {
 public:
  explicit BreakIterator_icu(ICUBreakIterator&& iterator)
      : fBreakIterator(std::move(iterator)), fLastResult(0) {}
  Position first() override {
    return fLastResult = ubrk_first(fBreakIterator.get());
  }
  Position current() override {
    return fLastResult = ubrk_current(fBreakIterator.get());
  }
  Position next() override {
    return fLastResult = ubrk_next(fBreakIterator.get());
  }
  Position preceding(Position offset) override {
    return fLastResult = ubrk_preceding(fBreakIterator.get(), offset);
  }
  Position following(Position offset) override {
    return fLastResult = ubrk_following(fBreakIterator.get(), offset);
  }
  Status status() override { return ubrk_getRuleStatus(fBreakIterator.get()); }
  bool isDone() override { return fLastResult == UBRK_DONE; }
  bool setText(const char* utfText8, int utf8Units) override {
    UErrorCode status = U_ZERO_ERROR;
    ICUUText text(utext_openUTF8(nullptr, utfText8, utf8Units, &status));
    if (U_FAILURE(status)) {
      SkDEBUGF("Break error: %s", u_errorName(status));
      return false;
    }
    SkASSERT(text);
    ubrk_setUText(fBreakIterator.get(), text.get(), &status);
    if (U_FAILURE(status)) {
      SkDEBUGF("Break error: %s", u_errorName(status));
      return false;
    }

    fLastResult = 0;
    return true;
  }

  bool setText(const char16_t* utfText16, int utf16Units) override {
    UErrorCode status = U_ZERO_ERROR;
    ICUUText text(utext_openUChars(nullptr, (const UChar*)utf16Units,
                                   utf16Units, &status));
    if (U_FAILURE(status)) {
      SkDEBUGF("Break error: %s", u_errorName(status));
      return false;
    }
    SkASSERT(text);
    ubrk_setUText(fBreakIterator.get(), text.get(), &status);
    if (U_FAILURE(status)) {
      SkDEBUGF("Break error: %s", u_errorName(status));
      return false;
    }
    fLastResult = 0;
    return false;
  }

 private:
  ICUBreakIterator fBreakIterator;
  Position fLastResult;
};

class IcuBreakIteratorCache {
 public:
  static IcuBreakIteratorCache& get() {
    static IcuBreakIteratorCache instance;
    return instance;
  }

  ICUBreakIterator makeBreakIterator(Unicode::BreakType type) {
    UErrorCode status = U_ZERO_ERROR;
    ICUBreakIterator* cacheIterator;
    {
      std::lock_guard lock(fBreakCacheMutex);
      auto itr = fBreakCache.find(type);
      if (itr == fBreakCache.end()) {
        ICUBreakIterator newIterator(ubrk_open(convertType(type),uloc_getDefault(),
                                               nullptr,0,&status));
        if (U_FAILURE(status)){
          SkDEBUGF("Break error: %s",u_errorName(status));
        } else {

          fBreakCache[type] = std::move(newIterator);
          cacheIterator = &fBreakCache[type];
        }
      }
    }
    ICUBreakIterator  iterator;
    if (cacheIterator) {
      iterator.reset(ubrk_clone(cacheIterator->get(),&status));
      if (U_FAILURE(status)){
        SkDEBUGF("Break error: %s",u_errorName(status));
      }
    }
    return iterator;
  }

 private:
  std::map<Unicode::BreakType, ICUBreakIterator> fBreakCache;
  std::mutex fBreakCacheMutex;
};
#endif  // CPPPLAYGROUND_BREAK_ITERATOR_ICU_H
