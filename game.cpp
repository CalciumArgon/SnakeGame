#include "game.h"
#include <QThread>
#include <fstream>
#include "aisnake.h"
#include "queue"
#include "path.h"
using namespace std;
typedef pair<int, int> Loc;
Game::Game(GameMode game_mode, int height, int width, std::vector<int> info) :
    level(1),
    clock(Clock(0, "global", 50)),// 全局时钟从 0 开始计时, 每 50ms 运行一次
    game_mode(game_mode)
{
    state = new Field(height, width);
    if (game_mode == TIMELIMIT) {
        target_food = info[0];
        target_time = info[1];
    } else if (game_mode == TIMEFREE) {
        target_food = info[0];
    } else if (game_mode == KILLSNAKE) {
        target_kill = info[0];
    }
}

Game::Game(Field *state, GameMode game_mode, std::vector<int> info) :
    level(1),
    clock(Clock(0, "global", 50)),// 全局时钟从 0 开始计时, 每 50ms 运行一次
    state(state),
    game_mode(game_mode)
{
    if (game_mode == TIMELIMIT) {
        target_food = info[0];
        target_time = info[1];
    } else if (game_mode == TIMEFREE) {
        target_food = info[0];
    } else if (game_mode == KILLSNAKE) {
        target_kill = info[0];
    }
}

bool Game::snakeAction(Snake *snake)
{
    //if (snake->getHealth() <= 0) assert(false);
    if (snake->isAI()){
        snake->changeDireciton(snake->act(this->getState()));
    }

    snake->move();   // 完成移动
    if (snake->hitSelf() || snake->hitEdge()) {
        bool dead = snake->death();
        if (dead) return false;
    }
    if (snake->hitOtherSnake(state->getSnakes())) {
        bool dead = snake->death();
        if (dead) return false;
    }
    return true;
    // TODO: 判断胜负
}

bool Game::runGame()
{
    // 删除血量为 0 的蛇
    getState()->clearSnake();

    Loc location;
    // 所有蛇进行行动 和 所撞物体产生的效果
    for (unsigned int i=0; i<state->getSnakes().size(); ++i) {
        Snake *snake = state->getSnakes()[i];

        // 未到时钟周期, 更改 cycle_record 并退出
        bool snake_action_ability = snake->ableMove();
        if (!snake_action_ability) {
            continue;
        }

        // 判断是否存活, 如果存活判断是否成功移动没有碰到障碍物 ----------
        bool alive = snake->getHealth() > 0;
        bool move_success = false;
        if (alive) {
            move_success = snakeAction(snake);
        }
        if (!move_success) {
            if (snake->isAI()) { continue; }
            else { return false; }
        }
        // -------------------------------------------------------

        snake->recover();
        Item* hit_item = snake->hitItem();
        Loc item_location = snake->getBody()[0];
        if(snake->speed_buff){
            if(snake->getMp() > 0){
                snake->addSpeed(3);
                snake->decMp();
            }
        }
        else {
            snake->incMp();
        }
        if(hit_item != nullptr)
        {
            switch (hit_item->getName()) {
            case FOOD:
            {
                hit_item->action(snake);
                do {
                    location = state->createRandomLoc();
                } while (snake->isPartOfSnake(location));
                state->deleteItem(item_location);
                state->createItem(FOOD, location, 1);
                break;
            }
            case MAGNET:
            case SHIELD:
            case FIRSTAID:
            case OBSTACLE:
            case WALL:
            {
                hit_item->action(snake);
                break;
            }
            default:    // BASIC, AEROLITE, MARSH
                break;
            }
        }

        // 检查死亡 ----------------------
        if (snake->getHealth() <= 0) {
            if (snake->isAI()) {
                continue;
            } else {
                return false;
            }
        }
        // ------------------------------

        if (snake->ableMagnetic()) {
            // 有吸铁石, 吃九宫格内且不在蛇身上的位置
            for (int i=-1; i<=1; ++i) {
                for (int j=-1; j<=1; ++j) {
                    Loc check = make_pair(item_location.first + i, item_location.second + j);
                    if (snake->isPartOfSnake(check)) {
                        continue;
                    } else {
                        hit_item = getState()->getItem(check.first, check.second);
                        if (hit_item == nullptr) continue;
                        switch (hit_item->getName()) {
                        case FOOD:
                            hit_item->action(snake);
                            state->deleteItem(check);
                            break;
                        default:
                            break;
                        }
                    }
                }
            }

        }


        if(snake->touchMarsh() != nullptr)
        {
            test = 0;
            Marsh* msh = snake->touchMarsh();
            msh->action(snake);
        }
        else {
            test = 1;
        }
    }
    return true;
}

