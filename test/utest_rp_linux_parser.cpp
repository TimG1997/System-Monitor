#include <experimental/filesystem>
#include <string>

#include "gtest/gtest.h"
#include "helper/test_helpers.h"
#include "parsing/linux_parser.h"

#define EXPECT_FLOAT_EQ_WITH_DELTA(val1, val2, delta) \
  EXPECT_TRUE((abs(val1 - val2)) < delta)

using namespace std;

class LinuxParserTest : public ::testing::Test {
 public:
  const int kPidOne{1015};
  const int kPidTwo{1016};
  const int kPidThree{1017};

  const string kTestResourcesDirectory{resolvePath("Testing/resources")};

  const string kProcDirectory{kTestResourcesDirectory + "/proc/"};
  const string kCmdlineFilename{"/cmdline"};
  const string kCpuinfoFilename{"/cpuinfo"};
  const string kStatusFilename{"/status"};
  const string kStatPath{kTestResourcesDirectory + "stat"};
  const string kStatFilename{"/stat"};
  const string kUptimePath{"/uptime"};
  const string kMeminfoFilename{"/meminfo"};
  const string kVersionFilename{"/version"};
  const string kOSPath{kTestResourcesDirectory + "/os-release"};
  const string kPasswordPath{"/etc/passwd"};
};

TEST_F(LinuxParserTest, TestOperatingSystem) {
  string expected_os = "Pretty Name of a OS";
  string actual_os = LinuxParser::OperatingSystem(kOSPath);

  EXPECT_EQ(expected_os, actual_os);
}

TEST_F(LinuxParserTest, TestKernel) {
  string expected_kernel = "5.4.72-microsoft-standard-WSL2";
  string actual_kernel = LinuxParser::Kernel(kProcDirectory, kVersionFilename);

  EXPECT_EQ(expected_kernel, actual_kernel);
}

TEST_F(LinuxParserTest, TestMemoryUtilization) {
  float expected_memory_usage = 0.2214;
  float actual_memory_usage = LinuxParser::MemoryUtilization(kProcDirectory, kMeminfoFilename);

  EXPECT_FLOAT_EQ_WITH_DELTA(expected_memory_usage, actual_memory_usage, 0.001);
}

TEST_F(LinuxParserTest, TestUpTime) {
  long expected_uptime = 1200;
  long actual_uptime = LinuxParser::UpTime(kProcDirectory, kUptimeFilename);

  EXPECT_LE(expected_uptime, actual_uptime);
}

TEST_F(LinuxParserTest, TestTotalProcesses) {
  int expected_total_processes = 5000;
  int actutal_total_processes = LinuxParser::TotalProcesses(kProcDirectory, kStatFilename);

  EXPECT_LE(expected_total_processes, actutal_total_processes);
}

TEST_F(LinuxParserTest, TestRunningProcesses) {
  int expected_running_processes = 300;
  int actual_running_processes = LinuxParser::RunningProcesses(kProcDirectory, kStatFilename);

  EXPECT_EQ(expected_running_processes, actual_running_processes);
}

TEST_F(LinuxParserTest, TestJiffies) {
  long expected_jiffies = 3370364;
  long actual_jiffies = LinuxParser::Jiffies(kProcDirectory, kStatFilename);

  EXPECT_EQ(expected_jiffies, actual_jiffies);
}

TEST_F(LinuxParserTest, TestActiveJiffies) {
  long expected_active_jiffies = 46089;
  long actual_active_jiffies = LinuxParser::ActiveJiffies(kProcDirectory, kStatFilename);

  EXPECT_EQ(expected_active_jiffies, actual_active_jiffies);
}

TEST_F(LinuxParserTest, TestIdleJiffies) {
  long expected_idle_jiffies = 3324275;
  long actual_idle_jiffies = LinuxParser::IdleJiffies(kProcDirectory, kStatFilename);

  EXPECT_EQ(expected_idle_jiffies, actual_idle_jiffies);
}

TEST_F(LinuxParserTest, TestCommand) {
  string expected_command = "/command";
  string actual_command = LinuxParser::Command(kPidOne, kProcDirectory, kCmdlineFilename);

  EXPECT_EQ(expected_command, actual_command);
}

TEST_F(LinuxParserTest, TestUid) {
  string expected_uid = "0";
  string actual_uid = LinuxParser::Uid(kPidOne, kProcDirectory, kStatusFilename);

  EXPECT_EQ(expected_uid, actual_uid);
}

TEST_F(LinuxParserTest, TestStarttimeForPid) {
  long expected_start_time = 1000;
  long actual_start_time = LinuxParser::StartTime(kPidOne, kProcDirectory, kStatFilename);

  EXPECT_EQ(expected_start_time, actual_start_time);
}

TEST_F(LinuxParserTest, TestRam) {
  string expected_ram = "1632";
  string actual_ram = LinuxParser::Ram(kPidOne, kProcDirectory, kStatusFilename);

  EXPECT_EQ(expected_ram, actual_ram);
}

TEST_F(LinuxParserTest, TestActiveJiffiesForPid) {
  long expected_active_jiffies = 400;
  long actual_active_jiffes = LinuxParser::ActiveJiffies(kPidOne, kProcDirectory, kStatFilename);

  EXPECT_EQ(expected_active_jiffies, actual_active_jiffes);
}

TEST_F(LinuxParserTest, TestCpuUtilization) {
  vector<long> cpu_utilizations = LinuxParser::CpuUtilization(kPidOne, kProcDirectory, kStatFilename);

  long expected_cpu_up_time = 1200;
  long expected_cpu_active_jiffies_for_pid = 400;
  long expected_cpu_start_time_for_pid = 1000;

  long actual_cpu_up_time = cpu_utilizations[0];
  long actual_cpu_active_jiffies_for_pid = cpu_utilizations[1];
  long actual_cpu_start_time_for_pid = cpu_utilizations[2];

  EXPECT_EQ(expected_cpu_up_time, actual_cpu_up_time);
  EXPECT_EQ(expected_cpu_active_jiffies_for_pid,
            actual_cpu_active_jiffies_for_pid);
  EXPECT_EQ(expected_cpu_start_time_for_pid, actual_cpu_start_time_for_pid);
}

TEST_F(LinuxParserTest, TestUser){
  string expected_user = "root";
  string actual_user = LinuxParser::User(kPidOne, kProcDirectory, kStatusFilename, kPasswordPath);

  EXPECT_EQ(expected_user, actual_user);
}