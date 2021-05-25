#include "model/process.h"
#include "helper/cpu_helper.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid) : pid_(pid) {
  this->pid_  = pid;
  this->cpu_utilization_ = Process::CpuUtilization();
}

int Process::Pid() { return this->pid_; }

float Process::CpuUtilization() {
  long active_jiffies = LinuxParser::ActiveJiffies(this->pid_) / CpuHelper::ClockTicksPerSecond();
  long uptime = LinuxParser::UpTime(this->pid_);

  return (float)active_jiffies / uptime; }

string Process::Command() { return LinuxParser::Command(this->pid_); }

string Process::Ram() {
  return LinuxParser::Ram(this->pid_);
}

string Process::User() { return LinuxParser::User(this->pid_); }

long int Process::UpTime() { return LinuxParser::UpTime(this->pid_); }

bool Process::operator<(Process const& other_process) const {
  return this->cpu_utilization_ > other_process.cpu_utilization_;
}