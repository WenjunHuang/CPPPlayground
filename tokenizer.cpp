//
// Created by xxzyjy on 23/07/2017.
//
#include <boost/tokenizer.hpp>
#include <iostream>
#include <gtest/gtest.h>

using namespace std;
using namespace boost;

TEST(Tokenizer, Iterator) {
  using tokenizer = tokenizer<char_separator<char>>;
  string s = "Boost C++ Libraries";

  tokenizer tok{s};
  for (const auto &t: tok)
    cout << t << endl;
//  auto itr = tok.begin();
//  EXPECT_EQ("Boost", *(itr++));
//  EXPECT_EQ("C++", *(itr++));
//  EXPECT_EQ("Libraries",*(itr++));
//  EXPECT_EQ(tok.end(), itr);
}

TEST(Tokenizer, Separator) {
  using tokenizer = tokenizer<char_separator<char>>;
  string s = "Boost C++ Libraries";

  char_separator<char> sep{" "};
  tokenizer tok{s, sep};

  auto itr = tok.begin();
  EXPECT_EQ("Boost", *(itr++));
  EXPECT_EQ("C++", *(itr++));
  EXPECT_EQ("Libraries", *(itr++));
  EXPECT_EQ(tok.end(), itr);
}

TEST(Tokenizer, SeparatorWhiteSpaceAndPuns) {
  using tokenizer = tokenizer<char_separator<char>>;

  string s = "Boost C++ Libraries";

  char_separator<char> sep{" +"};
  tokenizer tok{s, sep};
  for (const auto &t:tok)
    cout << t << endl;
}

TEST(Tokenizer, KeepEmptyTokens) {
  using tokenizer = tokenizer<char_separator<char>>;

  string s = "Boost C++ Libraries";

  char_separator<char> sep{" ", "+", keep_empty_tokens};
  tokenizer tok{s, sep};
  for (const auto &t:tok)
    cout << t << endl;
}

TEST(Tokenizer, WideStrings) {
  using tokenizer = tokenizer<char_separator<wchar_t>, wstring::const_iterator, wstring>;
  wstring s = L"Boost C++ Libraries";
  char_separator<wchar_t> sep{L" "};
  tokenizer tok{s, sep};
  for (const auto &t: tok)
    wcout << t << endl;
}
