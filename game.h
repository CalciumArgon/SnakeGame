#ifndef GAME_H
#define GAME_H

#include "field.h"
#include "snake.h"
#include "clock.h"
#include <ctime>

enum GameMode {TIMELIMIT=0, TIMEFREE, KILLSNAKE};
enum PlayMode {Normal=0};
class Game
{
public:
    Game(GameMode game_mode, int height, int width, std::vector<int> info);
    Game(Field* state, GameMode game_mode, std::vector<int> info);

    void setBeginTime(clock_t);

    bool snakeAction(Snake*);    // 核心运行, 包含 [一轮时钟周期里] 对 [一条蛇] 的全部操作
    virtual short runGame();     // 核心运行, 包含 [对时钟的控制] [接收决策信号] [对每条蛇的 snakeAction() 的调用]

    virtual void initializeGame(int level);  // 用来给图形界面初始化信息

    // 全局陨石掉落信息
    void countDown();
    bool isFall();

    bool loadMap(std::string map_path);
    short reachTarget();
    Field* getState();
    int test = 1;
protected:

    int aerolite_counting = 2;

    clock_t begin = 0;
    int level = 1;

    int target_score = 0;
    int target_time = 0;

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

class TestAISnake: public Game
{
public:
    TestAISnake(GameMode game_mode, int height, int width, std::vector<int> info);
    TestAISnake(Field* state, GameMode game_mode, std::vector<int> info);
    virtual void initializeGame(int level);
    virtual short runGame();
};

class Level3: public Game
{
public:
    Level3(GameMode game_mode, int height, int width, std::vector<int> info);
    Level3(Field* state, GameMode game_mode, std::vector<int> info);
    virtual void initializeGame(int level);
};

class Level4: public Game
{
public:
    Level4(GameMode game_mode, int height, int width, std::vector<int> info);
    Level4(Field* state, GameMode game_mode, std::vector<int> info);
    virtual void initializeGame(int level);
};

class Level5: public Game
{
public:
    Level5(GameMode game_mode, int height, int width, std::vector<int> info);
    Level5(Field* state, GameMode game_mode, std::vector<int> info);
    virtual void initializeGame(int level);
};

class Level6: public Game
{
public:
    Level6(GameMode game_mode, int height, int width, std::vector<int> info);
    Level6(Field* state, GameMode game_mode, std::vector<int> info);
    virtual void initializeGame(int level);
};

class Level7: public Game
{
public:
    Level7(GameMode game_mode, int height, int width, std::vector<int> info);
    Level7(Field* state, GameMode game_mode, std::vector<int> info);
    virtual void initializeGame(int level);
};

class Level8: public Game
{
public:
    Level8(GameMode game_mode, int height, int width, std::vector<int> info);
    Level8(Field* state, GameMode game_mode, std::vector<int> info);
    virtual void initializeGame(int level);
};

class Level9: public Game
{
public:
    Level9(GameMode game_mode, int height, int width, std::vector<int> info);
    Level9(Field* state, GameMode game_mode, std::vector<int> info);
    virtual void initializeGame(int level);
};

class Level10: public Game
{
public:
    Level10(GameMode game_mode, int height, int width, std::vector<int> info);
    Level10(Field* state, GameMode game_mode, std::vector<int> info);
    virtual void initializeGame(int level);
};
#endif // GAME_H
