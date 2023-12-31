#include "item.h"
#include "snake.h"

/****************************************************
    物品类:
    检查击中了哪条蛇
    返回具体子类的物体类别 ItemType
    子类各自重写的效果函数 action()
*/
Item::Item(Loc location) : location(location) {}

Loc Item::getLoc() {
    return this->location;
}

Snake *Item::hitHeadSnake(vector<Snake *> snakes) {
    for (Snake* snake: snakes) {
        if (this->location == snake->getBody()[0]) {
            return snake;
        }
    }
    return nullptr;
}

Snake *Item::hitBodySnake(vector<Snake *> snakes) {
    for (Snake* snake: snakes) {
        for (int i=1; i<snake->getLength(); ++i) {
            if (snake->getBody()[i] == this->location) {
                return snake;
            }
        }
    }
    return nullptr;
}


/****************************************************
    普通食物
    增加的长度默认为 1, 可通过 changeAddLength() 修改
*/
Food::Food(Loc location, int add_length):
    Item(location),
    add_length(add_length)
    {}

void Food::action(Snake *snake) {
    if (snake == nullptr) { return; }
    snake->addLength(add_length);
    snake->incEaten();
}


void Food::changeAddLength(int new_len) {
    this->add_length = new_len;
}

void Food::initWarningArea(vector<Loc> areas, size_t range) {
    this->warning_area = areas;
    this->range = range;
}

vector<Loc> Food::getWarningArea() {
    return this->warning_area;
} 

/****************************************************
    吸铁石
    让 Snake实例在 effective_time 时间里能吸收头部四周九宫格的 Item
    effective_time 跟随全局时钟变化
*/
Magnet::Magnet(Loc location, int effective_time):
    Item(location),
    effective_time(effective_time)
    {}

void Magnet::action(Snake* snake) {
    if (snake == nullptr) { return; }
    snake->setMagnetic(this->effective_time);
}


/****************************************************
    护盾
    让 Snake实例在 effective_time 时间里具有护盾, 保护一次死亡
    effective_time 跟随全局时钟变化
    (护盾的保护机制在 Snake类中实现)
*/
Shield::Shield(Loc location, int effective_time):
    Item(location),
    effective_time(effective_time) {}

void Shield::action(Snake* snake) {
    if (snake == nullptr) { return; }
    snake->setRevival(this->effective_time);
}


/****************************************************
    血包
    增加的血量默认为 1, 最多达到最大血量
*/
Firstaid::Firstaid(Loc location, int add_health):
    Item(location),
    add_health(add_health) {}

void Firstaid::changeAddHealth(int newHealth) {
    this->add_health = newHealth;
}

void Firstaid::action(Snake* snake) {
    if (snake == nullptr) { return; }
    // 最多加到满血
    snake->addHealth(this->add_health);
}


/****************************************************
    障碍
    不同障碍物有不同伤害量
*/
Obstacle::Obstacle(Loc location, int injury):
    Item(location),
    injury(injury) {}

void Obstacle::action(Snake* snake) {
    if (snake == nullptr) { return; }
    snake->addHealth(-injury);
}


/****************************************************
    墙壁
    和地图边界相同 撞击死亡
*/
Wall::Wall(Loc location): Item(location) {}

void Wall::action(Snake *snake) {
    if (snake == nullptr) { return; }
    snake->death();
}


/****************************************************
    陨石
    砸中头部死亡, 砸中身体则从砸中处截断以后部分
*/
Aerolite::Aerolite(Loc location):
    Item(location) {}

void Aerolite::action(Snake* snake) {
    if (snake == nullptr) { return; }
    /*
        陨石的工作原理:
        在 Game 运行函数中每个时钟周期找到 item_map_ptr 中的所有陨石
        这些陨石运行 hitHeadSnake() 和 hitBodySnake()
        将它们返回的 Snake* (如果有的话) 传进 Aerolite.action() 里
    */
    if (location == snake->getBody()[0]) {
        snake->death();
    } else {
        for (size_t i=snake->getLength()-1; i>=0; --i) {
            snake->addLength(-1);
            if (this->location == snake->getBody()[i]) {       // 砸中此段, 则从此处之后都截断
                break;
            }
        }
    }
}


/****************************************************
    沼泽
    只要蛇任何一部分在沼泽上 都将被减速
*/
Marsh::Marsh(Loc location):
    Item(location)
     {}

void Marsh::action(Snake* snake) {
    if (snake == nullptr) { return; }
    snake->addSpeed(-this->decelerate);
}

