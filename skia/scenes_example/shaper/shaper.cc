//
// Created by rick on 2021/9/15.
//

#include "shaper.h"
#include <hb-ot.h>
#include <hb.h>
#include <skia/core/SkStream.h>
#include <unicode/unistr.h>
#include <map>
#include <mutex>
#include <range/v3/algorithm.hpp>
#include <set>
#include "font_mgr_run_iterator.h"
#include "run_iterator_queue.h"
#include "unicode.h"
#include "unicode_bidi_run_iterator.h"
using namespace ranges;

namespace {
template <typename T, auto* p>
using resource =
    std::unique_ptr<T,
                    SkFunctionWrapper<std::remove_pointer_t<decltype(p)>, p>>;

using HBBuffer = resource<hb_buffer_t, hb_buffer_destroy>;
using HBFont = resource<hb_font_t, hb_font_destroy>;
using HBFace = resource<hb_face_t, hb_face_destroy>;
using HBBlob = resource<hb_blob_t, hb_blob_destroy>;
using UnicodeBreak = std::unique_ptr<BreakIterator>;
using UnicodeBidi = std::unique_ptr<BidiIterator>;
using UnicodeScript = std::unique_ptr<ScriptIterator>;

bool is_LTR(BidiIterator::Level level) {
  return (level & 1) == 0;
}

HBBlob stream_to_blob(std::unique_ptr<SkStreamAsset> asset) {
  size_t size = asset->getLength();
  HBBlob blob;
  if (auto base = asset->getMemoryBase()) {
    blob.reset(hb_blob_create((char*)base, SkToInt(size),
                              HB_MEMORY_MODE_READONLY, asset.release(),
                              [](void* p) { delete (SkStreamAsset*)p; }));
  } else {
    void* ptr = size ? sk_malloc_throw(size) : nullptr;
    asset->read(ptr, size);
    blob.reset(hb_blob_create((char*)ptr, SkToUInt(size),
                              HB_MEMORY_MODE_READONLY, ptr, sk_free));
  }
  SkASSERT(blob);
  hb_blob_make_immutable(blob.get());
  return blob;
}

hb_blob_t* skhb_get_table(hb_face_t* face, hb_tag_t tag, void* user_data) {
  SkTypeface& typeface = *reinterpret_cast<SkTypeface*>(user_data);

  auto data = typeface.copyTableData(tag);
  if (!data)
    return nullptr;

  SkData* rawData = data.release();
  return hb_blob_create(reinterpret_cast<char*>(rawData->writable_data()),
                        rawData->size(), HB_MEMORY_MODE_READONLY, rawData,
                        [](void* ctx) { SkSafeUnref(((SkData*)ctx)); });
}

class HBLockedFaceCache {
 public:
  HBLockedFaceCache(std::map<SkFontID, HBFace>& cache, std::mutex& mutex)
      : fCache(cache), fMutex(mutex) {
    fMutex.lock();
  }
  ~HBLockedFaceCache() { fMutex.unlock(); }
  HBLockedFaceCache(const HBLockedFaceCache&) = delete;
  HBLockedFaceCache& operator=(const HBLockedFaceCache&) = delete;

  HBFace* find(SkFontID fontId) {
    auto result = fCache.find(fontId);
    if (result == fCache.end())
      return nullptr;
    else
      return &(result->second);
  }
  HBFace* insert(SkFontID fontId, HBFace hbFace) {
    auto result = fCache.insert_or_assign(fontId, std::move(hbFace));
    if (result.second)
      return &(result.first->second);
    else
      return nullptr;
  }
  void reset() { fCache.clear(); }

