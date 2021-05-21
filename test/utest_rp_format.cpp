#include <string>

#include "gtest/gtest.h"
#include "../include/format.h"

using namespace std;

class FormatTest : public ::testing::Test {};

TEST_F(FormatTest, TestAddLeadingZero){
  string expected_without_leading_zero = "11";
  string actual_without_leading_zero = Format::AddLeadingZero(11);

  string expected_with_leading_zero = "01";
  string actual_with_leading_zero = Format::AddLeadingZero(1);

  EXPECT_EQ(expected_without_leading_zero, actual_without_leading_zero);
  EXPECT_EQ(expected_with_leading_zero, actual_with_leading_zero);
}

TEST_F(FormatTest, TestElaspedTime) {
  string expected_hhmmss = "01:30:45";
  string actual_hhmmss = Format::ElapsedTime(5445);

  EXPECT_EQ(expected_hhmmss, actual_hhmmss);
}
