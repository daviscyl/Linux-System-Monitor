#include "format.h"

#include <string>

using std::string;
using std::to_string;

// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long seconds) {
  string HH = to_string(seconds / 3600);
  if (HH.length() < 2) HH = "0" + HH;
  string MM = to_string((seconds % 3600) / 60);
  if (MM.length() < 2) MM = "0" + MM;
  string SS = to_string(seconds % 60);
  if (SS.length() < 2) SS = "0" + SS;
  return HH + ":" + MM + ":" + SS;
}