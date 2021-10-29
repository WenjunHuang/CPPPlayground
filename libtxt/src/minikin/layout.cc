//
// Created by rick on 2021/10/25.
//

#include "layout.h"
#include <mutex>
#include "minikin_internal.h"

namespace minikin {
const int kDirection_Mask = 0x1;
struct LayoutContext {
  MinikinPaint paint;
  FontStyle style;
  std::vector<hb_font_t*> hbFonts;  // parallel to mFaces

  void clearHbFonts() {
    for (size_t i = 0; i < hbFonts.size(); i++) {
      hb_font_set_funcs(hbFonts[i], nullptr, nullptr, nullptr);
      hb_font_destroy(hbFonts[i]);
    }
    hbFonts.clear();
  }
};

// Layout cache datatypes
class LayoutCacheKey {
 public:
  LayoutCacheKey(const std::shared_ptr<FontCollection>& collection,
                 const MinikinPaint& paint,
                 FontStyle style,
                 const uint16_t* chars,
                 size_t start,
                 size_t count,
                 size_t nchars,
                 bool dir)
      : mChars(chars),
        mNchars(nchars),
        mStart(start),
        mCount(count),
        mId(collection->getId()),
        mStyle(style),
        mSize(paint.size),
        mScaleX(paint.scaleX),
        mSkewX(paint.skewX),
        mLetterSpacing(paint.letterSpacing),
        mPaintFlags(paint.paintFlags),
        mHyphenEdit(paint.hyphenEdit),
        mIsRtl(dir),
        mHash(computeHash()) {}
 private:
  const uint16_t* mChars;
  size_t mNchars;
  size_t mStart;
  size_t mCount;
  uint32_t mId; // for the font collection
  FontStyle mStyle;
  float mSize;
  float mScaleX;
  float mSkewX;
  float mLetterSpacing;
  int32_t mPaintFlags;
  HyphenEdit mHyphenEdit;
  bool mIsRtl;

  android::hash_t mHash;
  android::hash_t computeHash() const;
};
float Layout::measureText(const uint16_t* buf,
                          size_t start,
                          size_t count,
                          size_t bufSize,
                          bool isRtl,
                          const minikin::FontStyle& style,
                          const minikin::MinikinPaint& paint,
                          const std::shared_ptr<FontCollection>& collection,
                          float* advances) {
  std::scoped_lock lock(gMinikinLock);
  return 0.0;
}
}  // namespace minikin