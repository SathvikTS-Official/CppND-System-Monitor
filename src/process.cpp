#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::stoi;

Process::Process(int pid, string username, string ram, 
                string command, long int uptime, long int total_jiffies) : 
                pid_(pid),
                user_(username),
                ram_(ram), 
                command_(command), 
                uptime_(uptime), 
                total_jiffies_(total_jiffies) {};

int Process::Pid() { return pid_; }

float Process::CpuUtilization() {
    long int elapsed_time = LinuxParser::UpTime() - this->UpTime();
    float cpu_usage = float((total_jiffies_ / sysconf(_SC_CLK_TCK))) / float(elapsed_time);
    return cpu_usage; 
}

string Process::Command() { 
    return command_; 
}

string Process::Ram() { 
    return to_string(stoi(ram_)/1000); 
}

string Process::User() { 
    return user_; 
}

long int Process::UpTime() { 
    int converted_time = uptime_/sysconf(_SC_CLK_TCK);
    return converted_time; 
}

bool Process::operator<(Process const& a) const {
    return stoi(ram_) < stoi(a.ram_); 
}