//
// Created by rick on 2021/10/11.
//

#pragma once

struct hb_font_t;
namespace minikin {
class MinikinFont;

void purgeHbFontCacheLocked();
void purgeHbFontLocked(const MinikinFont* minkinFont);
hb_font_t* getHbFontLocked(const MinikinFont* minikinFont);
}