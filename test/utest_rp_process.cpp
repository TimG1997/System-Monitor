#include "../include/model/process.h"
#include "gtest/gtest.h"

class ProcessTest : public ::testing::Test {};

TEST_F(ProcessTest, TestLessThanOperator) {
  Process process_one{1, "root", "/command", 150.0, "10 MB", 10};
  Process process_two{1, "root", "/command", 150.0, "5 MB", 10};
  Process process_three{1, "root", "/command", 100.0, "10 MB", 10};

  EXPECT_TRUE(process_two < process_one);
  EXPECT_TRUE(process_three < process_two);
}