#include "model/system.h"

#include <set>

using std::set;
using std::size_t;
using std::string;
using std::vector;

System::System(){
  this->processes_.clear();
  vector<int> pids = LinuxParser::Pids();

  for (auto &pid : pids) {
    this->processes_.emplace_back(Process(pid));
  }

  std::sort(this->processes_.begin(), this->processes_.end());
}

Processor& System::Cpu() {return this->cpu_; }

vector<Process>& System::Processes() {
}

std::string System::Kernel() { return LinuxParser::Kernel(); }

float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

long int System::UpTime() { return LinuxParser::UpTime(); }