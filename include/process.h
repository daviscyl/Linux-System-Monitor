#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include <vector>

#include "linux_parser.h"

using std::string;

/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  int Pid();
  string User();
  string Command();
  float CpuUtilization();
  string Ram();
  long int UpTime();
  bool operator>(Process const& a) const;
  Process(int pid, std::vector<long>& uptimes);

  // Declare any necessary private members
 private:
  int pid_;
  float process_cpu_;
};

#endif