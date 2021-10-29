//
// Created by rick on 2021/10/27.
//
#include <gtest/gtest.h>
#include <rapidcheck/gtest.h>
#include <harfbuzz/hb.h>
int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

TEST(HarfbuzzCommon, script) {
  auto script = hb_script_from_string("Hans", -1);
  EXPECT_EQ(HB_SCRIPT_HAN, script);

  script = hb_script_from_string("Hant",-1);
  EXPECT_EQ(HB_SCRIPT_HAN, script);

  script = hb_script_from_string("Zyyy",-1);
  EXPECT_NE(HB_SCRIPT_HAN, script);
}

TEST(HarfbuzzCommon,language){
  auto lang = hb_language_from_string("zh",-1);
  EXPECT_STREQ("zh",hb_language_to_string(lang));
}

RC_GTEST_PROP(MyTestCase,copyOfStringIsIdenticalToOriginal,
              (const std::string &str)) {
  RC_PRE((str.size() % 2) == 0);
//  const auto strCopy = str;
//  RC_ASSERT(strCopy == str);
}

// Should obviously fail:
RC_GTEST_PROP(MyTestCase, dividingByTenMakesAllNumbersEqual, (int a, int b)) {
  RC_ASSERT((a / 10) == (b / 10));
}