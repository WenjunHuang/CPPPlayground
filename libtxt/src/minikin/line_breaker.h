//
// Created by rick on 2021/10/20.
//

#pragma once
#include <vector>

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
    if (!indents_.empty()){
      if ((size_t)line < indents_.size()){
        width -= indents_[line];
      } else {
        width -= indents_.back();
      }
    }
    return width;
  }
  void clear() {indents_.clear();}

 private:
  float first_width_;
  int first_width_line_count_;
  float rest_width_;
  std::vector<float> indents_;
};
class LineBreaker {};
}  // namespace minikin
