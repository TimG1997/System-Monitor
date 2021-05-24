#include <helper/cpu_helper.h>

long CpuHelper::ConvertHertzToSeconds(long time_in_jiffies, long clock_ticks_per_second) {
  return time_in_jiffies / clock_ticks_per_second;
}

long CpuHelper::CalculateCpuUsage(long start_time, long uptime, long active_jiffies, long clock_ticks_per_second) {
  long time_since_start = uptime - (ConvertHertzToSeconds(start_time, clock_ticks_per_second));
  long total_time_seconds =
      ConvertHertzToSeconds(active_jiffies, clock_ticks_per_second);
  long cpu_usage = 100 * ((float) total_time_seconds / time_since_start);

   return cpu_usage;
}

long CpuHelper::ClockTicksPerSecond() {
  return sysconf(_SC_CLK_TCK);
}