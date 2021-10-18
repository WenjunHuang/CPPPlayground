//
// Created by rick on 2021/10/11.
//

#pragma once
#include <harfbuzz/hb.h>
#include <cstdint>
#include <memory>
#include <string>
#include "Hyphenator.h"
#include "font_family.h"
#include "hb_font_cache.h"

// An abstraction for platform fonts, allowing Minikin to be used with
// multiple actual implementations of fonts.
namespace minikin {
class MinikinFont;

struct MinikinPaint {
  MinikinFont* font;
  float size;
  float scaleX;
  float skewX;
  float letterSpacing;
  float wordSpacing;
  uint32_t paintFlags;
  FontFakery fakery;
  HyphenEdit hyphenEdit;
  std::string fontFeatureSettings;
};

// Only a few flags affect layout, but those that do should have values
// consistent with Android's paint flags.
enum MinikinPaintFlags {
  LinearTextFlag = 0x40,
};
struct MinikinRect {
  float mLeft, mTop, mRight, mBottom;
  bool isEmpty() const { return mLeft == mRight || mTop == mBottom; }
  void set(const MinikinRect& r) {
    mLeft = r.mLeft;
    mTop = r.mTop;
    mRight = r.mRight;
    mBottom = r.mBottom;
  }
  void offset(float dx, float dy) {
    mLeft += dx;
    mTop += dy;
    mRight += dx;
    mBottom += dy;
  }
  void setEmpty() { mLeft = mTop = mRight = mBottom = 0; }
  void join(const MinikinRect& r);
};

// Callback for freeing data
typedef void (*MinikinDestroyFunc)(void* data);

class MinikinFont {
 public:
  explicit MinikinFont(int32_t unique_id) : unique_id_(unique_id) {}
  virtual ~MinikinFont();
  virtual float GetHorizontalAdvance(uint32_t glyph_id,
                                     const MinikinPaint& paint) const = 0;
  virtual void GetBounds(MinikinRect* bounds,
                         uint32_t glyph_id,
                         const MinikinPaint& paint) const = 0;

  virtual hb_face_t* CreateHarfBuzzFace() const { return nullptr; }

  virtual const std::vector<minikin::FontVariation>& GetAxes() const = 0;

  virtual std::shared_ptr<MinikinFont> CreateFontWithVariation(
      const std::vector<FontVariation>&) const {
    return nullptr;
  }

  static uint32_t MakeTag(char c1, char c2, char c3, char c4) {
    return ((uint32_t)c1 << 24) | ((uint32_t)c2 << 16) | ((uint32_t)c3 << 8) |
           (uint32_t)c4;
  }

  [[nodiscard]] int32_t GetUniqueId() const { return unique_id_; }

 private:
  const int32_t unique_id_;
};
}  // namespace minikin
