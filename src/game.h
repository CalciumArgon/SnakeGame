#ifndef GAME_H
#define GAME_H

#include "field.h"
#include "snake.h"
#include <ctime>
#include <string>

enum GameMode {TIMELIMIT=0, TIMEFREE, KILLSNAKE};
enum PlayMode {Normal=0};
class Game
{
public:
    Game(GameMode game_mode, size_t height, size_t width, std::vector<int> info);
    Game(Field* state, GameMode game_mode, std::vector<int> info);
    virtual ~Game();

    void setBeginTime(clock_t);

    // 核心运行, 包含 [一轮时钟周期里] 对 [一条蛇] 的全部操作
    bool snakeAction(Snake*);
    // 核心运行, 包含 [对时钟的控制] [接收决策信号] [对每条蛇的 snakeAction() 的调用]
    short runGame();
    virtual bool createFood() { return false; }
    virtual bool createSnake() { return false; }

    // 用来给图形界面初始化信息
    virtual void initializeGame(int level);

    // 全局陨石掉落信息
    void countDown();
    bool isFall();
    bool isWarning();

    //地图载入
    bool loadMap(std::string map_path);
    //判断游戏是否结束
    short reachTarget();

    Field* getState();
    int getTargetTime();
    int getTargetScore();
    GameMode getMode();
protected:
    //游戏难度
    int level = 1;

    int target_time = 1000;
    int target_score = 10;
    GameMode game_mode;

    int aerolite_counting = 2;

    clock_t begin = 0;

    // state 里面有: [地图] [Item] [snakes]
    Field* state;
};

class Level1: public Game
{
public:
    Level1(GameMode game_mode, int height, int width, std::vector<int> info);
    Level1(Field* state, GameMode game_mode, std::vector<int> info);
    virtual void initializeGame(int level);
    virtual bool createFood() { return true; }
};

class Level2: public Game
{
public:
    Level2(GameMode game_mode, int height, int width, std::vector<int> info);
    Level2(Field* state, GameMode game_mode, std::vector<int> info);
    virtual void initializeGame(int level);
    virtual bool createFood() { return true; }
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

class Level11: public Game
{
public:
    Level11(GameMode game_mode, int height, int width, std::vector<int> info);
    Level11(Field* state, GameMode game_mode, std::vector<int> info);
    virtual void initializeGame(int level);
};

class Greedy : public Game
{
public:
    Greedy(GameMode game_mode, int height, int width, std::vector<int> info);
    Greedy(Field* state, GameMode game_mode, std::vector<int> info);
    virtual void initializeGame(int level);
    virtual bool createSnake() { return true; }
    virtual bool createFood() { return true; }
};
#endif // GAME_H
