#include "../include/memory_helper.h"
#include "gtest/gtest.h"

class MemoryCalculatorTest : public ::testing::Test{};

TEST_F(MemoryCalculatorTest, TestCalculateTotalUsedMemory){
  long total_memory = 100;
  long free_memory = 25;

  float expected_memory_usage_percentage = 0.75;
  float actual_memory_usage_percentage =
      MemoryHelper::CalculateTotalUsedMemory(total_memory, free_memory);

  EXPECT_FLOAT_EQ(expected_memory_usage_percentage, actual_memory_usage_percentage);
}

TEST_F(MemoryCalculatorTest, TestConvertKBToMB){
  float kilobytes = 2049;
  float expected_megabytes = 2.049;
  float actual_megabytes = MemoryHelper::ConvertKBToMB(kilobytes);

  EXPECT_FLOAT_EQ(expected_megabytes, actual_megabytes);
}