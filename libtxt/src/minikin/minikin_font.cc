//
// Created by rick on 2021/10/11.
//

#include "minikin_font.h"
#include "minikin_internal.h"
namespace minikin {
MinikinFont::~MinikinFont() {
  std::scoped_lock guard(gMinikinLock);
  purgeHbFontLocked(this);

}
}
