#ifndef CLOCK_H
#define CLOCK_H

#include <string>
using namespace std;

class Clock
{
public:
    Clock(int start, string name, int speed);
    Clock(string name, int speed);
    Clock(string name);
    ~Clock();

    void run();
    void show();
    int time();

    int time;
    int step = 1;   // 每次运行在 time 上叠加的数值
    int speed;  // 多少毫秒运行一次
    string name;
};

#endif