void Game::initializeGame(int level)
{
    Loc head = std::make_pair(20, 20);
    Snake* snk = new Snake(head, 5, 1, LEFT, this->state->getMapPtr());
    this->state->addSnake(snk);
    this->level = level;
    Loc location = state->createRandomLoc();
    while(state->getSnakes()[0]->isPartOfSnake(location))
        location = state->createRandomLoc();
    ItemType type = FOOD;
    int info = 1;
    state->createItem(type, location, info);
}

bool Game::loadMap(string map_name)
{
    /*
     * Map 格式：第一行输入地图所包含的物体数量（包括任何Item）|是否确定蛇的初始化位置（0：否，1：只确定蛇头位置, 先输入方向，再输入长度，再输入蛇头位置 2：先输入方向，长度，再输入完整的蛇身坐标)
     * 方向：0：上，1：下，2：左，3：右
     * 长度：蛇的长度
     * 蛇头位置：蛇头的坐标
     * 蛇身坐标：蛇身的坐标
     * 类型：1：食物，2：墙，3：陨石，4：沼泽
     * 然后每行输入一个物体的信息 类型 + 坐标 + info
     * 输入完物体后，如果需要确定蛇的位置，则输入蛇的坐标 坐标每行一个
     * */
    ifstream map_file;
    map_file.open(map_name);
    if (!map_file.is_open()) {
        return false;
    }
    int item_num, snake_init;
    map_file >> item_num >> snake_init;
    for (int i = 0; i < item_num; i++) {
        int type, x, y, info;
        map_file >> type >> x >> y >> info;
        state->createItem((ItemType)type, Loc(x, y), info);
    }
    switch (snake_init) {
        case 0: {
            // can't guarantee the snake's body won't be in the wall or the food
            Loc head = state->createRandomLoc();
            while(state->getSnakes()[0]->isPartOfSnake(head) )
                head = state->createRandomLoc();
            Snake* snk = new Snake(head, 5, 1, LEFT, this->state->getMapPtr());
            this->state->addSnake(snk);
            break;
        }
        case 1: {
            int direction, snake_len, x, y;
            map_file >> direction >> snake_len >> x >> y;
            Loc head = Loc(x, y);
            Snake* snk = new Snake(head, snake_len, 1, (Direction)direction, this->state->getMapPtr());
            this->state->addSnake(snk);
            break;
        }
        case 2: {
            int direction, snake_len, x, y;
            map_file >> direction >> snake_len;
            vector<Loc> body;
            for (int i = 0; i < snake_len; i++) {
                map_file >> x >> y;
                body.push_back(Loc(x, y));
            }
            Snake* snk = new Snake(body, snake_len, 1, (Direction)direction, this->state->getMapPtr());
            this->state->addSnake(snk);
            break;
        }
        default:
            throw "Error: unknown snake_init type";

    }
    map_file.close();
    return true;
}

int Game::reachTarget()
{
    Snake* player = state->getSnakes()[0];
    if (game_mode == TIMELIMIT) {
        if (player->getEaten() >= target_food) {
            return 1;   // 赢
        } else if (clock.getTime() >= target_time) {
            return -1;  // 输
        } else {
            return 0;   // 继续
        }
    } else if (game_mode == TIMEFREE) {
        if (player->getEaten() == target_food) {
            return 1;   // 不限时, 有足够食物就赢
        } else {
            return 0;   // 只要没死, 就一直继续
        }
    } else if (game_mode == KILLSNAKE) {
        if (player->getKilled() >= target_kill) {
            return 1;   // 不限时, 杀足够蛇就赢
        } else {
            return 0;   // 只要没死, 就一直继续
        }
    }
}

Field *Game::getState()
{
    return state;
}


AddWallGame::AddWallGame(GameMode game_mode, int height, int width, std::vector<int> info): Game(game_mode, height, width, info){}

AddWallGame::AddWallGame(Field *state, GameMode game_mode, std::vector<int> info): Game(state, game_mode, info){}

