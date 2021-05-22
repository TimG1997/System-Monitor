#ifndef MONITOR_MEMORY_HELPER_H
#define MONITOR_MEMORY_HELPER_H

namespace MemoryHelper {
float CalculateTotalUsedMemory(long total_memory, long free_memory);
float ConvertKBToMB(float kilobytes);
}

#endif  // MONITOR_MEMORY_HELPER_H
