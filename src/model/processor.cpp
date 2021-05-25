#include "model/processor.h"
#include "helper/cpu_helper.h"

Processor::Processor() : last_idle(0), last_total(0) {
}

float Processor::Utilization() {
  long active_jiffies = LinuxParser::ActiveJiffies();
  long idle_jiffies = LinuxParser::IdleJiffies();
  long total_jiffies = active_jiffies + idle_jiffies;

  float utilization = CpuHelper::CalculateCpuUtilization(this->last_idle, this->last_total, idle_jiffies, total_jiffies);

  this->last_idle = idle_jiffies;
  this->last_total = total_jiffies;

  return utilization;
}