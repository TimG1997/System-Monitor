#ifndef SYSTEM_PARSER_H
#define SYSTEM_PARSER_H

#include <fstream>
#include <regex>
#include <string>
#include <utility>

#include "helper/cpu_helper.h"
#include "linux_info_file_locations.h"

using namespace LinuxInfoFileLocations;

namespace LinuxParser {

// System
float MemoryUtilization(const std::string& proc_dir = kProcDirectory, const std::string& meminfo_filename = kMeminfoFilename);
long UpTime(const std::string& proc_dir = kProcDirectory, const std::string& up_time_filename = kUptimeFilename);
std::vector<int> Pids(const std::string& proc_dir = kProcDirectory);
int TotalProcesses(const std::string& proc_dir = kProcDirectory, const std::string& state_filename = kStatFilename);
int RunningProcesses(const std::string& proc_dir = kProcDirectory, const std::string& state_filename = kStatFilename);
std::string OperatingSystem(const std::string& os_path = kOSPath);
std::string Kernel(const std::string& proc_dir = kProcDirectory, const std::string& version_filename = kVersionFilename
);

// CPU
enum CPUStates {
  kUser_ = 0,
  kNice_ = 1,
  kSystem_ = 2,
  kIdle_ = 3,
  kIOwait_ = 4,
  kIRQ_ = 5,
  kSoftIRQ_ = 6,
  kSteal_ = 7,
  kGuest_ = 8,
  kGuestNice_ = 9
};

enum ProcCPUStates {
  kUtime_ = 14,
  kStime_ = 15,
  kCutime_ = 16,
  kCstime_ = 17,
  kStarttime_ = 22
};
std::vector<long> CpuUtilization(
    int pid, const std::string& proc_dir = kProcDirectory, const std::string& stat_filename = kStatFilename, const std::string& uptime_filename = kUptimeFilename);
long Jiffies(const std::string& proc_dir = kProcDirectory, const std::string& stat_filename = kStatFilename);
long ActiveJiffies(const std::string& proc_dir = kProcDirectory, const std::string& stat_filename = kStatFilename);
long IdleJiffies(const std::string& proc_dir = kProcDirectory, const std::string& stat_filename = kStatFilename);

// Processes
long ActiveJiffies(int pid, const std::string& proc_dir = kProcDirectory, const std::string& stat_filename = kStatFilename);
std::string Command(int pid, const std::string& proc_dir = kProcDirectory, const std::string& cmdline_filename = kCmdlineFilename);
std::string Ram(int pid, const std::string& proc_dir = kProcDirectory, const std::string& status_filename = kStatusFilename);
std::string Uid(int pid, const std::string& proc_dir = kProcDirectory, const std::string& status_filename = kStatusFilename);
std::string User(int pid, const std::string& proc_dir = kProcDirectory, const std::string& status_filename = kStatusFilename, const std::string& passwd_path = kPasswordPath);
long int UpTime(int pid, const std::string& proc_dir = kProcDirectory, const std::string& up_time_filename = kUptimeFilename, const std::string& stat_filename = kStatFilename);
long int StartTime(int pid, const std::string& proc_dir = kProcDirectory, const std::string& stat_filename = kStatFilename);

};  // namespace LinuxParser

#endif