#include "linux_parser.h"

#include <dirent.h>
#include <unistd.h>

#include <execution>
#include <iomanip>
#include <numeric>
#include <sstream>
#include <string>
#include <vector>

#include "cpu_helper.h"
#include "memory_helper.h"
#include "string_helper.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

LinuxParser::LinuxParser()
    : kProcDirectory(LinuxInfoFileLocations::kProcDirectory),
      kCmdlineFilename(LinuxInfoFileLocations::kCmdlineFilename),
      kCpuinfoFilename(LinuxInfoFileLocations::kCpuinfoFilename),
      kStatusFilename(LinuxInfoFileLocations::kStatusFilename),
      kStatFilename(LinuxInfoFileLocations::kStatFilename),
      kUptimeFilename(LinuxInfoFileLocations::kUptimeFilename),
      kMeminfoFilename(LinuxInfoFileLocations::kMeminfoFilename),
      kVersionFilename(LinuxInfoFileLocations::kVersionFilename),
      kOSPath(LinuxInfoFileLocations::kOSPath),
      kPasswordPath(LinuxInfoFileLocations::kPasswordPath) {}

// custom file locations (useful for testing)
LinuxParser::LinuxParser(std::string proc_dir, std::string cmd_line_filename,
                         std::string cpu_info_filename,
                         std::string status_filename, std::string stat_filename,
                         std::string up_time_filename,
                         std::string mem_info_filename,
                         std::string version_filename, std::string os_path,
                         std::string password_path)
    : kProcDirectory(std::move(proc_dir)),
      kCmdlineFilename(std::move(cmd_line_filename)),
      kCpuinfoFilename(std::move(cpu_info_filename)),
      kStatusFilename(std::move(status_filename)),
      kStatFilename(std::move(stat_filename)),
      kUptimeFilename(std::move(up_time_filename)),
      kMeminfoFilename(std::move(mem_info_filename)),
      kVersionFilename(std::move(version_filename)),
      kOSPath(os_path),
      kPasswordPath(std::move(password_path)) {}

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::string combined_path = kProcDirectory + kVersionFilename;
  std::ifstream stream(combined_path);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

float LinuxParser::MemoryUtilization() {
  long total_memory, free_memory;

  string key, memory_number, unit;
  string line;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> memory_number >> unit;

      if (key == "MemTotal:") {
        total_memory = stol(memory_number);
      } else if (key == "MemFree:") {
        free_memory = stol(memory_number);
      }
    }
  }

  return MemoryHelper::CalculateTotalUsedMemory(total_memory, free_memory);
}

long LinuxParser::UpTime() {
  string uptime;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> uptime;
  }

  return stol(uptime);
}

long LinuxParser::Jiffies() {
  string line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);

    int skip_first_element = 1;
    long cpu_jiffies_sum = 0;
    vector<long> cpu_jiffies =
        StringHelper::GetElements<long>(line,
                                        vector<int>{
                                            CPUStates::kUser_,
                                            CPUStates::kNice_,
                                            CPUStates::kSystem_,
                                            CPUStates::kIdle_,
                                            CPUStates::kIOwait_,
                                            CPUStates::kIRQ_,
                                            CPUStates::kSoftIRQ_,
                                            CPUStates::kSteal_,
                                            CPUStates::kGuest_,
                                            CPUStates::kGuestNice_,
                                        },
                                        skip_first_element);

    for (auto& cpu_jiffie : cpu_jiffies) {
      cpu_jiffies_sum += cpu_jiffie;
    }

    return cpu_jiffies_sum;
  }

  return 0;
}

long LinuxParser::ActiveJiffies(int pid) {
  int utime_index = 14, stime_index = 15, cutime_index = 16, cstime_index = 17;

  string line;
  string combined_path = kProcDirectory + to_string(pid) + kStatFilename;
  std::ifstream stream(combined_path);
  if (stream.is_open()) {
    std::getline(stream, line);

    long jiffies_sum = 0;
    vector<long> jiffies = StringHelper::GetElements<long>(
        line,
        vector<int>{utime_index, stime_index, cutime_index, cstime_index});

    for (long jiffie : jiffies) {
      jiffies_sum += jiffie;
    }

    return jiffies_sum;
  }

  return 0;
}

