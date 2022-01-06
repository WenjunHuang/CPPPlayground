//
// Created by rick on 2021/10/11.
//

#include "hb_font_cache.h"
#include <harfbuzz/hb-ot.h>
#include <harfbuzz/hb.h>
#include "minikin_internal.h"
#include "utils/LruCache.h"
namespace minikin {

class HbFontCache : private android::OnEntryRemoved<int32_t, hb_font_t*> {
 public:
  HbFontCache() : cache_(kMaxEntries) {
    cache_.setOnEntryRemovedListener(this);
  }

  // callback for OnEntryRemoved
  void operator()(int32_t&, hb_font_t*& value) { hb_font_destroy(value); }

  hb_font_t* get(int32_t fontId) { return cache_.get(fontId); }

  void put(int32_t fontId, hb_font_t* font) { cache_.put(fontId, font); }

  void clear() { cache_.clear(); }
  void remove(int32_t fontId) { cache_.remove(fontId); }

 private:
  static constexpr size_t kMaxEntries = 100;
  android::LruCache<int32_t, hb_font_t*> cache_;
};

HbFontCache* getFontCacheLocked() {
  assertMinikinLocked();
  static HbFontCache* cache = nullptr;
  if (cache == nullptr) {
    cache = new HbFontCache();
  }
  return cache;
}

void purgeHbFontCacheLocked() {
  assertMinikinLocked();
  getFontCacheLocked()->clear();
}
hb_font_t* getHbFontLocked(const MinikinFont* minikinFont) {
  assertMinikinLocked();
  static hb_font_t* nullFaceFont = nullptr;
  if (minikinFont == nullptr) {
    if (nullFaceFont == nullptr) {
      nullFaceFont = hb_font_create(nullptr);
    }
    return hb_font_reference(nullFaceFont);
  }

  HbFontCache* fontCache = getFontCacheLocked();
  const int32_t fontId = minikinFont->GetUniqueId();
  hb_font_t* font = fontCache->get(fontId);
  if (font != nullptr) {
    return hb_font_reference(font);
  }

  hb_face_t* face = minikinFont->CreateHarfBuzzFace();

  hb_font_t* parent_font = hb_font_create(face);
  hb_ot_font_set_funcs(parent_font);

  unsigned int upem = hb_face_get_upem(face);
  hb_font_set_scale(parent_font, upem, upem);

  font = hb_font_create_sub_font(parent_font);
  std::vector<hb_variation_t> variations;
  for (const auto& variation : minikinFont->GetAxes()) {
    variations.push_back({variation.axisTag, variation.value});
  }
  hb_font_set_variations(font, variations.data(), variations.size());
  hb_font_destroy(parent_font);
  hb_face_destroy(face);
  fontCache->put(fontId, font);
  return hb_font_reference(font);
}
void purgeHbFontLocked(const MinikinFont* minikinFont) {
  assertMinikinLocked();
  const int32_t fontId = minikinFont->GetUniqueId();
  getFontCacheLocked()->remove(fontId);
}

}  // namespace minikin
