#include "parsing/linux_parser.h"

#include <dirent.h>

#include <iostream>

#include "helper/memory_helper.h"
#include "helper/string_helper.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem(const std::string& os_path) {
  string line;
  string key;
  string value;
  std::ifstream filestream(os_path);
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
string LinuxParser::Kernel(const std::string& proc_dir, const std::string& version_filename) {
  string os, kernel, version;
  string line;
  std::string combined_path = proc_dir + version_filename;
  std::ifstream stream(combined_path);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids(const std::string& proc_dir) {
  vector<int> pids;
  DIR* directory = opendir(proc_dir.c_str());
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

float LinuxParser::MemoryUtilization(const std::string& proc_dir, const std::string& meminfo_filename) {
  long total_memory, free_memory;

  string key, memory_number, unit;
  string line;
  std::ifstream stream(proc_dir + meminfo_filename);
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

long LinuxParser::UpTime(const std::string& proc_dir, const std::string& uptime_filename) {
  string uptime;
  string line;
  std::ifstream stream(proc_dir + uptime_filename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> uptime;
  }

  return stol(uptime);
}

long LinuxParser::Jiffies(const std::string& proc_dir, const std::string& stat_filename) {
  string line;
  std::ifstream stream(proc_dir + stat_filename);
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

long LinuxParser::ActiveJiffies(int pid, const std::string& proc_dir, const std::string& stat_filename) {
  string line;
  string combined_path = proc_dir + to_string(pid) + stat_filename;
  std::ifstream stream(combined_path);
  if (stream.is_open()) {
    std::getline(stream, line);

    long jiffies_sum = 0;
    vector<long> jiffies = StringHelper::GetElements<long>(
        line, vector<int>{ProcCPUStates::kUtime_, ProcCPUStates::kStime_,
                          ProcCPUStates::kCutime_, ProcCPUStates::kCstime_});

    for (long jiffie : jiffies) {
      jiffies_sum += jiffie;
    }

    return jiffies_sum;
  }

  return 0;
}

long LinuxParser::ActiveJiffies(const std::string& proc_dir, const std::string& stat_filename) {
  string line;
  std::ifstream stream(proc_dir + stat_filename);
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

long LinuxParser::IdleJiffies(const std::string& proc_dir, const std::string& stat_filename) {
  string line;
  std::ifstream stream(proc_dir + stat_filename);
  if (stream.is_open()) {
    std::getline(stream, line);

    int skip_first_element = 1;
    long cpu_jiffies_sum = 0;
    vector<long> cpu_jiffies =
        StringHelper::GetElements<long>(line,
                                        vector<int>{
                                            CPUStates::kIOwait_,
                                            CPUStates::kIdle_,
                                        },
                                        skip_first_element);

    for (auto& cpu_jiffie : cpu_jiffies) {
      cpu_jiffies_sum += cpu_jiffie;
    }

    return cpu_jiffies_sum;
  }

  return 0;
}

vector<long> LinuxParser::CpuUtilization(int pid, const std::string& proc_dir, const std::string& stat_filename, const std::string& uptime_filename) {
  long up_time = UpTime(proc_dir, uptime_filename);
  long active_jiffies = ActiveJiffies(pid, proc_dir, stat_filename);
  long start_time = StartTime(pid, proc_dir, stat_filename);

  vector<long> cpu_utilizations{up_time, active_jiffies, start_time};

  return cpu_utilizations;
}

int LinuxParser::TotalProcesses(const std::string& proc_dir, const std::string& stat_filename) {
  string key, processes_number;
  string line;
  std::ifstream stream(proc_dir + stat_filename);
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
int LinuxParser::RunningProcesses(const std::string& proc_dir, const std::string& stat_filename) {
  string key, running_processes_number;
  string line;
  std::ifstream stream(proc_dir + stat_filename);
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

string LinuxParser::Command(int pid, const std::string& proc_dir, const std::string& cmdline_filename) {
  string command, line;
  std::ifstream filestream(proc_dir + to_string(pid) + cmdline_filename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> command;
  }
  return command;
}

string LinuxParser::Ram(int pid, const std::string& proc_dir, const std::string& status_filename) {
  string key, kilobytes;
  string line;
  string combined_path = proc_dir + to_string(pid) + status_filename;
  std::ifstream stream(combined_path);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> kilobytes;

      if (key == "VmSize:") {
        return std::to_string(MemoryHelper::ConvertKBToMB(stol(kilobytes)));
      }
    }
  }

  return string();
}

string LinuxParser::Uid(int pid, const std::string& proc_dir, const std::string& status_filename) {
  string key, uid;
  string line;
  string combined_path = proc_dir + to_string(pid) + status_filename;
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

string LinuxParser::User(int pid, const std::string& proc_dir, const std::string& status_filename, const std::string& password_path) {
  string line;
  std::ifstream stream(password_path);
  if (stream.is_open()) {
    string user_uid = Uid(pid, proc_dir, status_filename);
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

long LinuxParser::UpTime(int pid, const std::string& proc_dir, const std::string& uptime_filename, const std::string& stat_filename) {
  return UpTime(proc_dir, uptime_filename) - CpuHelper::ConvertHertzToSeconds(StartTime(pid,proc_dir, stat_filename));
}

long LinuxParser::StartTime(int pid, const std::string& proc_dir, const std::string& stat_filename) {
  string line;
  string combined_path = proc_dir + to_string(pid) + stat_filename;
  std::ifstream stream(combined_path);
  if (stream.is_open()) {
    std::getline(stream, line);
    return StringHelper::GetElement<long>(line, ProcCPUStates::kStarttime_);
  }

  return 0;
}