#include "model/process.h"
#include "helper/cpu_helper.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid) : pid_(pid) {
  this->determineCpuUtilization();
}

int Process::Pid() { return this->pid_; }

float Process::CpuUtilization() {
  long active_jiffies = LinuxParser::ActiveJiffies(this->pid_);
  long uptime = LinuxParser::UpTime(this->pid_);

  float active =  (float)(active_jiffies / uptime);

  return active / CpuHelper::ClockTicksPerSecond(); }

string Process::Command() { return LinuxParser::Command(this->pid_); }

string Process::Ram() {
  return LinuxParser::Ram(this->pid_);
}

string Process::User() { return LinuxParser::User(this->pid_); }

long int Process::UpTime() { return LinuxParser::UpTime(this->pid_); }

bool Process::operator<(Process const& other_process) const {
  return this->cpu_utilization_ > other_process.cpu_utilization_;
}

void Process::determineCpuUtilization() {
  vector<long> cpu_utilizations = LinuxParser::CpuUtilization(this->pid_);

  long up_time = cpu_utilizations[0];
  float active_jiffies_seconds =
      CpuHelper::ConvertHertzToSeconds(cpu_utilizations[1]);
  float start_time_seconds =
      CpuHelper::ConvertHertzToSeconds(cpu_utilizations[2]);

  float total_cpu_seconds = up_time - start_time_seconds;
  float active_utilization_seconds =
      (float)active_jiffies_seconds / (float)total_cpu_seconds;

  this->cpu_utilization_ = active_utilization_seconds;
}