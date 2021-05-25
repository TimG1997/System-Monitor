#include <helper/cpu_helper.h>

float CpuHelper::ConvertHertzToSeconds(long time_in_jiffies, long clock_ticks_per_second) {
  return (float) time_in_jiffies / (float) clock_ticks_per_second;
}

float CpuHelper::CalculateCpuUsage(long start_time, long uptime, long active_jiffies, long clock_ticks_per_second) {
  long time_since_start = uptime - (ConvertHertzToSeconds(start_time, clock_ticks_per_second));
  long total_time_seconds =
      ConvertHertzToSeconds(active_jiffies, clock_ticks_per_second);
  float cpu_usage = 100 * ((float) total_time_seconds / time_since_start);

   return cpu_usage;
}

long CpuHelper::ClockTicksPerSecond() {
  return sysconf(_SC_CLK_TCK);
}
float CpuHelper::CalculateCpuUtilization(long last_idle_jiffies,
                                         long last_total_jiffies,
                                         long idle_jiffies,
                                         long total_jiffies) {
  float diff_idle = (float) idle_jiffies - (float) last_idle_jiffies;
  float diff_total = (float) total_jiffies - (float) last_total_jiffies;

  float utilization = (diff_total - diff_idle) / diff_total;

  return utilization;
}
