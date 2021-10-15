//
// Created by rick on 2021/10/14.
//
#include <gtest/gtest.h>
#include <unicode/uloc.h>
#include <unicode/utf8.h>
#include <cassert>
#include <iostream>
#include <memory>
#include <string>

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

TEST(ULoc, Canonicalization) {
  char result[ULOC_FULLNAME_CAPACITY] = {'\0'};
  UErrorCode uErr = U_ZERO_ERROR;
  uloc_canonicalize("en_US@VARIANT", result, ULOC_FULLNAME_CAPACITY, &uErr);
  EXPECT_STREQ("en_US_VARIANT", result);

  uloc_canonicalize("en_US.utf8", result, ULOC_FULLNAME_CAPACITY, &uErr);
  EXPECT_STREQ("en_US", result);

  uloc_canonicalize("ZH_cn", result, ULOC_FULLNAME_CAPACITY, &uErr);
  EXPECT_STREQ("zh_CN", result);

  uloc_canonicalize("zh-CN", result, ULOC_FULLNAME_CAPACITY, &uErr);
  EXPECT_STREQ("zh_CN", result);

  uloc_canonicalize("he-IL-u-ca-hebrew-tz-jeruslm", result, ULOC_FULLNAME_CAPACITY, &uErr);
  EXPECT_STREQ("zh_CN", result);


//  uErr = U_ZERO_ERROR;
//  uloc_canonicalize("und-Zzzz",result,ULOC_FULLNAME_CAPACITY,&uErr);
//  EXPECT_STREQ("und_Zzzz",result);

}

TEST(ULoc,AddLikelySubtags) {
  char likely[ULOC_FULLNAME_CAPACITY] = {'\0'};
  char result[ULOC_FULLNAME_CAPACITY] = {'\0'};

  UErrorCode uErr = U_ZERO_ERROR;
  uloc_addLikelySubtags("zh_CN",likely,ULOC_FULLNAME_CAPACITY,&uErr);
  EXPECT_STREQ("zh_Hans_CN",likely);

  uloc_addLikelySubtags("und",likely,ULOC_FULLNAME_CAPACITY,&uErr);
  EXPECT_STREQ("en_Latn_US",likely);

  uErr = U_ZERO_ERROR;
  uloc_toLanguageTag(likely,result,ULOC_FULLNAME_CAPACITY,false,&uErr);
  EXPECT_STREQ("en-Latn-US",result);

  uErr = U_ZERO_ERROR;
  uloc_toLanguageTag("he_IL@calendar=hebrew;timezone=Asia/Jerusalem",result,ULOC_FULLNAME_CAPACITY,false,&uErr);
  EXPECT_STREQ("he-IL-u-ca-hebrew-tz-jeruslm",result);


}