#include "helper/format_helper.h"

#include <string>

using namespace std;

string Format::ElapsedTime(long total_in_seconds) {
  long hours, minutes, seconds;

  minutes = total_in_seconds / 60;
  seconds = total_in_seconds % 60;

  hours = minutes / 60;
  minutes = minutes % 60;

  return Format::AddLeadingZero(hours) + ":" +
         Format::AddLeadingZero(minutes) + ":" +
         Format::AddLeadingZero(seconds);
}

string Format::AddLeadingZero(long time){
  return time < 10 ? "0" + to_string(time) : to_string(time);
}