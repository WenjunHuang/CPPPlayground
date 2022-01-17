//
// Created by rick on 2022/1/10.
//

#include <algorithm>
#include <fmt/printf.h>
#include <string>

void reverseWordsInPlace(std::string &s) {
  // First, reverse the whole string.
  std::reverse(s.begin(), s.end());

  // Next, un-reverse each individual word.
  for (auto it = s.begin(); true; ++it) {
    auto next = std::find(it, s.end(), ' ');
    // Reverse the order of letters in this word.
    std::reverse(it, next);
    if (next == s.end())
      break;
    it = next;
  }
}

int main() {
  std::string s{"the quick brown fox jumps over the lazy dog"};
  reverseWordsInPlace(s);
  fmt::print("{}", s);

}