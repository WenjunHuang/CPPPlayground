//
// Created by rick on 2021/9/12.
//

#include "break_iterator_scene.h"
#include <skia/core/SkFont.h>
#include <skia/core/SkTextBlob.h>
#include <unicode/ubrk.h>
#include <string>

void DrawUnicode(SkCanvas* canvas,std::u16string& str) {
}
void DrawCharacter(SkCanvas* canvas, int height, const std::u16string& text) {
  UErrorCode status = U_ZERO_ERROR;
  UBreakIterator* instance = ubrk_open(UBreakIteratorType::UBRK_CHARACTER,
                                       uloc_getDefault(), nullptr, 0, &status);
  ubrk_setText(instance, text.data(), text.length(), &status);
  auto start = ubrk_first(instance);
  while (true){
    auto end = ubrk_next(instance);
    if (end == -1)
      break;

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
  std::u16string text =
      u"🐉☺️❤️👮🏿👨‍👩‍👧‍👦🚵🏼‍♀️🇷🇺🏴󠁧󠁢󠁥󠁮"
      "󠁧"
      "󠁿"
      "*"
      "️⃣ǍǍZ̵̡̢͇͓͎͖͎̪͑͜ͅͅबिक्";
  SkTextBlob::MakeFromString()
}
