#include "../include/model/process.h"
#include "gtest/gtest.h"

class ProcessTest : public ::testing::Test {};

TEST_F(ProcessTest, TestLessThanOperator) {
  Process process_one{1};
  Process process_two{2};

  EXPECT_TRUE(process_two < process_one);
}