long LinuxParser::ActiveJiffies() {
  string line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);

    int skip_first_element = 1;
    long cpu_jiffies_sum = 0;
    vector<long> cpu_jiffies =
        StringHelper::GetElements<long>(line,
                                        vector<int>{
                                            CPUStates::kUser_,
                                            CPUStates::kNice_,
                                            CPUStates::kSystem_,
                                            CPUStates::kIOwait_,
                                            CPUStates::kIRQ_,
                                            CPUStates::kSoftIRQ_,
                                            CPUStates::kSteal_,
                                            CPUStates::kGuest_,
                                            CPUStates::kGuestNice_,
                                        },
                                        skip_first_element);

    for (auto& cpu_jiffie : cpu_jiffies) {
      cpu_jiffies_sum += cpu_jiffie;
    }

    return cpu_jiffies_sum;
  }

  return 0;
}

long LinuxParser::IdleJiffies() {
  string line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);

    int skip_first_element = 1;
    long cpu_jiffies_sum = 0;
    vector<long> cpu_jiffies =
        StringHelper::GetElements<long>(line,
                                        vector<int>{
                                            CPUStates::kUser_,
                                            CPUStates::kNice_,
                                            CPUStates::kSystem_,
                                            CPUStates::kIOwait_,
                                            CPUStates::kIRQ_,
                                            CPUStates::kSoftIRQ_,
                                            CPUStates::kSteal_,
                                            CPUStates::kGuest_,
                                            CPUStates::kGuestNice_,
                                        },
                                        skip_first_element);

    for (auto& cpu_jiffie : cpu_jiffies) {
      cpu_jiffies_sum += cpu_jiffie;
    }

    return cpu_jiffies_sum;
  }
}

vector<string> LinuxParser::CpuUtilization(long clock_ticks_per_second) {
  vector<string> cpu_utilizations;
  long up_time = UpTime();

  for (int& pid : this->Pids()) {
    string combined_path = kProcDirectory + to_string(pid) + (kStatFilename);
    long active_jiffies = ActiveJiffies(pid);
    long cpu_utilization = CpuHelper::CalculateCpuUsage(
        pid, up_time, active_jiffies, clock_ticks_per_second);

    string cpu_utilization_string =
        "PID " + to_string(pid) + ": " + to_string(cpu_utilization);

    cpu_utilizations.push_back(cpu_utilization_string);
  }

  return cpu_utilizations;
}

int LinuxParser::TotalProcesses() {
  string key, processes_number;
  string line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> processes_number;

      if (key == "processes") {
        return stoi(processes_number);
      }
    }
  }

  return 0;
}
int LinuxParser::RunningProcesses() {
  string key, running_processes_number;
  string line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> running_processes_number;

      if (key == "procs_running") {
        return stoi(running_processes_number);
      }
    }
  }

  return 0;
}

string LinuxParser::Command(int pid) {
  string command;
  string line;
  string combined_path = kProcDirectory + to_string(pid) + kCmdlineFilename;
  std::ifstream stream(combined_path);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> command;

    return command;
  }

  return string();
}

string LinuxParser::Ram(int pid) {
  string key, kilobytes;
  string line;
  string combined_path = kProcDirectory + to_string(pid) + kStatusFilename;
  std::ifstream stream(combined_path);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> kilobytes;

      if (key == "VmSize:") {
        float ram = MemoryHelper::ConvertKBToMB(stof(kilobytes));
        std::stringstream ram_string_stream;

        ram_string_stream << std::fixed << std::setprecision(3) << ram;

        return ram_string_stream.str();
      }
    }
  }

  return string();
}

string LinuxParser::Uid(int pid) {
  string key, uid;
  string line;
  string combined_path = kProcDirectory + to_string(pid) + kStatusFilename;
  std::ifstream stream(combined_path);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> uid;

      if (key == "Uid:") {
        return uid;
      }
    }
  }

  return string();
}

string LinuxParser::User(int pid) {
  string line;
  std::ifstream stream(kPasswordPath);
  if (stream.is_open()) {
    string user_uid = Uid(pid);
    const char split_char = ':';

    while (std::getline(stream, line)) {
      vector<string> splitted_line =
          StringHelper::SplitString(line, split_char);
      bool line_contains_user_id =
          std::find(splitted_line.begin(), splitted_line.end(), user_uid) !=
          splitted_line.end();
      if (line_contains_user_id) {
        return StringHelper::GetStringLeftFromCharacter(line, split_char);
      }
    }
  }

  return string();
}

long LinuxParser::UpTime(int pid) { return UpTime() - StartTime(pid); }

long LinuxParser::StartTime(int pid) {
  string line;
  string combined_path = kProcDirectory + to_string(pid) + kStatFilename;
  std::ifstream stream(combined_path);
  if (stream.is_open()) {
    std::getline(stream, line);
    return StringHelper::GetElement<long>(line, 22);
  }

  return 0;
}