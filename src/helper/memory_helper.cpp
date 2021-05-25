#include "helper/memory_helper.h"

#include <iostream>

#include "helper/string_helper.h"

float MemoryHelper::CalculateTotalUsedMemory(long total_memory,
                                             long free_memory) {
  return (total_memory - free_memory) / (float)total_memory;
}

long MemoryHelper::ConvertKBToMB(long kilobytes) { return kilobytes / 1000; }

int MemoryHelper::CompareRamInMBStrings(std::string ram_one,
                                        std::string ram_two) {
  long ram_one_num =
      stol(StringHelper::GetStringLeftFromCharacter(ram_one, ' '));
  long ram_two_num =
      stol(StringHelper::GetStringLeftFromCharacter(ram_two, ' '));

  if (ram_one_num > ram_two_num) {
    return 1;
  } else if (ram_one_num < ram_two_num) {
    return -1;
  } else {
    return 0;
  }
}