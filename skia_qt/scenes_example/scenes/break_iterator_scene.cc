//
// Created by rick on 2021/9/12.
//

#include "break_iterator_scene.h"
#include <skia/core/SkFont.h>
#include <skia/core/SkTextBlob.h>
#include <unicode/ubrk.h>
#include <unicode/unistr.h>
#include <string>

void DrawUnicode(SkCanvas* canvas, const std::u16string& str) {
  icu::UnicodeString text(str.data());
}

void BreakIteratorScene::DrawCharacter(SkCanvas* canvas,
                                       int height,
                                       const std::u16string& text) {
  UErrorCode status = U_ZERO_ERROR;
  UBreakIterator* instance = ubrk_open(UBreakIteratorType::UBRK_CHARACTER,
                                       uloc_getDefault(), nullptr, 0, &status);
  ubrk_setText(instance, text.data(), text.length(), &status);
  auto start = ubrk_first(instance);
  while (true) {
    auto end = ubrk_next(instance);
    if (end == -1)
      break;
    //    DrawUnicode(canvas, text.substr(start, end));
    DrawSubString(canvas, text.substr(start, end), height);
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
  auto str = u"🐉☺️❤️👮🏿👨‍👩‍👧‍👦🚵🏼‍♀️🇷🇺🏴󠁧󠁢󠁥󠁮󠁧󠁿*️⃣ǍǍZ̵̡̢͇͓͎͖͎̪͑͜ͅͅबिक्";

  std::u16string text = str;

  DrawCharacter(canvas, height, text);
  //  SkTextBlob::MakeFromString()
}
void BreakIteratorScene::DrawSubString(SkCanvas* canvas,
                                       const std::u16string& str,
                                       int height) {
  auto char_size = sizeof(std::u16string::value_type);
  auto size = str.size() * char_size;
  auto text = SkTextBlob::MakeFromText(str.c_str(),size,inter13,SkTextEncoding::kUTF16);
  canvas->drawTextBlob(text,x_,y_,blackFill);
  //  canvas->drawSimpleText(str.data(),
  //                         str.size() * sizeof(std::u16string::size_type),
//                         SkTextEncoding::kUTF16, x_, y_, inter13, blackFill);
  y_ += 20;
  if (y_ + 20 > height - 20) {
    x_ += 100;
    y_ = 20;
  }
  //  SkTextBlob::Make(str.data(),)
}
