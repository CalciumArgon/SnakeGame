#include "game.h"
#include <QThread>

using namespace std;

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

bool Game::snakeAction(Snake *)
{
    Snake* msnake = state->getSnakes()[0];
    if (msnake->move()) {    // 完成移动
        if (msnake->hitSelf() || msnake->hitEdge()) {
            return (!msnake->death());
        }
    }
    /*
    vector<Snake*> other_snakes = state->getSnakes();
    other_snakes.erase(other_snakes.begin());
    if (snake->hitOtherSnake(other_snakes)) {
        snake->death();
    }*/

    // 判断胜负, 接口 TODO
}

void Game::runGame()
{
    while(1){
    clock.run();

    // 每个时钟周期设置物体
    // 如果这个周期不想设置, 可以把 type 设成 BASIC, 则会跳过这轮周期
    // 一些应该从文件中读取的数据 ===================== //
    Loc location = state->createRandomLoc();
    ItemType type = FOOD;
    int info = 1;
    // ============================================= //
    if (type != BASIC) {    // 非 BASIC 的物体都会被设到地图上
            state->createItem(type, location, info);
    }

    // 所有蛇进行行动
    bool maction = true;
    Snake* msnake = state->getSnakes()[0];
    for (Snake* snake: state->getSnakes()) {
        maction = snakeAction(snake);
    }
    if(!maction) return;
    if(msnake->hitItem()->getName() == FOOD)
    {
        state->createItem(BASIC, msnake->getBody()[0], 0);
        state->createItem(FOOD, location, info);
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
    QThread::msleep(50);
    }
}

void Game::initializeGame(int level)
{
    this->level = level;
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
