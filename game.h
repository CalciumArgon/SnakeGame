#ifndef GAME_H
#define GAME_H

#include "snake.h"
#include "clock.h"
#include <string>
using namespace std;

class Field {};

class Game
{
public:
    Game(string);
    ~Game();

    /*****************************/
    void snakeAction(Snake*);    // 核心运行, 要包含 [一轮时钟周期里] 对 [一条蛇] 的全部操作
    /*****************************/
    void runGame();
    void initializeGame();  // 用来给图形界面初始化信息

    bool reachTarget();

    Clock clock;
    Field* state;    // state 里面有: [地图] [Item] [snakes]
    string game_mode;
};


#endif