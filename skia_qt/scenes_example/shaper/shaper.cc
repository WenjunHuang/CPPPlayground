//
// Created by rick on 2021/9/15.
//

#include "shaper.h"
#include <hb-ot.h>
#include <hb.h>
#include <unicode/unistr.h>
#include <range/v3/algorithm.hpp>
#include <set>
#include "unicode.h"
#include "font_mgr_run_iterator.h"
using namespace ranges;
namespace {
template <typename T, auto* p>
using resource =
    std::unique_ptr<T,
                    SkFunctionWrapper<std::remove_pointer_t<decltype(p)>, p>>;

using HBBuffer = resource<hb_buffer_t, hb_buffer_destroy>;
using UnicodeBreak = std::unique_ptr<BreakIterator>;
using UnicodeBidi = std::unique_ptr<BidiIterator>;
using UnicodeScript = std::unique_ptr<ScriptIterator>;

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
};

class RunIteratorQueue {
 public:
  void inset(Shaper::RunIterator* runIterator, int priority) {
    fEntries.insert({runIterator, priority});
  }

  bool advanceRuns() {
    auto leastRun = (*fEntries.begin()).runIterator;
    if (leastRun->atEnd()) {
      SkASSERT(this->allRunsAreAtEnd());
      return false;
    }

    const auto leastEnd = leastRun->endOfCurrentRun();
    Shaper::RunIterator* currentRun = nullptr;
    SkDEBUGCODE(size_t previousEndOfCurrentRun);
    while (
        (currentRun = std::begin(fEntries)->runIterator)->endOfCurrentRun() <=
        leastEnd) {
      auto priority = std::begin(fEntries)->priority;
      fEntries.erase(std::begin(fEntries));

      SkDEBUGCODE(previousEndOfCurrentRun = currentRun->endOfCurrentRun());
      currentRun->consume();
      SkASSERT(previousEndOfCurrentRun < currentRun->endOfCurrentRun());
      fEntries.insert({currentRun, priority});
    }
    return true;
  }

  size_t endOfCurrentRun() const {
    return std::begin(fEntries)->runIterator->endOfCurrentRun();
  }

 private:
  bool allRunsAreAtEnd() const {
    return all_of(fEntries,
                  [](const auto& entry) { return entry.runIterator->atEnd(); });
  }
  struct Entry {
    Shaper::RunIterator* runIterator;
    int priority;
    bool operator<(const Entry& other) const {
      auto myEnd = runIterator->endOfCurrentRun();
      auto otherEnd = other.runIterator->endOfCurrentRun();
      return myEnd < otherEnd ||
             (myEnd == otherEnd && priority < other.priority);
    }
  };
  std::set<Entry> fEntries;
};

class UnicodeBidiRunIterator final : public Shaper::BiDiRunIterator {
 public:
  UnicodeBidiRunIterator(const char* utf8, const char* end, UnicodeBidi bidi)
      : fBidi(std::move(bidi)),
        fEndOfCurrentRun(utf8),
        fBegin(utf8),
        fEnd(end),
        fUTF16LogicalPosition(0),
        fLevel(BidiIterator::kLTR) {}

  void consume() override {
    SkASSERT(fUTF16LogicalPosition < fBidi->getLength());
    int32_t endPosition = fBidi->getLength();
    fLevel = fBidi->getLevelAt(fUTF16LogicalPosition);
    SkUnichar u;
    int offset = 0;
    U8_NEXT_OR_FFFD((const uint8_t*)fEndOfCurrentRun, offset,
                    fEnd - fEndOfCurrentRun, u);
    fEndOfCurrentRun += offset;
    fUTF16LogicalPosition += U16_LENGTH(u);

    BidiIterator::Level level;
    while (fUTF16LogicalPosition < endPosition) {
      level = fBidi->getLevelAt(fUTF16LogicalPosition);
      if (level != fLevel) {
        break;
      }

      U8_NEXT_OR_FFFD((const uint8_t*)fEndOfCurrentRun, offset,
                      fEnd - fEndOfCurrentRun, u);
      fEndOfCurrentRun += offset;
      fUTF16LogicalPosition += U16_LENGTH(u);
    }
  }

