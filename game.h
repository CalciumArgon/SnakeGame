#ifndef GAME_H
#define GAME_H

#include "snake.h"
#include "clock.h"
#include <string>
#include <thread>
using namespace std;

enum GameMode {TIMELIMIT=0, TIMEFREE, KILLSNAKE};

class Game
{
public:
    Game(GameMode, int height, int width, vector<int>);
    ~Game();

    void snakeAction(Snake*);    // 核心运行, 包含 [一轮时钟周期里] 对 [一条蛇] 的全部操作
    void runGame();     // 核心运行, 包含 [对时钟的控制] [接收决策信号] [对每条蛇的 snakeAction() 的调用]

    void initializeGame(int level);  // 用来给图形界面初始化信息

    int reachTarget();

    int level = 1;

    int target_food = 0;
    int target_time = 0;
    int target_kill = 0;

    Clock clock;
    Field* state;    // state 里面有: [地图] [Item] [snakes]
    GameMode game_mode;
};


#endif
