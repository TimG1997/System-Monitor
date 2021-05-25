#include "gtest/gtest.h"
#include "helper/cpu_helper.h"

class CpuHelperTest : public ::testing::Test{};

TEST_F(CpuHelperTest, TestConvertHertzToSeconds){
  long time_in_jiffies = 1000;
  long clock_ticks_per_second = 100;

  float expected_seconds = 10.0;
  float actutal_seconds =
      CpuHelper::ConvertHertzToSeconds(time_in_jiffies, clock_ticks_per_second);

  EXPECT_EQ(expected_seconds, actutal_seconds);
}

TEST_F(CpuHelperTest, TestCalculateCpuUsage){
  long active_jiffies = 6500;
  long uptime = 1200, start_time = 4000;
  long clock_ticks_per_second = 10;

  float expected_cpu_usage = 81.25f;
  float actual_cpu_usage = CpuHelper::CalculateCpuUsage(start_time, uptime, active_jiffies, clock_ticks_per_second);

  EXPECT_EQ(expected_cpu_usage, actual_cpu_usage);
}

TEST_F(CpuHelperTest, TestCalculateCpuUtilization){
  long last_idle_jiffies = 1000;
  long last_total_jiffies = 1200;

  long idle_jiffies = 1100;
  long total_jiffies = 1400;

  float expected_utilization = 0.5f;
  float actual_utilization = CpuHelper::CalculateCpuUtilization(last_idle_jiffies, last_total_jiffies, idle_jiffies, total_jiffies);

  EXPECT_EQ(expected_utilization, actual_utilization);
}