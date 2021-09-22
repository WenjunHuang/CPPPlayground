//
// Created by rick on 2021/9/22.
//

#pragma once
#include "shaper.h"
class FontMgrRunIterator final : public Shaper::FontRunIterator {
 public:
  FontMgrRunIterator(const char* utf8,
                     size_t utf8Bytes,
                     const SkFont& font,
                     sk_sp<SkFontMgr> fallbackMgr,
                     const char* requestName,
                     SkFontStyle requestStyle,
                     const Shaper::LanguageRunIterator* lang)
      : fCurrent(utf8),
        fBegin(utf8),
        fEnd(fCurrent + utf8Bytes),
        fFallbackMgr(std::move(fallbackMgr)),
        fFont(font),
        fFallbackFont(fFont),
        fCurrentFont(nullptr),
        fRequestName(requestName),
        fRequestStyle(requestStyle),
        fLanguage(lang) {
    fFont.setTypeface(font.refTypefaceOrDefault());
    fFallbackFont.setTypeface(nullptr);
  }
  FontMgrRunIterator(const char* utf8,
                     size_t utf8Bytes,
                     const SkFont& font,
                     sk_sp<SkFontMgr> fallbackMgr)
      : FontMgrRunIterator(utf8,
                           utf8Bytes,
                           font,
                           std::move(fallbackMgr),
                           nullptr,
                           font.refTypefaceOrDefault()->fontStyle(),
                           nullptr) {}

  void consume() override {
    SkASSERT(fCurrent < fEnd);
    SkASSERT(!fLanguage ||
             this->endOfCurrentRun() <= fLanguage->endOfCurrentRun());
    SkUnichar u;
    int pos = 0;
    U8_NEXT_OR_FFFD(fCurrent, pos, fEnd - fCurrent, u);
    fCurrent += pos;

    // If the starting typeface can handle this character, use it.
    if (fFont.unicharToGlyph(u)) {
      fCurrentFont = &fFont;
      // If the current fallback can handle this character, use it.
    } else if (fFallbackFont.getTypeface() && fFallbackFont.unicharToGlyph(u)) {
      fCurrentFont = &fFallbackFont;
      // If not, try to find a fallback typeface
    } else {
      const char* language = fLanguage ? fLanguage->currentLanguage() : nullptr;
      int languageCount = fLanguage ? 1 : 0;
      sk_sp<SkTypeface> candidate(fFallbackMgr->matchFamilyStyleCharacter(
          fRequestName, fRequestStyle, &language, languageCount, u));
      if (candidate) {
        fFallbackFont.setTypeface(std::move(candidate));
        fCurrentFont = &fFallbackFont;
      } else {
        fCurrentFont = &fFont;
      }
    }

    while (fCurrent < fEnd) {
      const char* prev = fCurrent;
      SkUnichar u;
      int i = 0;
      U8_NEXT_OR_FFFD(fCurrent, i, fEnd - fCurrent, u);
      fCurrent += i;

      // End run if not using initial typeface and initial typeface has this
      // character.
      if (fCurrentFont->getTypeface() != fFont.getTypeface() &&
          fFont.unicharToGlyph(u)) {
        fCurrent = prev;
        return;
      }

      // End run if current typeface does not have this character and some other
      // font does.
      if (!fCurrentFont->unicharToGlyph(u)) {
        const char* language =
            fLanguage ? fLanguage->currentLanguage() : nullptr;
        int languageCount = fLanguage ? 1 : 0;
        sk_sp<SkTypeface> candidate(fFallbackMgr->matchFamilyStyleCharacter(
            fRequestName, fRequestStyle, &language, languageCount, u));
        if (candidate) {
          fCurrent = prev;
          return;
        }
      }
    }
  }

  size_t endOfCurrentRun() const override { return fCurrent - fBegin; }
  bool atEnd() const override { return fCurrent == fEnd; }
  const SkFont& currentFont() const override { return *fCurrentFont; }

 private:
  char const* fCurrent;
  char const* const fBegin;
  char const* const fEnd;
  const sk_sp<SkFontMgr> fFallbackMgr;
  SkFont fFont;
  SkFont fFallbackFont;
  SkFont* fCurrentFont;
  const char* const fRequestName;
  const SkFontStyle fRequestStyle;
  const Shaper::LanguageRunIterator* const fLanguage;
};
