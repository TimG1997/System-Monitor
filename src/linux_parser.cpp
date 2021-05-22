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

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem(std::string osPath) {
  string line;
  string key;
  string value;
  std::ifstream filestream(osPath);
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
string LinuxParser::Kernel(std::string kernelPath) {
  string os, kernel, version;
  string line;
  std::ifstream stream(kernelPath);
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

float LinuxParser::MemoryUtilization(std::string mem_info_path) {
  long total_memory, free_memory;

  string key, memory_number, unit;
  string line;
  std::ifstream stream(mem_info_path);
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

long LinuxParser::UpTime(std::string uptime_path) {
  string uptime;
  string line;
  std::ifstream stream(uptime_path);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> uptime;
  }

  return stol(uptime);
}

long LinuxParser::Jiffies(std::string stat_path) {
  string cpu, user, nice, system, idle, iowait, irq, softirq, steal, guest,
      guest_nice;
  string line;
  std::ifstream stream(stat_path);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> cpu >> user >> nice >> system >> idle >> iowait >> irq >>
        softirq >> steal >> guest >> guest_nice;

    return stol(user) + stol(nice) + stol(system) + stol(idle) + stol(iowait) +
           stol(irq) + stol(softirq) + stol(steal) + stol(guest) +
           stol(guest_nice);
  }

  return 0;
}

long LinuxParser::ActiveJiffies(int pid, std::string proc_dir,
                                std::string stat_filename) {
  int utime_index = 14, stime_index = 15, cutime_index = 16, cstime_index = 17;

  string line;
  string combined_path = proc_dir + to_string(pid) + stat_filename;
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

long LinuxParser::ActiveJiffies(std::string stat_path) {
  string cpu, user, nice, system, idle, iowait, irq, softirq, steal, guest,
      guest_nice;
  string line;
  std::ifstream stream(stat_path);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> cpu >> user >> nice >> system >> idle >> iowait >> irq >>
        softirq >> steal >> guest >> guest_nice;

    return stol(user) + stol(nice) + stol(system) + stol(iowait) + stol(irq) +
           stol(softirq) + stol(steal) + stol(guest) + stol(guest_nice);
  }

  return 0;
}

long LinuxParser::IdleJiffies(std::string stat_path) {
  string cpu, user, nice, system, idle, iowait, irq, softirq, steal, guest,
      guest_nice;
  string line;
  std::ifstream stream(stat_path);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> cpu >> user >> nice >> system >> idle >> iowait >> irq >>
        softirq >> steal >> guest >> guest_nice;

    return stol(idle);
  }

  return 0;
}

vector<string> LinuxParser::CpuUtilization(std::vector<int> pids,
                                           string proc_dir, string stat_path,
                                           string uptime_path,
                                           long clock_ticks_per_second) {
  vector<string> cpu_utilizations;
  long up_time = UpTime(uptime_path);

  for (int& pid : pids) {
    string combined_path = proc_dir + to_string(pid) + (stat_path);
    long active_jiffies = ActiveJiffies(pid, proc_dir);
    long cpu_utilization =
        CpuHelper::CalculateCpuUsage(pid, up_time, active_jiffies, clock_ticks_per_second);

    string cpu_utilization_string =
        "PID " + to_string(pid) + ": " + to_string(cpu_utilization);

    cpu_utilizations.push_back(cpu_utilization_string);
  }

  return cpu_utilizations;
}

int LinuxParser::TotalProcesses(std::string stat_path) {
  string key, processes_number;
  string line;
  std::ifstream stream(stat_path);
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
int LinuxParser::RunningProcesses(std::string stat_path) {
  string key, running_processes_number;
  string line;
  std::ifstream stream(stat_path);
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

string LinuxParser::Command(int pid, std::string proc_dir,
                            std::string command_filename) {
  string command;
  string line;
  string combined_path = proc_dir + to_string(pid) + command_filename;
  std::ifstream stream(combined_path);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> command;

    return command;
  }

  return string();
}

string LinuxParser::Ram(int pid, std::string proc_dir,
                        std::string status_filename) {
  string key, kilobytes;
  string line;
  string combined_path = proc_dir + to_string(pid) + status_filename;
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

string LinuxParser::Uid(int pid, std::string proc_dir,
                        std::string status_filename) {
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

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid [[maybe_unused]]) { return string(); }

long LinuxParser::UpTime(int pid, std::string proc_dir, std::string uptime_path,
                         std::string stat_filename) {
  return UpTime(uptime_path) - StartTime(pid, proc_dir, stat_filename);
}

long LinuxParser::StartTime(int pid, std::string proc_dir,
                            std::string stat_filename) {
  string line;
  string combined_path = proc_dir + to_string(pid) + stat_filename;
  std::ifstream stream(combined_path);
  if (stream.is_open()) {
    std::getline(stream, line);
    return StringHelper::GetElement<long>(line, 22);
  }

  return 0;
}
