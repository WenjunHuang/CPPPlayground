//
// Created by rick on 2021/10/20.
//

#include "line_breaker.h"
namespace minikin {
// Large scores in a hierarchy; we prefer desperate breaks to an overfull line.
// All these constants are larger than any reasonable actual width score.
const float SCORE_INFTY = std::numeric_limits<float>::max();
const float SCORE_OVERFULL = 1e12f;
const float SCORE_DESPERATE = 1e10f;

void LineBreaker::setLocale() {
  word_breaker_.setLocale();
  locale_ = icu::Locale();
  hyphenator_ = nullptr;
}
void LineBreaker::setText() {
  word_breaker_.setText(text_buf_.data(), text_buf_.size());

  // handle initial break here because addStyleRun may never be called
  word_breaker_.next();
  candidates_.clear();
  Candidate cand = {0,   0, 0.0, 0.0, 0.0,
                    0.0, 0, 0,   0,   HyphenationType::DONT_BREAK};
  candidates_.push_back(cand);

  // reset greedy breaker state
  breaks_.clear();
  widths_.clear();
  flags_.clear();
  last_break_ = 0;
  best_break_ = 0;
  best_score_ = SCORE_INFTY;
  prebreak_ = 0;
  last_hyphenation_ = HyphenEdit::NO_EDIT;
  first_tab_index_ = INT_MAX;
  space_count_ = 0;
}
void LineBreaker::setLineWidths(float firstWidth,
                                int firstWidthLineCount,
                                float restWidth) {
  line_widths_.setWidths(firstWidth, firstWidthLineCount, restWidth);
}
void LineBreaker::setIndents(const std::vector<float>& indents) {
  line_widths_.setIndents(indents);
}

// Ordinarily, this method measures the text in the range given. However, when
// paint is nullptr, it assumes the widths have already been calculated and
// stored in the width buffer. This method finds the candidate word breaks
// (using the ICU break iterator) and sends them to addCandidate.
float LineBreaker::addStyleRun(MinikinPaint* paint,
                               const std::shared_ptr<FontCollection>& typeface,
                               FontStyle style,
                               size_t start,
                               size_t end,
                               bool isRtl) {
  float width = 0.0f;
  float hyphenPenalty = 0.0;
  if (paint != nullptr) {
//    width = Layout::
  }
  return 0.0;
}
}  // namespace minikin
