#include "processor.h"

#include <string>
#include <vector>

#include "linux_parser.h"

using std::string;
using std::vector;

// Return the aggregate CPU utilization
float Processor::Utilization() {
  vector<string> u = LinuxParser::CpuUtilization();
  long active = LinuxParser::ActiveJiffies(u);
  long idle = LinuxParser::IdleJiffies(u);
  long total = active + idle;
  float utilization =
      (float)(active - prevactive_) / (float)(total - prevtotal_);
  prevactive_ = active;
  prevtotal_ = total;
  return utilization;
}