 private:
  std::map<SkFontID, HBFace>& fCache;
  std::mutex& fMutex;
};

static HBLockedFaceCache get_hbFace_cache() {
  static std::mutex gHBFaceCacheMutex;
  static std::map<SkFontID, HBFace> gHBFaceCache;
  return HBLockedFaceCache(gHBFaceCache, gHBFaceCacheMutex);
}

HBFace create_hb_face(const SkTypeface& typeface) {
  int index;
  std::unique_ptr<SkStreamAsset> typefaceAsset = typeface.openStream(&index);
  HBFace face;
  if (typefaceAsset && typefaceAsset->getMemoryBase()) {
    HBBlob blob(stream_to_blob(std::move(typefaceAsset)));
    // hb_face_create always succeeds. Check that the format is minimally
    // recognized first. hb_face_create_for_tables may still create a working
    // hb_face. See https://github.com/harfbuzz/harfbuzz/issues/248 .
    auto num_hb_faces = hb_face_count(blob.get());
    if (0 < num_hb_faces && (unsigned)index < num_hb_faces) {
      face.reset(hb_face_create(blob.get(), (unsigned)index));
      // Check the number of glyphs as a basic sanitization step.
      if (face && hb_face_get_glyph_count(face.get()) == 0) {
        face.reset();
      }
    }
  }
  if (!face) {
    face.reset(hb_face_create_for_tables(
        skhb_get_table, const_cast<SkTypeface*>(SkRef(&typeface)),
        [](void* user_data) {
          SkSafeUnref(reinterpret_cast<SkTypeface*>(user_data));
        }));
  }
  SkASSERT(face);
  if (!face) {
    return nullptr;
  }
  hb_face_set_index(face.get(), (unsigned)index);
  hb_face_set_upem(face.get(), typeface.getUnitsPerEm());

  return face;
}

HBFont create_hb_font(const SkFont& font, const HBFace& face) {
  HBFont otFont(hb_font_create(face.get()));
  SkASSERT(otFont);
  if (!otFont) {
    return nullptr;
  }
  hb_ot_font_set_funcs(otFont.get());
  int axis_count = font.getTypeface()->getVariationDesignPosition(nullptr, 0);
}

struct ShapedGlyph {
  SkGlyphID fID;
  uint32_t fCluster;
  SkPoint fOffset;
  SkVector fAdvance;
  bool fMayLineBreakBefore;
  bool fMustLineBreakBefore;
  bool fHasVisual;
  bool fGraphemeBreakBefore;
  bool fUnsafeToBreak;
};

struct ShapedRun {
  Shaper::RunHandler::Range fUtf8Range;
  SkFont fFont;
  BidiIterator::Level fLevel;
  std::unique_ptr<ShapedGlyph[]> fGlyphs;
  size_t fNumGlyphs;
  SkVector fAdvance;
  

  ShapedRun(Shaper::RunHandler::Range utf8Range,
            const SkFont& font,
            BidiIterator::Level level,
            std::unique_ptr<ShapedGlyph[]> glyphs,
            size_t numGlyphs,
            SkVector advance = {0, 0})
      : fUtf8Range(utf8Range),
        fFont(font),
        fLevel(level),
        fGlyphs(std::move(glyphs)),
        fNumGlyphs(numGlyphs),
        fAdvance(advance) {}
};

class ShaperHarfBuzz : public Shaper {
 public:
  ShaperHarfBuzz(std::unique_ptr<Unicode> unicode,
                 UnicodeBreak lineIter,
                 UnicodeBreak graphIter,
                 HBBuffer buffer,
                 sk_sp<SkFontMgr> fontMgr)
      : fUnicode(std::move(unicode)),
        fLineBreakIterator(std::move(lineIter)),
        fGraphemeBreakIterator(std::move(graphIter)),
        fFontMgr(std::move(fontMgr)),
        fBuffer(std::move(buffer)),
        fUndefinedLanguage(hb_language_from_string("und", -1)) {}

 protected:
  std::unique_ptr<Unicode> fUnicode;
  UnicodeBreak fLineBreakIterator;
  UnicodeBreak fGraphemeBreakIterator;

  ShapedRun shape(const char* utf8,
                  size_t utf8Bytes,
                  const char* utf8Start,
                  const char* utf8End,
                  const BiDiRunIterator&,
                  const LanguageRunIterator&,
                  const ScriptRunIterator&,
                  const FontRunIterator&,
                  const Feature*,
                  size_t featuresSize) const;

 private:
  const sk_sp<SkFontMgr> fFontMgr;
  HBBuffer fBuffer;
  hb_language_t fUndefinedLanguage;

