#include "game.h"
#include <QThread>
#include <fstream>
#include "aisnake.h"
#include "queue"
using namespace std;
typedef pair<size_t, size_t> Loc;
Game::Game(GameMode game_mode, size_t height, size_t width, std::vector<int> info) :
        level(1),
        game_mode(game_mode)
{
    state = new Field(height, width);
    target_score = info[0];
    target_time = info[1];
}

Game::Game(Field *state, GameMode game_mode, std::vector<int> info) :
        level(1),
        game_mode(game_mode),
        state(state)
{
    target_score = info[0];
    target_time = info[1];
}

Game::~Game() {
    delete this->state;
}


void Game::setBeginTime(clock_t begin) {
    this->begin = begin;
}


bool Game::snakeAction(Snake *snake) {
    //if (snake->getHealth() <= 0) assert(false);
    if (snake->isAI()) {
        snake->changeDireciton(snake->act(this->getState()));
    }

    snake->move();   // 完成移动
    if (snake->hitSelf() || snake->hitEdge()) {
        bool dead = snake->death();
        if (dead){
            if (snake->isAI()) {
                this->state->getSnakes()[0]->incKilled();
                return false;
            }
            else {
                return false;
            }
        }
    }
    if (snake->hitOtherSnake(state->getSnakes())) {
        bool dead = snake->death();
        if (dead) {
            if (snake->isAI()) {
                this->state->getSnakes()[0]->incKilled();
            }
            return false;
        }
    }
    return true;
}

