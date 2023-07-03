

#include "clock.h"

Clock::Clock(int start, string name, int speed):
    time(start),
    name(name),
    speed(speed) {}

Clock::Clock(string name, int speed):
    time(0),
    name(name),
    speed(speed) {}

Clock::Clock(string name):  // 默认 10ms 运行一次
    time(0),
    name(name),
    speed(10) {}

Clock::~Clock() {}  // 好像没有指针 没什么要释放的空间

void Clock::run() {
    time += step;
}

int Clock::getTime() {
    return this->time;
}

void Clock::show() {
    cout << "Time: " << time << endl;
}