  void shape(const char* utf8,
             size_t utf8Bytes,
             const SkFont& srcFont,
             bool leftToRight,
             SkScalar width,
             RunHandler* handler) const override {
    //    BidiIterator::Level defaultLevel =
    //        leftToRight ? BidiIterator::kLTR : BidiIterator::kRTL;
    //    std::unique_ptr<BiDiRunIterator> bidi(
    //        MakeBidiRunIterator(fUnicode.get(), utf8, utf8Bytes,
    //        defaultLevel));
  }

  void shape(const char* utf8,
             size_t utf8Bytes,
             FontRunIterator& iterator,
             BiDiRunIterator& runIterator,
             ScriptRunIterator& scriptRunIterator,
             LanguageRunIterator& languageRunIterator,
             SkScalar width,
             RunHandler* handler) const override;
  void shape(const char* utf8,
             size_t utf8Bytes,
             FontRunIterator& iterator,
             BiDiRunIterator& runIterator,
             ScriptRunIterator& scriptRunIterator,
             LanguageRunIterator& languageRunIterator,
             const Feature* features,
             size_t featuresSize,
             SkScalar width,
             RunHandler* handler) const override;

  virtual void wrap(char const* const utf8,
                    size_t utf8Bytes,
                    const BiDiRunIterator&,
                    const LanguageRunIterator&,
                    const ScriptRunIterator&,
                    const FontRunIterator&,
                    RunIteratorQueue& runSegmenter,
                    const Feature*,
                    size_t featuresSize,
                    SkScalar width,
                    RunHandler*) const = 0;
};
ShapedRun ShaperHarfBuzz::shape(const char* utf8,
                                size_t utf8Bytes,
                                const char* utf8Start,
                                const char* utf8End,
                                const Shaper::BiDiRunIterator& bidi,
                                const Shaper::LanguageRunIterator& language,
                                const Shaper::ScriptRunIterator& script,
                                const Shaper::FontRunIterator& font,
                                const Shaper::Feature* features,
                                size_t featuresSize) const {
  auto utf8runLength = utf8End - utf8Start;
  ShapedRun run(RunHandler::Range(utf8Start - utf8, utf8runLength),
                font.currentFont(), bidi.currentLevel(), nullptr, 0);

  auto buffer = fBuffer.get();
  SkAutoTCallVProc<hb_buffer_t, hb_buffer_clear_contents> autoClearBuffer(
      buffer);
  hb_buffer_set_content_type(buffer, HB_BUFFER_CONTENT_TYPE_UNICODE);
  hb_buffer_set_cluster_level(buffer,
                              HB_BUFFER_CLUSTER_LEVEL_MONOTONE_CHARACTERS);
  // Documentation for HB_BUFFER_FLAG_BOT/EOT at
  // 763e5466c0a03a7c27020e1e2598e488612529a7. Currently BOT forces a dotted
  // circle when first codepoint is a mark; EOT has no effect. Avoid adding
  // dotted circle, re-evaluate if BOT/EOT change. See https://skbug.com/9618.
  // hb_buffer_set_flags(buffer, HB_BUFFER_FLAG_BOT | HB_BUFFER_FLAG_EOT);

  // Add precontext.
  hb_buffer_add_utf8(buffer, utf8, utf8Start - utf8, utf8Start - utf8, 0);

  // Populate the hb_buffer directly with utf8 cluster indexes.
  const char* utf8Current = utf8Start;
  while (utf8Current < utf8End) {
    unsigned int cluster = utf8Current - utf8;
    int index = 0;
    UChar32 u;
    U8_NEXT_OR_FFFD(utf8Current, index, utf8End - utf8Current, u);
    utf8Current += index;
    hb_buffer_add(buffer, u, cluster);
  }

  // Add postcontext.
  hb_buffer_add_utf8(buffer, utf8Current, utf8 + utf8Bytes - utf8Current, 0, 0);

  hb_direction_t direction =
      is_LTR(bidi.currentLevel()) ? HB_DIRECTION_LTR : HB_DIRECTION_RTL;
  hb_buffer_set_direction(buffer, direction);
  hb_buffer_set_script(
      buffer, hb_script_from_iso15924_tag((hb_tag_t)script.currentScript()));
  // Buffers with HB_LANGUAGE_INVALID race since hb_language_get_default is not
  // thread safe. The user must provide a language, but may provide data
  // hb_language_from_string cannot use. Use "und" for the undefined language in
  // this case (RFC5646 4.1 5).
  hb_language_t hbLanguage =
      hb_language_from_string(language.currentLanguage(), -1);
  if (hbLanguage == HB_LANGUAGE_INVALID) {
    hbLanguage = fUndefinedLanguage;
  }
  hb_buffer_set_language(buffer, hbLanguage);
  hb_buffer_guess_segment_properties(buffer);

  // An HBFace is expensive (it sanitizes the bits).
  // An HBFont is fairly inexpensive.
  // An HBFace is actually tied to the data, not the typeface.
  // The size of 100 here is completely arbitrary and used to match libtxt.
  HBFont hbFont;
  {
    HBLockedFaceCache cache = get_hbFace_cache();
    SkFontID dataId = font.currentFont().getTypeface()->uniqueID();
    auto hbFaceCached = cache.find(dataId);
    if (!hbFaceCached) {
      HBFace hbFace(create_hb_face(*font.currentFont().getTypeface()));
      hbFaceCached = cache.insert(dataId, std::move(hbFace));
    }
    hbFont = create_hb_font(font.currentFont(), *hbFaceCached);
  }
}

static std::unique_ptr<Shaper> MakeHarfBuzz(sk_sp<SkFontMgr> fontMgr,
                                            bool correct) {
  HBBuffer buffer(hb_buffer_create());
  if (!buffer) {
    SkDEBUGF("Could not create hb_buffer");
    return nullptr;
  }

  auto unicode = Unicode::Make();
  if (!unicode)
    return nullptr;

  auto lineIter = unicode->makeBreakIterator("th", Unicode::BreakType::kLines);
  if (!lineIter)
    return nullptr;

  auto graphIter =
      unicode->makeBreakIterator("th", Unicode::BreakType::kGraphemes);
  if (!graphIter)
    return nullptr;

  if (correct) {
    return std::make_unique<ShaperDrivenWrapper>(
        std::move(unicode), std::move(lineIter), std::move(graphIter),
        std::move(buffer), std::move(fontMgr));
  } else {
    //    return std::make_unique()
  }
}
}  // namespace
std::unique_ptr<Shaper> Shaper::Make(sk_sp<SkFontMgr>) {
  return std::unique_ptr<Shaper>();
}

