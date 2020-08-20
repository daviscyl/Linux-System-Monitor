#include "process.h"

#include <unistd.h>

#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

// Return this process's ID
int Process::Pid() { return pid_; }

// Return this process's CPU utilization
float Process::CpuUtilization() { return process_cpu_; }

// Return the command that generated this process
string Process::Command() { return LinuxParser::Command(pid_); }

// Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(pid_); }

// Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(pid_); }

// Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime(pid_); }

// Overload the "greater than" comparison operator for Process objects
bool Process::operator>(Process const& that) const {
  return this->process_cpu_ > that.process_cpu_;
}

// jiffies = {prev_process_jiffies, prev_system_jiffies}
Process::Process(int pid, std::vector<long>& jiffies) : pid_(pid) {
  vector<string> u = LinuxParser::CpuUtilization();
  long sys_jiffies = LinuxParser::Jiffies(u);
  long proc_jiffies = LinuxParser::ActiveJiffies(pid_);
  process_cpu_ = (float)(proc_jiffies - jiffies[0]) / (float)(sys_jiffies - jiffies[1]);
  jiffies[0] = proc_jiffies;
  jiffies[1] = sys_jiffies;
}
