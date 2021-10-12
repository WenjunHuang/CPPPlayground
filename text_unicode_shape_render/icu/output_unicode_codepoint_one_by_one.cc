//
// Created by rick on 2021/9/17.
//
#include <fmt/format.h>
#include <unicode/unistr.h>
#ifdef _WIN32
#include <windows.h>
#endif
#include <iostream>

uint32_t utf8_next(const char* begin, const char* end) {
  uint32_t val;
  int i;
  U8_NEXT(begin, i, end - begin, val);
  return val;
}
int main() {
#ifdef _WIN32
  SetConsoleOutputCP(CP_UTF8);
#endif
  char str[] = u8"中文abc\U0001F6CC";
  int length = strlen(str);
  int i = 0;
  uint32_t val;
  while (i < length) {
    U8_NEXT(str, i, length, val);
    std::cout << fmt::format("code point U+{:x},offset: {}", val, i) << std::endl;
  }
  //  std::cout << "\"u\" with two dots on top: \xc3\xbc\n";
  //  std::cout << "chinese glyph for \"world\": \xe5\x80\xbc\n";
  //  std::cout << "smiling emoji: \xf0\x9f\x98\x80\n";
  return 0;
}