std::unique_ptr<Shaper::BiDiRunIterator> MakeBiDiRunIterator(
    Unicode* unicode,
    const char* utf8,
    size_t utf8Bytes,
    uint8_t bidiLevel) {
  if (!SkTFitsIn<int32_t>(utf8Bytes)) {
    SkDEBUGF("Bidi error: text too long");
    return nullptr;
  }

  UErrorCode errorCode = UErrorCode::U_ZERO_ERROR;
  icu::UnicodeString utf16(utf8, utf8Bytes, nullptr, errorCode);
  if (U_FAILURE(errorCode)) {
    SkDEBUGF("Invalid utf8 input\n");
  }

  auto bidiDir = (bidiLevel % 2 == 0) ? BidiIterator::kLTR : BidiIterator::kRTL;
  UnicodeBidi bidi = unicode->makeBidiIterator(
      (const char16_t*)utf16.getBuffer(), utf16.length(), bidiDir);
  if (!bidi) {
    SkDEBUGF("Bidi error\n");
    return nullptr;
  }
  return std::make_unique<UnicodeBidiRunIterator>(utf8, utf8 + utf8Bytes,
                                                  std::move(bidi));
}

std::unique_ptr<Shaper::FontRunIterator> Shaper::MakeFontMgrRunIterator(
    const char* utf8,
    size_t utf8Bytes,
    const SkFont& font,
    sk_sp<SkFontMgr> fallback) {
  return std::make_unique<FontMgrRunIterator>(utf8, utf8Bytes, font,
                                              std::move(fallback));
}

std::unique_ptr<Shaper::FontRunIterator> Shaper::MakeFontMgrRunIterator(
    const char* utf8,
    size_t utf8Bytes,
    const SkFont& font,
    sk_sp<SkFontMgr> fallback,
    const char* requestName,
    SkFontStyle requestStyle,
    const Shaper::LanguageRunIterator* language) {
  return std::make_unique<FontMgrRunIterator>(utf8, utf8Bytes, font,
                                              std::move(fallback), requestName,
                                              requestStyle, language);
}
