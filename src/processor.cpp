#include "processor.h"
#include "linux_parser.h"
#include <vector>
#include <string>
#include <iostream>

using std::stoi;

template<typename T>
T pop_front(std::vector<T> &v)
{
    T element = v.front();
    if (v.size() > 0) {
        v.erase(v.begin());
    }
    return element;
}

Processor::Processor() {
    std::vector<std::string> cpu_data = LinuxParser::CpuUtilization();
    user = stoi(pop_front(cpu_data));
    nice = stoi(pop_front(cpu_data));
    system = stoi(pop_front(cpu_data));
    idle = stoi(pop_front(cpu_data));
    iowait = stoi(pop_front(cpu_data));
    irq = stoi(pop_front(cpu_data));
    softirq = stoi(pop_front(cpu_data));
    steal = stoi(pop_front(cpu_data));
    guest = stoi(pop_front(cpu_data));
    guest_nice = stoi(pop_front(cpu_data));
}

float Processor::Utilization() { 
    int idle_time = idle + iowait;
    int non_idle_time = user + nice + system + irq + softirq + steal;
    int total_time = idle_time + non_idle_time;
    return float(total_time - idle_time)/float(total_time);
}