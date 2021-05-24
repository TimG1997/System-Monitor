#ifndef MONITOR_CPU_HELPER_H
#define MONITOR_CPU_HELPER_H

#include <unistd.h>

namespace CpuHelper {
long ClockTicksPerSecond();
long ConvertHertzToSeconds(long time_in_jiffies,
                            long clock_ticks_per_second = ClockTicksPerSecond());
long CalculateCpuUsage(long start_time, long uptime, long active_jiffies, long clock_ticks_per_second = ClockTicksPerSecond());
}

#endif  // MONITOR_CPU_HELPER_H
