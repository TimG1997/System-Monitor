#include <string>
#include <vector>

#include "gtest/gtest.h"
#include "../include/string_helper.h"

using namespace std;

class StringHelperTest : public ::testing::Test{};

TEST_F(StringHelperTest, TestGetElement){
  string input = "1015 (init) S 1 1015 1015 0 -1 4211008 16 0 0 0 0 0 0 0 20 0 1 0 46397 1671168 142 18446744073709551615 1 1 0 0 0 0 65536 2147024638 65536 0 0 0 17 4 0 0 0 0 0 0 0 0 0 0 0 0 0";

  int expected_element = 46397;
  int actual_element = StringHelper::GetElement<int>(input, 22);

  EXPECT_EQ(expected_element, actual_element);
}

TEST_F(StringHelperTest, TestExceptionThrownWrongIndex){
  try{
    StringHelper::GetElement<int>("Test", 0);
    FAIL();
  } catch(const runtime_error &e){
    EXPECT_STREQ("Please provide an index greater than 0.", e.what());
  }
}

TEST_F(StringHelperTest, TestExceptionThrownEmptyString){
  try{
    StringHelper::GetElement<int>("", 10);
    FAIL();
  } catch(const invalid_argument &e){
    EXPECT_STREQ("The provided string is empty.", e.what());
  }
}

TEST_F(StringHelperTest, TestGetElements){
  string input = "1015 (init) S 1 1015 1015 0 -1 4211008 16 0 0 0 0 0 0 0 20 0 1 0 46397 1671168 142 18446744073709551615 1 1 0 0 0 0 65536 2147024638 65536 0 0 0 17 4 0 0 0 0 0 0 0 0 0 0 0 0 0";
  vector<int> input_indezes{1, 4, 5, 8};

  vector<int> actual_elements = StringHelper::GetElements<int>(input, input_indezes);

  EXPECT_EQ(1015, actual_elements[0]);
  EXPECT_EQ(1, actual_elements[1]);
  EXPECT_EQ(1015, actual_elements[2]);
  EXPECT_EQ(-1, actual_elements[3]);
}