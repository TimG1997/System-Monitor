//
// Created by User on 21.05.2021.
//

#ifndef MONITOR_LINUX_INFO_FILE_LOCATIONS_H
#define MONITOR_LINUX_INFO_FILE_LOCATIONS_H

namespace LinuxInfoFileLocations {
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
}  // namespace LinuxInfoFileLocations

#endif  // MONITOR_LINUX_INFO_FILE_LOCATIONS_H