  [[nodiscard]] size_t endOfCurrentRun() const override {
    return fEndOfCurrentRun - fBegin;
  }

  [[nodiscard]] bool atEnd() const override {
    return fUTF16LogicalPosition == fBidi->getLength();
  }

  [[nodiscard]] BidiIterator::Level currentLevel() const override {
    return fLevel;
  }

 private:
  UnicodeBidi fBidi;
  const char* fEndOfCurrentRun;
  const char* const fBegin;
  const char* const fEnd;
  int32_t fUTF16LogicalPosition;
  BidiIterator::Level fLevel;
};

class UnicodeHbScriptRunIterator final : public Shaper::ScriptRunIterator {
 public:
  UnicodeHbScriptRunIterator(UnicodeScript, const char* utf8, size_t utf8Bytes)
      : fCurrent(utf8),
        fBegin(utf8),
        fEnd(fCurrent + utf8Bytes),
        fCurrentScript(HB_SCRIPT_UNKNOWN) {}

  hb_script_t hb_script_for_unichar(SkUnichar u) {
    return hb_unicode_script(hb_unicode_funcs_get_default(), u);
  }

  void consume() override {
    SkASSERT(fCurrent < fEnd);
    SkUnichar u;
    int offset = 0;

    U8_NEXT_OR_FFFD((const uint8_t*)fCurrent, offset, fEnd - fCurrent, u);
    fCurrent += offset;
    fCurrentScript = hb_script_for_unichar(u);

    while (fCurrent < fEnd) {
      const char* prev = fCurrent;
      U8_NEXT_OR_FFFD((const uint8_t*)fCurrent, offset, fEnd - fCurrent, u);
      fCurrent += offset;

      const auto script = hb_script_for_unichar(u);
      if (script != fCurrentScript) {
        if (fCurrentScript == HB_SCRIPT_INHERITED ||
            fCurrentScript == HB_SCRIPT_COMMON) {
          fCurrentScript = script;
        } else if (script == HB_SCRIPT_INHERITED ||
                   script == HB_SCRIPT_COMMON) {
          continue;
        } else {
          fCurrent = prev;
          break;
        }
      }
    }
    if (fCurrentScript == HB_SCRIPT_INHERITED) {
      fCurrentScript = HB_SCRIPT_COMMON;
    }
  }

  size_t endOfCurrentRun() const override { return fCurrent - fBegin; }

  bool atEnd() const override { return fCurrent == fEnd; }

  SkFourByteTag currentScript() const override {
    return SkSetFourByteTag(HB_UNTAG(fCurrentScript));
  }

 private:
  char const* fCurrent;
  char const* const fBegin;
  char const* const fEnd;
  hb_script_t fCurrentScript;
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
    BidiIterator::Level defaultLevel =
        leftToRight ? BidiIterator::kLTR : BidiIterator::kRTL;
    std::unique_ptr<BiDiRunIterator> bidi(
        MakeBidiRunIterator(fUnicode.get(), utf8, utf8Bytes, defaultLevel));
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
}  // namespace

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
std::unique_ptr<Shaper> Shaper::Make(sk_sp<SkFontMgr>) {
  return std::unique_ptr<Shaper>();
}

std::unique_ptr<Shaper::BiDiRunIterator> Shaper::MakeBiDiRunIterator(
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
      (const uint16_t*)utf16.getBuffer(), utf16.length(), bidiDir);
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
  return std::make_unique<FontMgrRunIterator>(utf8,utf8Bytes,font,std::move(fallback));
}

std::unique_ptr<Shaper::FontRunIterator> Shaper::MakeFontMgrRunIterator(
    const char* utf8,
    size_t utf8Bytes,
    const SkFont& font,
    sk_sp<SkFontMgr> fallback,
    const char* requestName,
    SkFontStyle requestStyle,
    const Shaper::LanguageRunIterator* language) {
  return std::make_unique<FontMgrRunIterator>(utf8,
                                              utf8Bytes,
                                              font,
                                              std::move(fallback),
                                              requestName,
                                              requestStyle,
                                              language);
}
