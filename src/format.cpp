#include <string>

#include "format.h"

using std::string;

string Format::ElapsedTime(long seconds) { 
    int hrs = 0;
    int mins = 0;
    int secs = 0;
    int time = seconds;
    hrs = time/3600;
    time = time%3600;
    mins = time/60;
    time = time%60;
    secs = time;
    string format_time = std::to_string(hrs) + ":" + std::to_string(mins) + ":" + std::to_string(secs);
    return format_time;
}