#ifndef SYSTEM_PARSER_H
#define SYSTEM_PARSER_H

#include <fstream>
#include <regex>
#include <string>
#include <utility>

#include "helper/cpu_helper.h"
#include "linux_info_file_locations.h"

class LinuxParser {
 public:

  // default linux file locations
  LinuxParser();

  // custom file locations (useful for testing)
  LinuxParser(std::string proc_dir, std::string cmd_line_filename,
              std::string cpu_info_filename, std::string status_filename,
              std::string stat_filename, std::string up_time_filename,
              std::string mem_info_filename, std::string version_filename,
              std::string os_path, std::string password_path);

  // System
  float MemoryUtilization();
  long UpTime();
  std::vector<int> Pids();
  int TotalProcesses();
  int RunningProcesses();
  std::string OperatingSystem();
  std::string Kernel();

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
  std::vector<std::string> CpuUtilization(long clock_ticks_per_second = CpuHelper::ClockTicksPerSecond());
  long Jiffies();
  long ActiveJiffies();
  long IdleJiffies();

  // Processes
  long ActiveJiffies(int pid);
  std::string Command(int pid);
  std::string Ram(int pid);
  std::string Uid(int pid);
  std::string User(int pid);
  long int UpTime(int pid);
  long int StartTime(int pid);

 private:
  const std::string kProcDirectory;
  const std::string kCmdlineFilename;
  const std::string kCpuinfoFilename;
  const std::string kStatusFilename;
  const std::string kStatFilename;
  const std::string kUptimeFilename;
  const std::string kMeminfoFilename;
  const std::string kVersionFilename;
  const std::string kOSPath;
  const std::string kPasswordPath;

};  // namespace LinuxParser

#endif