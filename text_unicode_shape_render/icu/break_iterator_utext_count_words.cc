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
  UText mytext = UTEXT_INITIALIZER;
  UText* ut = nullptr;
  UBreakIterator* bi = nullptr;
  int word_count = 0;
  UErrorCode status = U_ZERO_ERROR;

  ut = utext_openUTF8(&mytext, utf8_string, -1, &status);
  bi = ubrk_open(UBRK_WORD, "en_us", nullptr, 0, &status);
  ubrk_setUText(bi, ut, &status);
  int start = 0, end = 0;
  while ((end = ubrk_next(bi)) != UBRK_DONE) {
    auto sub_str =
        utext_openUTF8(nullptr, utf8_string + start, end - start, &status);
    UChar32 code_point;
    std::cout << std::string_view(utf8_string + start,end-start) << " ";
    std::cout << "rule status: " << ubrk_getRuleStatus(bi) << " ";
    while ((code_point = utext_next32(sub_str)) != U_SENTINEL) {
      std::cout << "U+" << std::setfill('0') << std::setw(5) << std::hex
                << code_point << " ";
    }
    utext_close(sub_str);
    std::cout << std::endl;
    word_count++;
    start = end;
  }
  ubrk_close(bi);
  utext_close(ut);
  assert(U_SUCCESS(status));
  return word_count;
}

size_t GetCodePointWithCPPAPI(const char* utf8_string) {
  using namespace icu;
  size_t word_count = 0;
  UErrorCode status;
  auto bi = std::unique_ptr<BreakIterator>(
      BreakIterator::createWordInstance("en_us", status));
  UText mytext = UTEXT_INITIALIZER;
  auto ut = utext_openUTF8(&mytext, utf8_string, -1, &status);

  bi->setText(&mytext,status);
  int start = bi->first();
  int end = start;
  while ((end = bi->next()) !=BreakIterator::DONE) {
    auto sub_str =
        utext_openUTF8(nullptr, utf8_string + start, end - start, &status);
    UChar32 code_point;
    std::cout << std::string_view(utf8_string + start,end-start) << " ";
    std::cout << "rule status: " << bi->getRuleStatus() << " ";
    while ((code_point = utext_next32(sub_str)) != U_SENTINEL) {
      std::cout << "U+" << std::setfill('0') << std::setw(5) << std::hex
                << code_point << " ";
    }
    utext_close(sub_str);
    std::cout << std::endl;
    word_count++;
    start = end;
  }
  return word_count;
}

int main() {
  std::cout << GetCodePointsWithCAPI(
                   "fuck 🐉☺️❤️👮🏿👨‍👩‍👧‍👦")
            << std::endl;
  std::cout << GetCodePointWithCPPAPI(
      "fuck 🐉☺️❤️👮🏿👨‍👩‍👧‍👦")
            << std::endl;
}