#ifndef GAME_H
#define GAME_H

#include "field.h"
#include "snake.h"
#include "clock.h"

enum GameMode {TIMELIMIT=0, TIMEFREE, KILLSNAKE};
enum PlayMode {Normal=0};
class Game
{
public:
    Game(GameMode game_mode, int height, int width, std::vector<int> info);
    Game(Field* state, GameMode game_mode, std::vector<int> info);

    bool snakeAction(Snake*);    // 核心运行, 包含 [一轮时钟周期里] 对 [一条蛇] 的全部操作
    bool runGame();     // 核心运行, 包含 [对时钟的控制] [接收决策信号] [对每条蛇的 snakeAction() 的调用]

    virtual void initializeGame(int level);  // 用来给图形界面初始化信息

    bool loadMap(std::string map_path);
    int reachTarget();
    Field* getState();
protected:
    int level = 1;

    int target_food = 0;
    int target_time = 0;
    int target_kill = 0;

    Clock clock;
    Field* state;    // state 里面有: [地图] [Item] [snakes]
    GameMode game_mode;
};



class AddWallGame: public Game
{
public:
    AddWallGame(GameMode game_mode, int height, int width, std::vector<int> info);
    AddWallGame(Field* state, GameMode game_mode, std::vector<int> info);
    virtual void initializeGame(int level);
};


#endif // GAME_H
