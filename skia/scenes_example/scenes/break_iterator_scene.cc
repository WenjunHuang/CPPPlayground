//
// Created by rick on 2021/9/12.
//

#include "break_iterator_scene.h"
#include <fmt/core.h>
#include <fmt/ranges.h>
#include <skia/core/SkFont.h>
#include <skia/core/SkTextBlob.h>
#include <unicode/ubrk.h>
#include <unicode/unistr.h>
#include <iomanip>
#include <iostream>
#include <range/v3/range.hpp>
#include <string>

void BreakIteratorScene::DrawUnicode(
    SkCanvas* canvas,
    const std::vector<std::string>& code_points) {
  auto str = fmt::format("{}", fmt::join(code_points, " "));
  auto char_size = sizeof(std::string::value_type);
  auto size = str.size() * char_size;
  auto text = SkTextBlob::MakeFromText(str.c_str(), size, inter13,
                                       SkTextEncoding::kUTF8);
  canvas->drawTextBlob(text, x_ + 50, y_, blackFill);
}

void BreakIteratorScene::DrawCharacter(SkCanvas* canvas,
                                       int height,
                                       const std::string& utf8_string) {
  UErrorCode status = U_ZERO_ERROR;
  auto ut =
      utext_openUTF8(nullptr, utf8_string.c_str(), utf8_string.size(), &status);
  auto bi = ubrk_open(UBreakIteratorType::UBRK_WORD, uloc_getDefault(), nullptr,
                      0, &status);

  ubrk_setUText(bi, ut, &status);
  auto start = ubrk_first(bi);
  while (true) {
    auto end = ubrk_next(bi);
    if (end == -1)
      break;
    auto str = utf8_string.substr(start, end);
    auto sub_str = utext_openUTF8(nullptr, str.c_str(), str.size(), &status);
    UChar32 u;
    std::vector<std::string> code_points;
    while ((u = utext_next32(sub_str)) != U_SENTINEL) {
      code_points.push_back(fmt::format("U+{0:05x}", u));
    }
    DrawUnicode(canvas, code_points);
    DrawSubString(canvas, str, height);
    start = end;
  }
}
BreakIteratorScene::BreakIteratorScene() {
  mono11_.setTypeface(jbMono);
  mono11_.setSize(11);
}
void BreakIteratorScene::draw(SkCanvas* canvas,
                              int width,
                              int height,
                              float scale,
                              int xpos,
                              int ypos) {
  x_ = 20;
  y_ = 60;
  auto str = "🐉☺️";

  std::string text = str;

  DrawCharacter(canvas, height, text);
  //  SkTextBlob::MakeFromString()
}
void BreakIteratorScene::DrawSubString(SkCanvas* canvas,
                                       const std::string& str,
                                       int height) {
  auto char_size = sizeof(std::string::value_type);
  auto size = str.size() * char_size;
  auto text = SkTextBlob::MakeFromText(str.c_str(), size, inter13,
                                       SkTextEncoding::kUTF8);
  canvas->drawTextBlob(text, x_, y_, blackFill);
  //  canvas->drawSimpleText(str.data(),
  //                         str.size() * sizeof(std::u16string::size_type),
  //                         SkTextEncoding::kUTF16, x_, y_, inter13,
  //                         blackFill);
  y_ += 20;
  if (y_ + 20 > height - 20) {
    x_ += 100;
    y_ = 20;
  }
  //  SkTextBlob::Make(str.data(),)
}
