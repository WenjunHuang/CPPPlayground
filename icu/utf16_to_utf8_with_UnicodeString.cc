//
// Created by rick on 2021/9/18.
//

#include <fmt/format.h>
#include <unicode/ucnv.h>
#include <unicode/unistr.h>
#ifdef _WIN32
#include <windows.h>
#endif
#include <iostream>
#include "util.h"
int main() {
#ifdef _WIN32
  SetConsoleOutputCP(CP_UTF8);
#endif
  // convert string
  char16_t utf16[] = u"这是一串abc字符串， \U0001F600 \u0671\u0679";
  icu::UnicodeString unicodeString(utf16, -1);
  std::string toPrint;
  unicodeString.toUTF8String(toPrint);
  fmt::print("origin string: {}\n", toPrint);

  // convert utf16 char to utf8 char
  UErrorCode status = U_ZERO_ERROR;
  auto utf8Converter = std::unique_ptr<UConverter, FunctionWrapper<ucnv_close>>(
      ucnv_open("utf8", &status));
  if (U_FAILURE(status)) {
    fmt::print("error {}", u_errorName(status));
    return -1;
  }
  const char utf8[] = u8"这是一串abc字符串， \U0001F600 \u0671\u0679";
  icu::UnicodeString fromUtf8(utf8, -1, utf8Converter.get(), status);
  if (U_FAILURE(status)) {
    fmt::print("error {}", u_errorName(status));
    return -1;
  }
  assert(std::memcmp(utf16, fromUtf8.getBuffer(), sizeof(utf16)) == 0);
  toPrint.clear();
  fromUtf8.toUTF8String(toPrint);
  fmt::print("converted str: {}", toPrint);

  // visual map
}