short Game::runGame() {
    // 删除血量为 0 的蛇
    getState()->clearSnake();
    Loc location;
    if (this->createSnake()) {
        if (state->getSnakes().size() == 1) {
            do {
                location = state->createRandomLoc();
            } while (state->getSnakes()[0]->isPartOfSnake(location) || state->getMapPtr()->at(location.first)[location.second] != 0);
            int direction = rand() % 4;
            Snake* snake = new GreedyFood(location, 2, 1, (Direction)direction, this->state->getMapPtr());
            this->state->addSnake(snake);
        }
    }

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
            else { return -1; }
        }
        // -------------------------------------------------------

        snake->recover();
        Item* hit_item = snake->hitItem();
        Loc item_location = snake->getBody()[0];
        if (snake->speed_buff) {
            if (snake->getMp() > 0) {
                snake->addSpeed(5);
                snake->decMp();
            }
        } else {
            snake->incMp();
        }
        if (hit_item != nullptr) {
            switch (hit_item->getName()) {
                case FOOD:
                {
                    hit_item->action(snake);
                    state->deleteItem(item_location);
                    if (this->createFood()) {
                        do {
                            location = state->createRandomLoc();
                        } while (snake->isPartOfSnake(location));
                        state->createItem(FOOD, location, 1);
                    }

                    break;
                }
                case MAGNET:
                case SHIELD:
                case FIRSTAID:
                case OBSTACLE:
                    state->deleteItem(item_location);
                    hit_item->action(snake);
                    break;
                case WALL:
                {
                    hit_item->action(snake);
                    break;
                }
                default:    // BASIC, AEROLITE, MARSH
                    break;
            }
        }
        if (snake->getMagnetic() > 0)
            snake->decMagnectic();

        // 检查陨石
        if (this->isFall()) {
            Aerolite* aerolite = snake->touchAerolite();
            if (aerolite != nullptr) {
                aerolite->action(snake);
            }
        }
        this->countDown();  // 新的一轮陨石倒计时


        // 检查死亡 ----------------------
        if (snake->getHealth() <= 0) {
            if (snake->isAI()) {
                continue;
            } else {
                return -1;
            }
        }
        // ------------------------------

        if (snake->ableMagnetic()) {
            // 有吸铁石, 吃九宫格内且不在蛇身上的位置
            for (int i=-1; i<=1; ++i) {
                for (int j=-1; j<=1; ++j) {
                    int new_w = int(item_location.first) + i;
                    int new_h = int(item_location.first) + j;
                    if (new_w < 0 || new_h < 0) {
                        // 不在地图内
                        continue;
                    }
                    Loc check = make_pair(new_w, new_h);
                    if (snake->isPartOfSnake(check)) {
                        // 在蛇身上的部分不检查
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
        if (snake->getMagnetic() > 0)
            snake->decMagnectic();

        //检查沼泽减速
        if (snake->touchMarsh() != nullptr) {
            Marsh* msh = snake->touchMarsh();
            msh->action(snake);
        }
    }

    // 胜负判断
    short game_status = reachTarget();
    return game_status;
}

void Game::initializeGame(int level) {
    Loc head = std::make_pair(20, 20);
    Snake* snk = new Snake(head, 5, 1, LEFT, this->state->getMapPtr());
    this->state->addSnake(snk);
    this->state->getSnakes()[0]->level = level;
    Loc location = state->createRandomLoc();
    while(state->getSnakes()[0]->isPartOfSnake(location))
        location = state->createRandomLoc();
    ItemType type = FOOD;
    int info = 1;
    state->createItem(type, location, info);
}

void Game::countDown() {
    aerolite_counting = (aerolite_counting + 16) % 45 - 15;
}

bool Game::isFall() {
    return this->aerolite_counting < 0;
}

bool Game::isWarning() {
    return (aerolite_counting > 0);
}

bool Game::loadMap(string map_name) {
    /*
     * Map 格式：第一行输入地图所包含的物体数量（包括任何Item）|是否确定蛇的初始化位置（0：否，1：只确定蛇头位置, 先输入方向，再输入长度，再输入蛇头位置 2：先输入方向，长度，再输入完整的蛇身坐标)
     * 11: 在 1 的基础上，输入蛇的max_health
     * 方向：0：上，1：下，2：左，3：右
     * 长度：蛇的长度
     * 蛇头位置：蛇头的坐标
     * 蛇身坐标：蛇身的坐标
     * 类型：1：食物，2：墙，3：陨石，4：沼泽，5：吸铁石，6：护盾，7：血包，8：障碍物
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
            int direction, x, y;
            size_t snake_len;
            map_file >> direction >> snake_len >> x >> y;
            Loc head = Loc(x, y);
            Snake* snk = new Snake(head, snake_len, 1, (Direction)direction, this->state->getMapPtr());
            this->state->addSnake(snk);
            break;
        }
        case 2: {
            int direction, x, y;
            size_t snake_len;
            map_file >> direction >> snake_len;
            vector<Loc> body;
            for (size_t i = 0; i < snake_len; i++) {
                map_file >> x >> y;
                body.push_back(Loc(x, y));
            }
            Snake* snk = new Snake(body, snake_len, 1, (Direction)direction, this->state->getMapPtr());
            this->state->addSnake(snk);
            break;
        }
        case 11:{
            int direction, x, y, max_health;
            size_t snake_len;
            map_file >> direction >> snake_len >> x >> y >> max_health;
            Loc head = Loc(x, y);
            Snake* snk = new Snake(head, snake_len, max_health, (Direction)direction, this->state->getMapPtr());
            this->state->addSnake(snk);
            break;
        }
        default:
            throw "Error: unknown snake_init type";
    }
    map_file.close();
    return true;
}

short Game::reachTarget() {
    Snake* player = state->getSnakes()[0];
    clock_t end = std::clock();
    if (player->score() >= this->target_score) {
        return 1;
    } else if ((end - this->begin) > CLK_TCK * target_time) {
        return -1;
    } else {
        return 0;
    }
}

Field *Game::getState() {
    return state;
}

int Game::getTargetTime() {
    return target_time;
}

int Game::getTargetScore() {
    return target_score;
}

GameMode Game::getMode() {
    return game_mode;
}

Level1::Level1(Field *state, GameMode game_mode, std::vector<int> info): Game(state, game_mode, info) {}
Level1::Level1(GameMode game_mode, int height, int width, std::vector<int> info): Game(game_mode, height, width, info) {}
void Level1::initializeGame(int level) {
    Loc head = std::make_pair(20, 20);
    Snake* snk = new Snake(head, 5, 1, LEFT, this->state->getMapPtr());
    this->state->addSnake(snk);
    this->state->getSnakes()[0]->level = level;
    Loc location = state->createRandomLoc();
    while(state->getSnakes()[0]->isPartOfSnake(location))
        location = state->createRandomLoc();
    ItemType type = FOOD;
    int info = 1;
    state->createItem(type, location, info);
    this->state->getSnakes()[0]->level = level;
}

Level2::Level2(GameMode game_mode, int height, int width, std::vector<int> info): Game(game_mode, height, width, info) {}

Level2::Level2(Field *state, GameMode game_mode, std::vector<int> info): Game(state, game_mode, info) {}


void Level2::initializeGame(int level) {
    if (!this->loadMap(".\\map\\addwallgame.txt"))
        assert(false);
    this->state->getSnakes()[0]->level = level;
}

Level3::Level3(GameMode game_mode, int height, int width, std::vector<int> info): Game(game_mode, height, width, info) {}

Level3::Level3(Field *state, GameMode game_mode, std::vector<int> info): Game(state, game_mode, info) {}

void Level3::initializeGame(int level) {
    if (!this->loadMap(".\\map\\level3.txt"))
        assert(false);
    this->state->getSnakes()[0]->level = level;
}

Level4::Level4(GameMode game_mode, int height, int width, std::vector<int> info): Game(game_mode, height, width, info) {}
Level4::Level4(Field *state, GameMode game_mode, std::vector<int> info): Game(state, game_mode, info) {}
void Level4::initializeGame(int level) {
    if (!this->loadMap(".\\map\\level4.txt"))
        assert(false);
    this->state->getSnakes()[0]->level = level;
    queue<Loc> path;
    path.push(make_pair(16, 15));
    path.push(make_pair(25, 15));
    path.push(make_pair(25, 25));
    path.push(make_pair(16, 25));
    Snake* snake = new WalkingSnake(path, {16, 20}, 5, 1, UP, this->state->getMapPtr());
    // Snake* snake2 = new GreedyFood({5, 5}, 2, 1, DOWN, this->state->getMapPtr());
    this->state->addSnake(snake);
}

Level5::Level5(GameMode game_mode, int height, int width, std::vector<int> info): Game(game_mode, height, width, info) {}

Level5::Level5(Field *state, GameMode game_mode, std::vector<int> info): Game(state, game_mode, info) {}

void Level5::initializeGame(int level) {
    if (!this->loadMap(".\\map\\level5.txt"))
        assert(false);
    this->state->getSnakes()[0]->level = level;
}

Level6::Level6(GameMode game_mode, int height, int width, std::vector<int> info): Game(game_mode, height, width, info) {}
Level6::Level6(Field *state, GameMode game_mode, std::vector<int> info): Game(state, game_mode, info) {}
void Level6::initializeGame(int level) {
    if (!this->loadMap(".\\map\\level6.txt"))
        assert(false);
    this->state->getSnakes()[0]->level = level;
}

Level7::Level7(GameMode game_mode, int height, int width, std::vector<int> info): Game(game_mode, height, width, info) {}
Level7::Level7(Field *state, GameMode game_mode, std::vector<int> info): Game(state, game_mode, info) {}
void Level7::initializeGame(int level) {
    if (!this->loadMap(".\\map\\level7.txt"))
        assert(false);
    this->state->getSnakes()[0]->level = level;
}

Level8::Level8(GameMode game_mode, int height, int width, std::vector<int> info): Game(game_mode, height, width, info) {}
Level8::Level8(Field *state, GameMode game_mode, std::vector<int> info): Game(state, game_mode, info) {}
void Level8::initializeGame(int level) {
    if (!this->loadMap(".\\map\\level8.txt"))
        assert(false);
    this->state->getSnakes()[0]->level = level;
    queue<Loc> path;
    path.push(make_pair(30, 13));
    path.push(make_pair(30, 28));
    path.push(make_pair(7, 28));
    path.push(make_pair(7, 13));
    // this->state->getSnakes()[0]->addSpeed(-100000);
    Snake* snake1 = new WalkingSnake(path, {11, 13}, 5, 1, RIGHT, this->state->getMapPtr());
    Snake* snake2 = new WalkingSnake(path, {22, 13},8, 1, RIGHT, this->state->getMapPtr());
    Snake* snake3 = new WalkingSnake(path,{29, 13}, 4, 1, RIGHT, this->state->getMapPtr());
    Loc temp_target = path.front();
    path.pop();
    path.push(temp_target);
    Snake* snake4 = new WalkingSnake(path, {30, 20}, 6, 1, DOWN, this->state->getMapPtr());
    Snake* snake5 = new WalkingSnake(path, {30, 27}, 5, 1, DOWN, this->state->getMapPtr());
    temp_target = path.front();
    path.pop();
    path.push(temp_target);
    Snake* snake6 = new WalkingSnake(path, {17, 28}, 8, 1, LEFT, this->state->getMapPtr());
    Snake* snake7 = new WalkingSnake(path, {8, 28}, 7, 1, LEFT, this->state->getMapPtr());
    temp_target = path.front();
    path.pop();
    path.push(temp_target);
    Snake* snake8 = new WalkingSnake(path, {7, 15}, 12, 1, UP, this->state->getMapPtr());
    this->state->addSnake(snake1);
    this->state->addSnake(snake2);
    this->state->addSnake(snake3);
    this->state->addSnake(snake4);
    this->state->addSnake(snake5);
    this->state->addSnake(snake6);
    this->state->addSnake(snake7);
    this->state->addSnake(snake8);
}

Level9::Level9(GameMode game_mode, int height, int width, std::vector<int> info): Game(game_mode, height, width, info) {}
Level9::Level9(Field *state, GameMode game_mode, std::vector<int> info): Game(state, game_mode, info) {}
void Level9::initializeGame(int level) {
    if (!this->loadMap(".\\map\\level9.txt"))
        assert(false);
    this->state->getSnakes()[0]->level = level;
}

Level10::Level10(GameMode game_mode, int height, int width, std::vector<int> info): Game(game_mode, height, width, info) {}
Level10::Level10(Field *state, GameMode game_mode, std::vector<int> info): Game(state, game_mode, info) {}
void Level10::initializeGame(int level) {
    if (!this->loadMap(".\\map\\level10.txt"))
        assert(false);
    this->state->getSnakes()[0]->level = level;
}

Level11::Level11(GameMode game_mode, int height, int width, std::vector<int> info): Game(game_mode, height, width, info) {}

Level11::Level11(Field* state, GameMode game_mode, std::vector<int> info): Game(state, game_mode, info) {}

void Level11::initializeGame(int level) {
    if (!this->loadMap(".\\map\\level11.txt"))
        assert(false);
    this->state->getSnakes()[0]->level = level;
    queue<Loc> path1;
    Loc birthplace1 = {2, 28};
    path1.push(make_pair(2, 34));
    path1.push(make_pair(4, 34));
    path1.push(make_pair(4, 36));
    path1.push(make_pair(7, 36));
    path1.push(make_pair(7, 34));
    path1.push(make_pair(5, 34));
    path1.push(make_pair(5, 30));
    path1.push(make_pair(4, 30));
    path1.push(make_pair(4, 28));
    path1.push(make_pair(2, 28));
    Snake* snake1 = new WalkingSnake(path1, birthplace1, 3, 1, LEFT, this->state->getMapPtr());
    this->state->addSnake(snake1);
    queue<Loc> path2;
    Loc birthplace2 = {28, 37};
    path2.push(make_pair(28, 38));
    path2.push(make_pair(35, 38));
    path2.push(make_pair(35, 30));
    path2.push(make_pair(38, 30));
    path2.push(make_pair(38, 35));
    path2.push(make_pair(28, 35));
    Snake* snake2 = new WalkingSnake(path2, birthplace2, 3, 1, DOWN, this->state->getMapPtr());
    this->state->addSnake(snake2);
}

Greedy::Greedy(Field *state, GameMode game_mode, std::vector<int> info): Game(state, game_mode, info) {}
Greedy::Greedy(GameMode game_mode, int height, int width, std::vector<int> info): Game(game_mode, height, width, info) {}
void Greedy::initializeGame(int level) {
    if (!this->loadMap(".\\map\\greedy.txt"))
        assert(false);
    this->state->getSnakes()[0]->level = level;
    Snake* snake = new GreedyFood({5, 5}, 2, 1, DOWN, this->state->getMapPtr());
    this->state->addSnake(snake);
}
