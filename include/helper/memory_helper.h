#ifndef MONITOR_MEMORY_HELPER_H
#define MONITOR_MEMORY_HELPER_H

#include <string>

namespace MemoryHelper {

enum MemoryUnit {
  KB = 0,
  MB = 1,
  GB = 2,
  TB = 3
};

float CalculateTotalUsedMemory(long total_memory, long free_memory);
float ConvertKBToMB(float kilobytes);
int CompareRamInMBStrings(std::string ram_one, std::string ram_two);
}

#endif  // MONITOR_MEMORY_HELPER_H
