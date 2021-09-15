//
// Created by rick on 2021/9/15.
//

#include "shaper.h"
#include <hb-ot.h>
#include <hb.h>
#include <skia/modules/unicode/SkUnicode.h>
#include <queue>
#include <range/v3/algorithm.hpp>
using namespace ranges;
namespace {
template <typename T, auto* p>
using resource =
    std::unique_ptr<T,
                    SkFunctionWrapper<std::remove_pointer_t<decltype(p)>, p>>;

using HBBuffer = resource<hb_buffer_t, hb_buffer_destroy>;

using SkUnicodeBreak = std::unique_ptr<SkBreakIterator>;

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
  SkBidiIterator::Level fLevel;
  std::unique_ptr<ShapedGlyph[]> fGlyphs;
  size_t fNumGlyphs;
  SkVector fAdvance;
};

class RunIteratorQueue {
 private:
  bool allRunsAreAtEnd() const {
    return all_of(fEntries,[](const auto& entry){
      return entry.runIterator->atEnd();
    });
  }
  struct Entry {
    Shaper::RunIterator* runIterator;
    int priority;
    bool operator<(const Entry& other) {
      auto myEnd = runIterator->endOfCurrentRun();
      auto otherEnd = other.runIterator->endOfCurrentRun();
      return myEnd < otherEnd ||
             (myEnd == otherEnd && priority < other.priority);
    }
  };
  std::vector<Entry> fEntries;
};

class ShaperHarfBuzz : public Shaper {
 public:
  ShaperHarfBuzz(std::unique_ptr<SkUnicode> unicode,
                 SkUnicodeBreak lineIter,
                 SkUnicodeBreak graphIter,
                 HBBuffer buffer,
                 sk_sp<SkFontMgr> fontMgr)
      : fUnicode(std::move(unicode)),
        fLineBreakIterator(std::move(lineIter)),
        fGraphemeBreakIterator(std::move(graphIter)),
        fFontMgr(std::move(fontMgr)),
        fBuffer(std::move(buffer)),
        fUndefinedLanguage(hb_language_from_string("und", -1)) {}

 protected:
  std::unique_ptr<SkUnicode> fUnicode;
  SkUnicodeBreak fLineBreakIterator;
  SkUnicodeBreak fGraphemeBreakIterator;

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
             RunHandler* handler) const override;
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
}  // namespace

static std::unique_ptr<Shaper> MakeHarfBuzz(sk_sp<SkFontMgr> fontMgr,
                                            bool correct) {
  HBBuffer buffer(hb_buffer_create());
  if (!buffer) {
    SkDEBUGF("Could not create hb_buffer");
    return nullptr;
  }

  auto unicode = SkUnicode::Make();
  if (!unicode)
    return nullptr;

  auto lineIter =
      unicode->makeBreakIterator("th", SkUnicode::BreakType::kLines);
  if (!lineIter)
    return nullptr;

  auto graphIter =
      unicode->makeBreakIterator("th", SkUnicode::BreakType::kGraphemes);
  if (!graphIter)
    return nullptr;

  if (correct) {
    return std::make_unique<ShaperDrivenWrapper>(
        std::move(unicode), std::move(lineIter), std::move(graphIter),
        std::move(buffer), std::move(fontMgr));
  }
}
std::unique_ptr<Shaper> Shaper::Make(sk_sp<SkFontMgr>) {
  return std::unique_ptr<Shaper>();
}
