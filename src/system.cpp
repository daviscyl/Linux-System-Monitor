#include "system.h"

#include <unistd.h>

#include <cstddef>
#include <set>
#include <string>

#include "linux_parser.h"
#include "process.h"
#include "processor.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

// Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// Return a container composed of the system's processes
vector<Process>& System::Processes() {
  processes_.clear();
  for (int pid : LinuxParser::Pids()) {
    if (process_uptimes_.find(pid) == process_uptimes_.end()) {
      process_uptimes_[pid] = vector<long>{0, 0};
    }
    Process p(pid, process_uptimes_[pid]);
    processes_.emplace_back(p);
  }
  std::sort(processes_.begin(), processes_.end(), std::greater<Process>());
  return processes_;
}

// Return the system's kernel identifier (string)
std::string System::Kernel() {
  if (kernel_.empty()) {
    kernel_ = LinuxParser::Kernel();
  }
  return kernel_;
}

// Return the system's memory utilization
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

// Return the operating system name
std::string System::OperatingSystem() {
  if (operating_system_.empty()) {
    operating_system_ = LinuxParser::OperatingSystem();
  }
  return operating_system_;
}

// Return the number of processes actively running on the system
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

// Return the total number of processes on the system
int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

// Return the number of seconds since the system started running
long int System::UpTime() { return LinuxParser::UpTime(); }