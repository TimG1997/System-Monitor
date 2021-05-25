#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <parsing/linux_parser.h>
#include <memory>

class Processor {
 public:
  Processor();

  float Utilization();

 private:
  long last_idle;
  long last_total;
};

#endif