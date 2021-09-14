//
// Created by HUANG WEN JUN on 2021/9/13.
//
#include <unicode/brkiter.h>
#include <unicode/ubrk.h>
#include <unicode/utext.h>
#include <unicode/utypes.h>
#include <cassert>
#include <iomanip>
#include <iostream>

int GetCodePointsWithCAPI(const char* utf8_string) {
  UText* ut = nullptr;
  UBreakIterator* bi = nullptr;
  int word_count = 0;
  UErrorCode status = U_ZERO_ERROR;

  ut = utext_openUTF8(ut, utf8_string, -1, &status);
  bi = ubrk_open(UBRK_WORD, "en_us", nullptr, 0, &status);
  ubrk_setUText(bi, ut, &status);
  int start = 0, end = 0;
  while ((end = ubrk_next(bi)) != UBRK_DONE) {
    auto sub_str =
        utext_openUTF8(nullptr, utf8_string + start, end - start, &status);
    UChar32 code_point;
    while ((code_point = utext_next32(sub_str)) != U_SENTINEL) {
      std::cout << "U+" << std::setfill('0') << std::setw(5) << std::hex
                << code_point << " ";
    }
    utext_close(sub_str);
    std::cout << std::endl;
    word_count++;
    start = end;
  }
  utext_close(ut);
  ubrk_close(bi);
  assert(U_SUCCESS(status));
  return word_count;
}

size_t GetCodePointWithCPPAPI(const char* utf8_string) {
  using namespace icu;
  UnicodeString str(utf8_string);
  size_t word_count = 0;
  UErrorCode status;
  auto bi = std::unique_ptr<BreakIterator>(
      BreakIterator::createWordInstance("en_us", status));
  bi->setText(str);
  int start = bi->first();
  int end = start;
  while ((end = bi->next()) !=BreakIterator::DONE) {
    auto sub_str = str.tempSubStringBetween(start,end-start);
    auto sub_str_bi = std::unique_ptr<BreakIterator>(
        BreakIterator::createCharacterInstance("en_us", status));
    sub_str_bi->setText(sub_str);
    auto code_point = str.char32At(start);
    std::cout << "U+" << std::setfill('0') << std::setw(5) << std::hex
              << code_point << " ";
    start = end;
    word_count++;
    std::cout << std::endl;

  }
  return word_count;
}

int main() {
  std::cout << GetCodePointsWithCAPI(
                   "🐉☺️❤️👮🏿👨‍👩‍👧‍👦")
            << std::endl;
  std::cout << GetCodePointWithCPPAPI(
      "🐉☺️❤️👮🏿👨‍👩‍👧‍👦")
            << std::endl;
}