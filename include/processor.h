#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  Processor();
  float Utilization(); 

 private:
  int user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice;
};

#endif