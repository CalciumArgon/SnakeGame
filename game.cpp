#include "game.h"
#include <QThread>
#include <fstream>
#include "AISnake.h"
using namespace std;

Game::Game(GameMode playMode, int height, int width, std::vector<int> info) :
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

bool Game::snakeAction(Snake *)
{
    Snake* msnake = state->getSnakes()[0];
    msnake->move();   // 完成移动
    if (msnake->hitSelf() || msnake->hitEdge()) {
        return false;
    }
    return true;
    /*
    vector<Snake*> other_snakes = state->getSnakes();
    other_snakes.erase(other_snakes.begin());
    if (snake->hitOtherSnake(other_snakes)) {
        snake->death();
    }*/

    // 判断胜负, 接口 TODO
}

bool Game::runGame()
{
    //clock.run();

    // 每个时钟周期设置物体
    // 如果这个周期不想设置, 可以把 type 设成 BASIC, 则会跳过这轮周期
    // 一些应该从文件中读取的数据 ===================== //
    Loc location;
    // ============================================= //
    /*if (type != BASIC) {    // 非 BASIC 的物体都会被设到地图上
            state->createItem(type, location, info);
    }*/

    // 所有蛇进行行动
    bool maction = true;
    Snake* msnake = state->getSnakes()[0];
    maction = snakeAction(msnake);
    if(!maction) return false;
    if(msnake->hitItem() != nullptr )
    {
        switch (msnake->hitItem()->getName()) {
            case FOOD:{
                location = state->createRandomLoc();
                while(msnake->isPartOfSnake(location))
                    location = state->createRandomLoc();
                state->createItem(BASIC, msnake->getBody()[0], 0);
                state->createItem(FOOD, location, 1);
                break;
            }
            case WALL: {
                // dead
                return false;
            }
            default:
                throw "Error: unknown item type";
        }

    }
    // 陨石和沼泽的特殊效果判断
    /*for (vector<Item*> row: *state->getMapPtr()) {
        for (auto item: row) {
            if (item->getName() == AEROLITE || item->getName() == MARSH) {
                // 对每个陨石看是否砸到了蛇; 对每个沼泽看是否有蛇在上面
                // 砸到了任何一部分则另一个函数一定返回 nullptr, 因此不用 if-else 判断
                item->action(item->hitHeadSnake(state->snakes));
                item->action(item->hitBodySnake(state->snakes));
            }
        }
    }*/
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

    if (!this->loadMap("F:\\OneDrive - sjtu.edu.cn\\Documents\\university_life\\grade_one_summer\\snake_src_full\\map\\addwallgame.txt"))
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
        //clock.run();

        // 每个时钟周期设置物体
        // 如果这个周期不想设置, 可以把 type 设成 BASIC, 则会跳过这轮周期
        // 一些应该从文件中读取的数据 ===================== //
        Loc location;
        // ============================================= //
        /*if (type != BASIC) {    // 非 BASIC 的物体都会被设到地图上
                state->createItem(type, location, info);
        }*/

        // 所有蛇进行行动
        bool maction = true;
        Snake* msnake = state->getSnakes()[0];
        maction = snakeAction(msnake);
        if(!maction) return false;
        if(msnake->hitItem() != nullptr )
        {
            switch (msnake->hitItem()->getName()) {
                case FOOD:{
                    location = state->createRandomLoc();
                    while(msnake->isPartOfSnake(location))
                        location = state->createRandomLoc();
                    state->createItem(BASIC, msnake->getBody()[0], 0);
                    state->createItem(FOOD, location, 1);
                    break;
                }
                case WALL: {
                    // dead
                    return false;
                }
                default:
                    throw "Error: unknown item type";
            }

        }
        // 陨石和沼泽的特殊效果判断
        /*for (vector<Item*> row: *state->getMapPtr()) {
            for (auto item: row) {
                if (item->getName() == AEROLITE || item->getName() == MARSH) {
                    // 对每个陨石看是否砸到了蛇; 对每个沼泽看是否有蛇在上面
                    // 砸到了任何一部分则另一个函数一定返回 nullptr, 因此不用 if-else 判断
                    item->action(item->hitHeadSnake(state->snakes));
                    item->action(item->hitBodySnake(state->snakes));
                }
            }
        }*/
        return true;
    }