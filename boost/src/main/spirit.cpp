//
// Created by rick on 2017/7/23.
//

#include <gtest/gtest.h>
#include <boost/spirit/include/qi.hpp>
#include <boost/format.hpp>
#include <string>

using namespace std;
using namespace boost;
using namespace boost::spirit;

TEST(Spirit, Parser) {
  string s = "123981798123abcd";
  auto it = s.begin();
  bool match = qi::parse(it, s.end(), ascii::digit);

  EXPECT_TRUE(match);
  if (it != s.end())
    cout << string{it, s.end()} << endl;
}

TEST(Spirit, PhaseParser) {
  string s = "   13";
  auto it = s.begin();
  bool match = qi::phrase_parse(it, s.end(), ascii::digit, ascii::space);

  EXPECT_TRUE(match);
  EXPECT_TRUE(it != s.end());
  EXPECT_EQ("3", string(it, s.end()));
}

TEST(Spirit, Parsers) {
  string s = "123";
  auto it = s.begin();
  bool match = qi::phrase_parse(it, s.end(), ascii::digit >> ascii::digit, ascii::space);

  cout << boolalpha << match << endl;
  if (it != s.end())
    cout << string{it, s.end()} << endl;

}

TEST(Spirit, lexeme) {
  string s = "12x";
  auto it = s.begin();
  bool match = qi::phrase_parse(it, s.end(), qi::lexeme[ascii::digit >> ascii::digit], ascii::space);
  cout << boolalpha << match << endl;
  if (it != s.end())
    cout << string{it, s.end()} << endl;
}

TEST(Spirit, RexLike) {
  string s = "12  3abc";
  auto it = s.begin();
  bool match = qi::phrase_parse(it, s.end(), qi::lexeme[+ascii::digit], ascii::space);
  cout << boolalpha << match << endl;
  if (it != s.end())
    cout << string{it, s.end()} << endl;

  it = s.begin();
  match = qi::phrase_parse(it, s.end(), +ascii::digit, ascii::space);
  cout << boolalpha << match << endl;
  if (it != s.end())
    cout << string{it, s.end()} << endl;
}

TEST(Spirit, NumberParser) {
  string s = "123.123ba";
  auto it = s.begin();
  bool match = qi::phrase_parse(it, s.end(), qi::int_, ascii::space);
  cout << boolalpha << match << endl;
  cout << string{it, s.end()} << endl;

  it = s.begin();
  match = qi::phrase_parse(it, s.end(), qi::float_, ascii::space);
  cout << boolalpha << match << endl;
  cout << string{it, s.end()} << endl;

}

TEST(Spirit, Actions) {
  string s = "123ggg";
  auto it = s.begin();

  int result;
  bool match = qi::phrase_parse(it, s.end(),
                                qi::int_[([&result](int i) { result = i; })], ascii::space);
  cout << boolalpha << match << endl;
  cout << format{"%1%"} % result << endl;
  cout << string{it, s.end()} << endl;
}

TEST(Spirit, Attributes) {
  string s = "123ggg";
  auto it = s.begin();
  int i;
  if (qi::phrase_parse(it, s.end(), qi::int_, ascii::space, i))
    cout << i << endl;
}

TEST(Spirit, SevelIntValues) {
  string s = "123,324,123";
  auto it = s.begin();
  list<int> v;

  if (qi::phrase_parse(it, s.end(), qi::int_ % ',', ascii::space, v)) {
    ostream_iterator<int> out{cout, ";"};
    copy(v.begin(), v.end(), out);
  }
}

TEST(Spirit, Rules) {
  string s = "123,123";
  auto it = s.begin();
  qi::rule<string::iterator, vector<int>(), ascii::space_type> values = qi::int_ % ',';
  vector<int> v;

  if (qi::phrase_parse(it, s.end(), values, ascii::space, v)) {
    ostream_iterator<int> out{cout, "\n"};
    copy(v.begin(), v.end(), out);
  }
}

TEST(Spirit, NestedRules) {
  string s = "123,true,false,123,false";
  auto it = s.begin();

  qi::rule<string::iterator, variant<int, bool>(), ascii::space_type> value = qi::int_ | qi::bool_;
  qi::rule<string::iterator, vector<variant<int, bool>>(), ascii::space_type> values = value % ',';
  vector<variant<int, bool>> v;
  if (qi::phrase_parse(it, s.end(), values, ascii::space, v)) {
    for (const auto &elem: v)
      apply_visitor([](auto t) {
        cout << boolalpha << t << endl;
      }, elem);
  }

}