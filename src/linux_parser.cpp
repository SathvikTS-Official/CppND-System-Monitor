#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

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

string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
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

float LinuxParser::MemoryUtilization() { 
  float memory_usage = 0.0;
  float memory_total = 1.0;
  float memory_free = 0.0;
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "MemTotal:") {
          memory_total = std::stof(value);
        }
        else if (key == "MemFree:") {
          memory_free = std::stof(value);
        }
      }
    }
  }
  memory_usage = (memory_total - memory_free)/memory_total;
  return memory_usage;
}

long LinuxParser::UpTime() { 
  long int up_time = 0;
  string line;
  string value;
  std::ifstream filestream(kProcDirectory + kUptimeFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> value;
  }
  up_time = std::stoi(value);
  return up_time;
  return 0;
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

long LinuxParser::ActiveJiffies(int pid) { 
  string line;
  string value;
  long int total_jiffies;
  int i = 1;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    while(linestream >> value)
    {
      if(i>=14 && i<=17)
      {
        
        total_jiffies += std::stol(value);
      }
      i++;
    }
  }
  return total_jiffies;
 }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

vector<string> LinuxParser::CpuUtilization() { 
  string cpu_, user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice;
  string line;
  vector<string> cpu_data = {};
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> cpu_ >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal >> guest >> guest_nice;
  }
  cpu_data.push_back(user);
  cpu_data.push_back(nice);
  cpu_data.push_back(system);
  cpu_data.push_back(idle);
  cpu_data.push_back(iowait);
  cpu_data.push_back(irq);
  cpu_data.push_back(softirq);
  cpu_data.push_back(steal);
  cpu_data.push_back(guest);
  cpu_data.push_back(guest_nice);
  return cpu_data; 
}

int LinuxParser::TotalProcesses() { 
  int total_processes = 0;
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "processes") {
          total_processes = std::stoi(value);
        }
      }
    }
  }
  return total_processes;
}

int LinuxParser::RunningProcesses() { 
  int running_processes = 0;
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "procs_running") {
          running_processes = std::stoi(value);
        }
      }
    }
  }
  return running_processes;
}

string LinuxParser::Command(int pid) { 
  string command_;
  string line;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    command_ = line;
  }
  return command_;
}

string LinuxParser::Ram(int pid) { 
  string ram_ ;
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "VmSize:") {
          ram_ = value;
          break;
        }
      }
    }
  }
  return ram_;
}

string LinuxParser::Uid(int pid) { 
  string uid ;
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "Uid:") {
          uid = value;
          break;
        }
      }
    }
  }
  return uid;
}

string LinuxParser::User(int pid) { 
  string uid = Uid(pid);
  string username;
  string line;
  std::size_t found;
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      found = line.find(":"+uid+":");
      if (found!=std::string::npos)
      {
        username = line.substr(0, line.find(":"));
        break;
      }
    }
  }
  return username; 
}

long LinuxParser::UpTime(int pid) { 
  string line;
  string value;
  long int uptime;
  int i = 1;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    while(linestream >> value)
    {
      if(i==22)
      {
        
        uptime = std::stol(value);
        break;
      }
      i++;
    }
  }
  return uptime;
}
