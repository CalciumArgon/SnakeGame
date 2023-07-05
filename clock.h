#ifndef CLOCK_H
#define CLOCK_H

#include <string>

class Clock
{
public:
    Clock(int start, std::string name, int speed);
    Clock(std::string name, int speed);
    Clock(std::string name);

    void run();
    int getTime();
private:
    int time;
    int step = 1;   // 每次运行在 time 上叠加的数值
    int speed;  // 多少毫秒运行一次
    std::string name;
};

#endif