void AddWallGame::initializeGame(int level) {

    if (!this->loadMap(WORKING_DIR + "\\map\\addwallgame.txt"))
        assert(false);
    this->level = level;
}

TestAISnake::TestAISnake(Field *state, GameMode game_mode, std::vector<int> info): Game(state, game_mode, info){}
TestAISnake::TestAISnake(GameMode game_mode, int height, int width, std::vector<int> info): Game(game_mode, height, width, info){}
void TestAISnake::initializeGame(int level) {
    Loc head = std::make_pair(20, 20);
    Snake* snk = new GreedyFood(head, 5, 1, LEFT, this->state->getMapPtr());
    this->state->addSnake(snk);
    this->level = level;
    Loc location = state->createRandomLoc();
    while(state->getSnakes()[0]->isPartOfSnake(location))
        location = state->createRandomLoc();
    ItemType type = FOOD;
    int info = 1;
    state->createItem(type, location, info);
}

bool TestAISnake::runGame() {
    Loc location;
    for (unsigned int i=0; i<state->getSnakes().size(); ++i) {
        Snake *snake = state->getSnakes()[i];
        if (snake->getHealth() <= 0) {
            continue;   // 电脑蛇死亡后不再有任何操作
        }

        // 未到时钟周期, 更改 cycle_record 并退出 -------------
        bool snake_action_ability = snake->ableMove();
        if (!snake_action_ability) {
            continue;
        }
        // ------------------------------------------------

        bool move_success = snakeAction(snake);
        if (i == 0 && !move_success) {
            return false;   // 玩家没有成功移动, 直接结束游戏
        }

        Item* hit_item = snake->hitItem();
        Loc item_location = snake->getBody()[0];
        if(hit_item != nullptr)
        {
            switch (hit_item->getName()) {
            case FOOD:
            {
                hit_item->action(snake);

                do {
                    location = state->createRandomLoc();
                } while (snake->isPartOfSnake(location));
                state->deleteItem(item_location);
                state->createItem(FOOD, location, 1);
                break;
            }
            case MAGNET:
            case SHIELD:
            case FIRSTAID:
            case OBSTACLE:
            {
                hit_item->action(snake);
                break;
            }
            case WALL:
            {
                hit_item->action(snake);
                return false;
            }
            }
        }

        if(snake->touchMarsh() != nullptr)
        {
            test = 0;
            Marsh * msh = snake->touchMarsh();
            msh->action(snake);
        }
        else {
            test = 1;
        }
        snake->recover();

        if(snake->touchMarsh() != nullptr)
        {
            test = 0;
            Marsh * msh = snake->touchMarsh();
            msh->action(snake);
        }
        else {
            test = 1;

        }

    }
    return true;
}

Level3::Level3(GameMode game_mode, int height, int width, std::vector<int> info): Game(game_mode, height, width, info){}

Level3::Level3(Field *state, GameMode game_mode, std::vector<int> info): Game(state, game_mode, info){}

void Level3::initializeGame(int level) {

    if (!this->loadMap(WORKING_DIR + "\\map\\level3.txt"))
        assert(false);
    this->level = level;
}

Level4::Level4(GameMode game_mode, int height, int width, std::vector<int> info): Game(game_mode, height, width, info){}
Level4::Level4(Field *state, GameMode game_mode, std::vector<int> info): Game(state, game_mode, info){}
void Level4::initializeGame(int level) {

    if (!this->loadMap(WORKING_DIR + "\\map\\level4.txt"))
        assert(false);
    this->level = level;
    queue<Loc> path;
    path.push(make_pair(16, 15));
    path.push(make_pair(25, 15));
    path.push(make_pair(25, 25));
    path.push(make_pair(16, 25));
    Snake* snake = new WalkingSnake(path, {16, 20}, 5, 1, UP, this->state->getMapPtr());
    Snake* snake2 = new GreedyFood({5, 5}, 2, 1, DOWN, this->state->getMapPtr());
    this->state->addSnake(snake);
}

Level5::Level5(GameMode game_mode, int height, int width, std::vector<int> info): Game(game_mode, height, width, info){}

Level5::Level5(Field *state, GameMode game_mode, std::vector<int> info): Game(state, game_mode, info){}

void Level5::initializeGame(int level) {

    if (!this->loadMap(WORKING_DIR + "\\map\\level5.txt"))
        assert(false);
    this->level = level;
}
