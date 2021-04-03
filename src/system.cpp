#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

System::System() : cpu_() {
    vector<int> pids = LinuxParser::Pids();
    string username;
    string ram;
    string command;
    long int uptime;
    long int total_jiffies;
    for(auto i : pids)
    {
        username = LinuxParser::User(i);
        ram = LinuxParser::Ram(i);
        command = LinuxParser::Command(i);
        uptime = LinuxParser::UpTime(i);
        total_jiffies = LinuxParser::ActiveJiffies(i);
        processes_.push_back(Process(i,username,ram,command,uptime,total_jiffies));
    }
};

Processor& System::Cpu() { return cpu_; }

vector<Process>& System::Processes() { return processes_; }

std::string System::Kernel() { 
    return LinuxParser::Kernel();
}

float System::MemoryUtilization() { 
    return LinuxParser::MemoryUtilization() ; 
}

std::string System::OperatingSystem() { 
    return LinuxParser::OperatingSystem();
}

int System::RunningProcesses() { 
    return LinuxParser::RunningProcesses(); 
}

int System::TotalProcesses() { 
    return LinuxParser::TotalProcesses(); 
}

long int System::UpTime() { 
    return LinuxParser::UpTime(); 
}