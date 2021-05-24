#include <string>
#include <vector>

#include "gtest/gtest.h"
#include "helper/string_helper.h"

using namespace std;

class StringHelperTest : public ::testing::Test {};

TEST_F(StringHelperTest, TestGetElement) {
  string input =
      "1015 (init) S 1 1015 1015 0 -1 4211008 16 0 0 0 0 0 0 0 20 0 1 0 46397 "
      "1671168 142 18446744073709551615 1 1 0 0 0 0 65536 2147024638 65536 0 0 "
      "0 17 4 0 0 0 0 0 0 0 0 0 0 0 0 0";

  int expected_element = 46397;
  int actual_element = StringHelper::GetElement<int>(input, 22);

  EXPECT_EQ(expected_element, actual_element);
}

TEST_F(StringHelperTest, TestExceptionThrownWrongIndex) {
  try {
    StringHelper::GetElement<int>("Test", 0);
    FAIL();
  } catch (const runtime_error &e) {
    EXPECT_STREQ("Please provide an index greater than 0.", e.what());
  }
}

TEST_F(StringHelperTest, TestExceptionThrownEmptyString) {
  try {
    StringHelper::GetElement<int>("", 10);
    FAIL();
  } catch (const invalid_argument &e) {
    EXPECT_STREQ("The provided string is empty.", e.what());
  }
}

TEST_F(StringHelperTest, TestGetElements) {
  string input =
      "1015 (init) S 1 1015 1015 0 -1 4211008 16 0 0 0 0 0 0 0 20 0 1 0 46397 "
      "1671168 142 18446744073709551615 1 1 0 0 0 0 65536 2147024638 65536 0 0 "
      "0 17 4 0 0 0 0 0 0 0 0 0 0 0 0 0";
  vector<int> input_indezes{1, 4, 5, 8};

  vector<int> actual_elements =
      StringHelper::GetElements<int>(input, input_indezes);

  int expected_first_element = 1015;
  int expected_second_element = 1;
  int expected_third_element = 1015;
  int expected_fourth_element = -1;

  EXPECT_EQ(expected_first_element, actual_elements[0]);
  EXPECT_EQ(expected_second_element, actual_elements[1]);
  EXPECT_EQ(expected_third_element, actual_elements[2]);
  EXPECT_EQ(expected_fourth_element, actual_elements[3]);
}

TEST_F(StringHelperTest, TestGetElementsWithSkippingElements){
  string input = "element_to_skip 1 2 3";
  vector<int> input_indezes{0,1,2};

  int skip_first_element = 1;
  vector<int> actual_elements = StringHelper::GetElements<int>(input, input_indezes, skip_first_element);

  int expected_first_element = 1;
  int expected_second_element = 2;
  int expected_third_element = 3;

  EXPECT_EQ(expected_first_element, actual_elements[0]);
  EXPECT_EQ(expected_second_element, actual_elements[1]);
  EXPECT_EQ(expected_third_element, actual_elements[2]);
}

TEST_F(StringHelperTest, TestSplitString) {
  string input_string = "root:x:0:0:root:/root:/bin/bash";
  char split_character = ':';

  vector<string> expected_elements{"root", "x",     "0",        "0",
                                   "root", "/root", "/bin/bash"};
  vector<string> actual_elements =
      StringHelper::SplitString(input_string, split_character);

  EXPECT_EQ(expected_elements, actual_elements);
}

TEST_F(StringHelperTest, TestGetStringBetweenCharacter) {
  string input_string = "root:x:0:0:root:/root:/bin/bash";
  char split_character = ':';

  string expected_string = "root";
  string actual_String = StringHelper::GetStringLeftFromCharacter(input_string, split_character);

  EXPECT_EQ(expected_string, actual_String);
}

TEST_F(StringHelperTest, TestContains){
  string input = "10 MB";
  string string_one_to_check = "MB";
  string string_two_to_check = "GB";

  bool contains_string_one = StringHelper::Contains(input, string_one_to_check);
  bool contains_string_two = StringHelper::Contains(input, string_two_to_check);

  EXPECT_TRUE(contains_string_one);
  EXPECT_FALSE(contains_string_two);
}