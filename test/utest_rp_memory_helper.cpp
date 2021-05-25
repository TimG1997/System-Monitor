#include <string>

#include "gtest/gtest.h"
#include "helper/memory_helper.h"

using namespace std;

class MemoryHelperTest : public ::testing::Test{};

TEST_F(MemoryHelperTest, TestCalculateTotalUsedMemory){
  long total_memory = 100;
  long free_memory = 25;

  float expected_memory_usage_percentage = 0.75;
  float actual_memory_usage_percentage =
      MemoryHelper::CalculateTotalUsedMemory(total_memory, free_memory);

  EXPECT_FLOAT_EQ(expected_memory_usage_percentage, actual_memory_usage_percentage);
}

TEST_F(MemoryHelperTest, TestConvertKBToMB){
  float kilobytes = 2049;
  long expected_megabytes = 2;
  long actual_megabytes = MemoryHelper::ConvertKBToMB(kilobytes);

  EXPECT_FLOAT_EQ(expected_megabytes, actual_megabytes);
}

TEST_F(MemoryHelperTest, TestCompareRamInMBStrings){
  string ram_string_one = "10 MB";
  string ram_string_two = "20 MB";

  int ram_two_is_greater = 1;
  int ram_one_is_smaller = -1;
  int ram_is_equal = 0;

  EXPECT_EQ(ram_one_is_smaller, MemoryHelper::CompareRamInMBStrings(ram_string_one, ram_string_two));
  EXPECT_EQ(ram_two_is_greater, MemoryHelper::CompareRamInMBStrings(ram_string_two, ram_string_one));
  EXPECT_EQ(ram_is_equal, MemoryHelper::CompareRamInMBStrings(ram_string_one, ram_string_one));
}