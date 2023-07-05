#include "clock.h"

using namespace std;

Clock::Clock(int start, string name, int speed):
    time(start),
    speed(speed),
    name(name)
    {}

Clock::Clock(string name, int speed):
    time(0),
    speed(speed),
    name(name)
     {}

Clock::Clock(string name):  // 默认 10ms 运行一次
    time(0),
    speed(10),
    name(name)
     {}

void Clock::run() {
    time += step;
}

int Clock::getTime() {
    return time;
}

