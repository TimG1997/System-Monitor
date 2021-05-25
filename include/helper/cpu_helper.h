#ifndef MONITOR_CPU_HELPER_H
#define MONITOR_CPU_HELPER_H

#include <unistd.h>

namespace CpuHelper {
long ClockTicksPerSecond();
float ConvertHertzToSeconds(long time_in_jiffies,
                            long clock_ticks_per_second = ClockTicksPerSecond());
float CalculateCpuUsage(long start_time, long uptime, long active_jiffies, long clock_ticks_per_second = ClockTicksPerSecond());
float CalculateCpuUtilization(long last_idle_jiffies, long last_total_jiffies, long idle_jiffies, long total_jiffies);
}

#endif  // MONITOR_CPU_HELPER_H
