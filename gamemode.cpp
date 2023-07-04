#include "game.h"

/*
    以
*/
Game::Game(GameMode game_mode, int height, int width, vector<int> info):
    game_mode(game_mode),
    level(1),
    clock(Clock(0, "global", 50))   // 全局时钟从 0 开始计时, 每 50ms 运行一次
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

Game::~Game() {
    delete state;
}


void Game::initializeGame(int level) {
    /** TODO:
        给图形界面传递信息, 并给每关重置关卡胜利目标
        level 指示需要读取的文件
        比如从 level.data 里面读取所有需要的对局信息
    **/
    this->level = level;
    return;
}

void Game::snakeAction(Snake* snake) {
    /*
        1. 接收键盘按键 (方向和 Ctrl)
        2. 更改方向或加速效果
        3. 向所选方向运行
        4. 判断碰撞蛇--是否需要调用 death()
        5. 判断遇到物体--调用物体的 action()
        6. 判断是否达成游戏目标
        【】 返回一些其他需要反馈的信息 ....
    */
    Direction direction;    // 假设某种方式接收了 direction
    snake->changeDirection(direction);  // 如果接收到了信号或者
    if (snake->move()) {    // 完成移动
        if (snake->hitSelf()) {
            snake->death();
        }
    }
    vector<Snake*> other_snakes = state->snakes;
    other_snakes.erase(other_snakes.begin());
    if (snake->hitOtherSnake(other_snakes)) {
        snake->death();
    }

    // 判断胜负, 接口 TODO
    if (reachTarget() == 1) {
        cout << "======== You Win ========\n";
        return;
    } else if (reachTarget() == -1) {
        cout << "======== You Lose ========\n";
        return;
    }

    // 【】返回其他需要的信息
}

/*
    1. 读取地图, 生成计划好的物体 (无论是否 hitItem() 按全局时钟生成)
    2. 一些特殊 Item 的自己的效果
    3. 暂停时间, 全局时钟运行一轮   <-- 不用每条蛇 Action 中运行, 在 Game 域中整体运行一次
    4. 对每条蛇调用 snakeAction(Snake*)
*/
void Game::runGame() {
    this_thread::sleep_for(std::chrono::milliseconds(clock.speed));
    clock.run();
    
    // 每个时钟周期设置物体
    // 如果这个周期不想设置, 可以把 type 设成 BASIC, 则会跳过这轮周期
    // 一些应该从文件中读取的数据 ===================== //
    Loc location = state->createRandomLoc();
    ItemType type;
    int info;
    // ============================================= //
    if (type != BASIC) {    // 非 BASIC 的物体都会被设到地图上
        if (type == WARNING) {
            Item* nextItem;     // 应该从文件读取的数据
            state->createWarning(location, info, nextItem);
        } else {
            state->setItem(state->createItem(type, location, info));
        }      
    }

    // 所有蛇进行行动
    for (Snake* snake: state->snakes) {
        snakeAction(snake);
    }

    // 陨石和沼泽的特殊效果判断
    for (vector<Item*> row: state->item_map) {
        for (auto item: row) {
            if (item->getName() == AEROLITE || item->getName() == MARSH) {
                // 对每个陨石看是否砸到了蛇; 对每个沼泽看是否有蛇在上面
                // 砸到了任何一部分则另一个函数一定返回 nullptr, 因此不用 if-else 判断
                item->action(item->hitHeadSnake(state->snakes));
                item->action(item->hitBodySnake(state->snakes));
            }
        }
    }

    return;
}


/*
    赢:     返回 1
    输:     返回 -1
    继续:   返回 0
*/
int Game::reachTarget() {
    Snake* player = state->snakes[0];
    if (game_mode == TIMELIMIT) {
        if (player->eaten >= target_food) {
            return 1;   // 赢
        } else if (clock.getTime() >= target_time) {
            return -1;  // 输
        } else {
            return 0;   // 继续
        }
    } else if (game_mode == TIMEFREE) {
        if (player->eaten == target_food) {
            return 1;   // 不限时, 有足够食物就赢
        } else {
            return 0;   // 只要没死, 就一直继续
        }
    } else if (game_mode == KILLSNAKE) {
        if (player->killed >= target_kill) {
            return 1;   // 不限时, 杀足够蛇就赢
        } else {
            return 0;   // 只要没死, 就一直继续
        }
    }
}
