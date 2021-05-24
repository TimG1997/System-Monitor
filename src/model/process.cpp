#include "model/process.h"

#include <unistd.h>

#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "../../include/helper/memory_helper.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid, std::string user, std::string command,
                 float cpu_utilization, std::string ram, long up_time)
    : pid(pid),
      user(user),
      command(command),
      cpu_utilization(cpu_utilization),
      ram(ram),
      up_time(up_time) {}

int Process::Pid() { return this->pid; }

float Process::CpuUtilization() { return this->cpu_utilization; }

string Process::Command() { return this->command; }

string Process::Ram() { return this->ram; }

string Process::User() { return this->user; }

long int Process::UpTime() { return this->up_time; }

bool Process::operator<(Process const& other_process) const {
  if (this->cpu_utilization < other_process.cpu_utilization) {
    return true;
  } else if (this->cpu_utilization > other_process.cpu_utilization) {
    return false;
  } else {
    int ram_compare_result =
        MemoryHelper::CompareRamInMBStrings(this->ram, other_process.ram);
    if (ram_compare_result == 1) {
      return false;
    } else {
      // return true, when the ram consumption is equal or less
      return true;
    }
  }
}