#include "linux_parser.h"

#include <dirent.h>
#include <unistd.h>

#include <string>
#include <vector>

using std::stof;
using std::stol;
using std::string;
using std::to_string;
using std::vector;

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
  string os, vrs, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> vrs >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR *directory = opendir(kProcDirectory.c_str());
  struct dirent *file;
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

// Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  float memtotal, memfree;
  string line, n;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestreamtotal(line);
    linestreamtotal >> n >> n;
    memtotal = std::stof(n);

    std::getline(stream, line);
    std::istringstream linestreamfree(line);
    linestreamfree >> n >> n;
    memfree = std::stof(n);
  }
  return (memtotal - memfree) / memtotal;
}

// Read and return the system uptime
long LinuxParser::UpTime() {
  string uptime;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> uptime;
    return stol(uptime);
  }
  return 0;
}

// Read and return the number of jiffies for the system
long LinuxParser::Jiffies(vector<string> &u) {
  return ActiveJiffies(u) + IdleJiffies(u);
}

// Read and return the number of active jiffies for a PID
long LinuxParser::ActiveJiffies(int pid) {
  string n, utime, stime, cutime, cstime;
  string line;

  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    for (int i = 0; i < 13; i++) linestream >> n;
    linestream >> utime >> stime >> cutime >> cstime;
    return (stol(utime) + stol(stime));
  }
  return 0;
}

// Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies(vector<string> &u) {
  // user + nice + system + irq + softirq + steal
  return stol(u[1]) + stol(u[2]) + stol(u[3]) + stol(u[6]) + stol(u[7]) +
         stol(u[8]);
}

// Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies(vector<string> &u) {
  // idle + iowait
  return stol(u[4]) + stol(u[5]);
}

// Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  string cpu, user, nice, system, idle, iowait, irq, softirq, steal, guest,
      guest_nice;
  string line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> cpu >> user >> nice >> system >> idle >> iowait >> irq >>
        softirq >> steal >> guest >> guest_nice;
  }
  return vector<string>{cpu, user,    nice,  system, idle,      iowait,
                        irq, softirq, steal, guest,  guest_nice};
}

// Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  string n;
  string line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> n;
      if (n == "processes") {
        linestream >> n;
        return std::stoi(n);
      }
    }
  }
  return 0;
}

// Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  string n;
  string line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> n;
      if (n == "procs_running") {
        linestream >> n;
        return std::stoi(n);
      }
    }
  }
  return 0;
}

// Read and return the command associated with a process
string LinuxParser::Command(int pid) {
  string cmd;
  std::ifstream stream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if (stream.is_open()) std::getline(stream, cmd);
  return cmd;
}

// Read and return the memory used by a process
string LinuxParser::Ram(int pid) {
  string name, ram = "0";
  string line;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> name;
      if (name == "VmSize:") {
        linestream >> ram;
        int digits = ram.length();
        ram = digits > 3 ? ram.substr(0, digits - 3) : "0";
        break;
      }
    }
  }
  return ram;
}

// Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) {
  string uid;
  string line;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> uid;
      if (uid == "Uid:") {
        linestream >> uid;
        break;
      }
    }
  }
  return uid;
}

// Read and return the user associated with a process
string LinuxParser::User(int pid) {
  string uid = Uid(pid);
  string user, x, id;
  string line;
  std::ifstream stream(kPasswordPath);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      std::getline(linestream, user, ':');
      std::getline(linestream, x, ':');
      std::getline(linestream, id, ':');
      if (id == uid) break;
    }
  }
  return user;
}

// Read and return the uptime of a process
long LinuxParser::UpTime(int pid) {
  return ActiveJiffies(pid) / sysconf(_SC_CLK_TCK);
}