//
// Created by rick on 2021/10/13.
//

#include <unicode/ubidi.h>
#include <unicode/utf16.h>
#include <vector>

class LineMetrics {
 public:
  LineMetrics() {}
  LineMetrics(size_t start,
              size_t end,
              size_t end_excluding_whitespace,
              size_t end_including_newline,
              bool hard_break)
      : start_index(start),
        end_index(end),
        end_excluding_whitespace(end_excluding_whitespace),
        end_including_newline(end_including_newline),
        hard_break(hard_break) {}

 public:
  // The following fields are used in the layout process itself.

  // The indexes in the text buffer the line begins and ends.
  size_t start_index = 0;
  size_t end_index = 0;
  size_t end_excluding_whitespace = 0;
  size_t end_including_newline = 0;
  bool hard_break = false;

  // The following fields are tracked after or during layout to provide to
  // the user as well as for computing bounding boxes.

  // The final computed ascent and descent for the line. This can be impacted by
  // the strut, height, scaling, as well as outlying runs that are very tall.
  //
  // The top edge is `baseline - ascent` and the bottom edge is `baseline +
  // descent`. Ascent and descent are provided as positive numbers. Raw numbers
  // for specific runs of text can be obtained in run_metrics_map. These values
  // are the cumulative metrics for the entire line.
  double ascent = 0.0;
  double descent = 0.0;
  double unscaled_ascent = 0.0;

  // Total height of the paragraph including the current line.

  // The height of the current line is `round(ascent + descent)`.
  double height = 0.0;
  // Width of the line.
  double width = 0.0;
  // The left edge of the line. The right edge can be obtained with `left +
  // width`
  double left = 0.0;
  // The y position of the baseline for this line from the top of the paragraph.
  double baseline = 0.0;
  // Zero indexed line number.
  size_t line_number = 0;
};
bool ComputeLineBreaks(const std::vector<char16_t>& paragraph_text,
                       std::vector<LineMetrics>* line_metrics,
                       std::vector<double>* line_widths) {
  line_metrics->clear();
  line_widths->clear();

  std::vector<size_t> newline_positions;
  // Discover and add all hard breaks.
  for (size_t i = 0; i < paragraph_text.size(); ++i) {
    ULineBreak ubl = static_cast<ULineBreak>(
        u_getIntPropertyValue(paragraph_text[i], UCHAR_LINE_BREAK));
    if (ubl == U_LB_LINE_FEED || ubl == U_LB_MANDATORY_BREAK)
      newline_positions.push_back(i);
  }

  // Break at the end of the paragraph.
  newline_positions.push_back(paragraph_text.size());

  // Calculate and add any breaks due to a line being too long.
  size_t run_index = 0;
  size_t inline_placeholder_index = 0;
  for (size_t newline_index = 0; newline_index < newline_positions.size();
       ++newline_index) {
    size_t block_start =
        newline_index > 0 ? newline_positions[newline_index - 1] + 1 : 0;
    size_t block_end = newline_positions[newline_index];
    size_t block_size = block_end - block_start;

    if (block_size == 0) {
      line_metrics->emplace_back(block_start,
                                 block_end,
                                 block_end,
                                 block_end + 1,
                                 true);
      line_widths->push_back(0);
      continue;
    }
  }
}