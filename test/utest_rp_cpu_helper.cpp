#include "gtest/gtest.h"
#include "helper/cpu_helper.h"

class CpuHelperTest : public ::testing::Test{};

TEST_F(CpuHelperTest, TestConvertHertzToSeconds){
  long time_in_jiffies = 1000;
  long clock_ticks_per_second = 100;

  long expected_seconds = 10;
  long actutal_seconds =
      CpuHelper::ConvertHertzToSeconds(time_in_jiffies, clock_ticks_per_second);

  EXPECT_EQ(expected_seconds, actutal_seconds);
}

TEST_F(CpuHelperTest, TestCalculateCpuUsage){
  long active_jiffies = 6500;
  long uptime = 1200, start_time = 4000;
  long clock_ticks_per_second = 10;

  long expected_cpu_usage = 81;
  long actual_cpu_usage = CpuHelper::CalculateCpuUsage(start_time, uptime, active_jiffies, clock_ticks_per_second);

  EXPECT_EQ(expected_cpu_usage, actual_cpu_usage);
}