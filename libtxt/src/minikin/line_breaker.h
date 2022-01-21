//
// Created by rick on 2021/10/20.
//

#pragma once
#include <vector>
#include "Hyphenator.h"
#include "font_collection.h"
#include "minikin_font.h"
#include "word_breaker.h"

namespace minikin {
enum BreakStrategy {
  kBreakStrategy_Greedy = 0,
  kBreakStrategy_HighQuality = 1,
  kBreakStrategy_Balanced = 2
};

enum HyphenationFrequency {
  kHyphenationFrequency_None = 0,
  kHyphenationFrequency_Normal = 1,
  kHyphenationFrequency_Full = 2
};

class LineWidths {
 public:
  void setWidths(float firstWidth, int firstWidthLineCount, float restWidth) {
    first_width_ = firstWidth;
    first_width_line_count_ = firstWidthLineCount;
    rest_width_ = restWidth;
  }
  void setIndents(const std::vector<float>& indents) { indents_ = indents; }

  bool isConstant() const {
    return rest_width_ == first_width_ && indents_.empty();
  }

  float getLineWidth(int line) const {
    float width = (line < first_width_line_count_) ? first_width_ : rest_width_;
    if (!indents_.empty()) {
      if ((size_t)line < indents_.size()) {
        width -= indents_[line];
      } else {
        width -= indents_.back();
      }
    }
    return width;
  }
  void clear() { indents_.clear(); }

 private:
  float first_width_;
  int first_width_line_count_;
  float rest_width_;
  std::vector<float> indents_;
};
class LineBreaker {
 public:
  const static int kTab_Shift =
      29;  // keep synchronized with TAB_MASK in StaticLayout.java

  // Note: Locale persists across multiple invocations (it is not cleaned up by
  // finish()), explicitly to avoid the cost of creating ICU BreakIterator
  // objects. It should always be set on the first invocation, but callers are
  // encouraged not to call again unless locale has actually changed. That logic
  // could be here but it's better for performance that it's upstream because of
  // the cost of constructing and comparing the ICU Locale object.
  // Note: caller is responsible for managing lifetime of hyphenator
  //
  // libtxt extension: always use the default locale so that a cached instance
  // of the ICU break iterator can be reused.
  void setLocale();

  void resize(size_t size) {
    text_buf_.resize(size);
    char_widths_.resize(size);
  }

  size_t size() const { return text_buf_.size(); }
  uint16_t* buffer() { return text_buf_.data(); }
  float* charWidths() { return char_widths_.data(); }

  // set text to current contents of buffer
  void setText();

  void setLineWidths(float firstWidth,
                     int firstWidthLineCount,
                     float restWidth);

  void setIndents(const std::vector<float>& indents);

  BreakStrategy getStrategy() const { return strategy_; }

  void setStrategy(BreakStrategy strategy) { strategy_ = strategy; }
  void setJustified(bool justified) { justified_ = justified; }

  HyphenationFrequency getHyphenationFrequency() const {
    return hyphenation_frequency_;
  }

  // TODO: this class is actually fairly close to being general and not tied to
  // using Minikin to do the shaping of the strings. The main thing that would
  // need to be changed is having some kind of callback (or virtual class, or
  // maybe even template), which could easily be instantiated with Minikin's
  // Layout. Future work for when needed.
  float addStyleRun(MinikinPaint* paint,
                    const std::shared_ptr<FontCollection>& typeface,
                    FontStyle style,
                    size_t start,
                    size_t end,
                    bool isRtl);

 private:
  // ParaWidth is used to hold cumulative width from beginning of paragraph.
  // Note that for very large paragraphs, accuracy could degrade using only
  // 32-bit float. Note however that float is used extensively on the Java side
  // for this. This is a typedef so that we can easily change it based on
  // performance/accuracy tradeoff.
  typedef double ParaWidth;

  // A single candidate break
  struct Candidate {
    size_t offset;        // offset to text buffer, in code units
    size_t prev;          // index to previous break
    ParaWidth preBreak;   // width of text until this point, if we decide to not
                          // break here
    ParaWidth postBreak;  // width of text until this point, if we decide to
                          // break here
    float penalty;        // penalty of this break (for example, hyphen penalty)
    float score;          // best score found for this break
    size_t lineNumber;    // only updated for non-constant line widths
    size_t preSpaceCount;   // preceding space count before breaking
    size_t postSpaceCount;  // preceding space count after breaking
    HyphenationType hyphenationType;
  };

  float currentLineWidth() const;
  void addWordBreak(size_t offset,
                    ParaWidth preBreak,
                    ParaWidth postBreak,
                    size_t preSpaceCount,
                    size_t postSpaceCount,
                    float penalty,
                    HyphenationType hyph);

  void addCandidate(Candidate cand);
  void pushGreedyBreak();

  // push an actual break to the output. Takes care of setting flags for tab
  void pushBreak(int offset, float width, uint8_t hyphenEdit);

  float getSpaceWidth() const;
  void computeBreaksGreedy();
  void computeBreaksOptimal(bool isRectangular);
  void finishBreaksOptimal();

  WordBreaker word_breaker_;
  icu::Locale locale_;
  std::vector<uint16_t> text_buf_;
  std::vector<float> char_widths_;

  Hyphenator* hyphenator_;
  std::vector<HyphenationType> hyph_buf_;

  // layout parameters
  BreakStrategy strategy_ = kBreakStrategy_Greedy;
  HyphenationFrequency hyphenation_frequency_ = kHyphenationFrequency_Normal;
  bool justified_;
  LineWidths line_widths_;

  // result of line breaking
  std::vector<int> breaks_;
  std::vector<float> widths_;
  std::vector<int> flags_;

  ParaWidth width_ = 0;
  std::vector<Candidate> candidates_;
  float line_penalty_ = 0.0f;

  // the following are state for greedy breaker (updated while adding style
  // runs)
  size_t last_break_;
  size_t best_break_;
  float best_score_;
  ParaWidth prebreak_;         // prebreak of last break
  uint32_t last_hyphenation_;  // hyphen edit of last break kept for next line
  int first_tab_index_;
  size_t space_count_;
};
}  // namespace minikin
