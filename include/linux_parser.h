#ifndef SYSTEM_PARSER_H
#define SYSTEM_PARSER_H

#include <fstream>
#include <regex>
#include <string>

#include "linux_info_file_locations.h"
#include "cpu_helper.h"

const std::string kProcDirectory{"/proc/"};
const std::string kCmdlineFilename{"/cmdline"};
const std::string kCpuinfoFilename{"/cpuinfo"};
const std::string kStatusFilename{"/status"};
const std::string kStatFilename{"/stat"};
const std::string kUptimeFilename{"/uptime"};
const std::string kMeminfoFilename{"/meminfo"};
const std::string kVersionFilename{"/version"};
const std::string kOSPath{"/etc/os-release"};
const std::string kPasswordPath{"/etc/passwd"};

namespace LinuxParser {
// System
float MemoryUtilization(std::string mem_info_path = LinuxInfoFileLocations::kProcDirectory + kMeminfoFilename);
long UpTime(std::string uptime_path = LinuxInfoFileLocations::kProcDirectory + LinuxInfoFileLocations::kUptimeFilename);
std::vector<int> Pids();
int TotalProcesses(std::string stat_path = LinuxInfoFileLocations::kProcDirectory + LinuxInfoFileLocations::kStatFilename);
int RunningProcesses(std::string stat_path = LinuxInfoFileLocations::kProcDirectory + LinuxInfoFileLocations::kStatFilename);
std::string OperatingSystem(std::string os_path = LinuxInfoFileLocations::kOSPath);
std::string Kernel(std::string kernel_path = LinuxInfoFileLocations::kProcDirectory + LinuxInfoFileLocations::kVersionFilename);

// CPU
enum CPUStates {
  kUser_ = 0,
  kNice_,
  kSystem_,
  kIdle_,
  kIOwait_,
  kIRQ_,
  kSoftIRQ_,
  kSteal_,
  kGuest_,
  kGuestNice_
};
std::vector<std::string> CpuUtilization(std::vector<int> pids = Pids(), std::string proc_dir = LinuxInfoFileLocations::kProcDirectory, std::string stat_path =LinuxInfoFileLocations::kStatFilename, std::string uptime_path = LinuxInfoFileLocations::kUptimeFilename, long clock_ticks_per_second = CpuHelper::ClockTicksPerSecond());
long Jiffies(std::string stat_path = LinuxInfoFileLocations::kProcDirectory + LinuxInfoFileLocations::kStatFilename);
long ActiveJiffies(std::string stat_path = LinuxInfoFileLocations::kProcDirectory + LinuxInfoFileLocations::kStatFilename);
long IdleJiffies(std::string stat_path = LinuxInfoFileLocations::kProcDirectory + LinuxInfoFileLocations::kStatFilename);

// Processes
long ActiveJiffies(int pid, std::string proc_dir = LinuxInfoFileLocations::kProcDirectory, std::string stat_filename = LinuxInfoFileLocations::kStatFilename);
std::string Command(int pid, std::string proc_dir = LinuxInfoFileLocations::kProcDirectory, std::string command_filename = LinuxInfoFileLocations::kCmdlineFilename);
std::string Ram(int pid, std::string proc_dir = LinuxInfoFileLocations::kProcDirectory, std::string status_filename = LinuxInfoFileLocations::kStatusFilename);
std::string Uid(int pid, std::string proc_dir = LinuxInfoFileLocations::kProcDirectory, std::string command_filename = LinuxInfoFileLocations::kStatusFilename);
std::string User(int pid);
long int UpTime(int pid, std::string proc_dir = LinuxInfoFileLocations::kProcDirectory, std::string uptime_path = LinuxInfoFileLocations::kUptimeFilename, std::string command_filename = LinuxInfoFileLocations::kStatFilename);
long int StartTime(int pid, std::string proc_dir = LinuxInfoFileLocations::kProcDirectory, std::string command_filename = LinuxInfoFileLocations::kStatFilename);
};  // namespace LinuxParser

#endif