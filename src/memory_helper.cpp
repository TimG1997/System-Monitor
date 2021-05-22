#include <iostream>

#include "memory_helper.h"

float MemoryHelper::CalculateTotalUsedMemory(long total_memory, long free_memory) {
  return (total_memory -  free_memory) / (float) total_memory;
}

float MemoryHelper::ConvertKBToMB(float kilobytes) {
  return kilobytes / 1000;
}