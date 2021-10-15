//
// Created by rick on 2021/10/11.
//
#include "minikin_internal.h"
#include "log/log.h"
namespace minikin {
std::recursive_mutex gMinikinLock;
void assertMinikinLocked() {
#ifdef ENABLE_RACE_DETECTION
  LOG_ALWAYS_FATAL_IF(gMinikinLock.try_lock() == false);
#endif
}
hb_blob_t* getFontTable(const MinikinFont* minikinFont, uint32_t tag) {
  assertMinikinLocked();
  hb_font_t* font = getHbFontLocked(minikinFont);
  hb_face_t* face = hb_font_get_face(font);
  hb_blob_t* blob = hb_face_reference_table(face,tag);
  hb_font_destroy(font);
  return blob;
}
}  // namespace minikin
