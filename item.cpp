

#include "item.h"

Item::Item(Loc location): location(location) {}

Snake* Item::hitHeadSnake(vector<Snake*> snakes) {
    for (Snake* snake: snakes) {
        if (this->location == snake->body[0]) {
            return snake;
        }
    }
    return nullptr;
}

Snake* Item::hitBodySnake(vector<Snake*> snakes) {
    for (Snake* snake: snakes) {
        for (int i=1; i<snake->body.size(); ++i) {
            if (snake->body[i] == this->location) {
                return snake;
            }
        }
    }
    return nullptr;
}

itemType Item::getName() { return name; }

// 父类的 action() 不应该被调用 -------------------------------
void Item::action(Snake*) {
    cout << "Calling the wrong action() in <class 'Item'>\n";
}
// ----------------------------------------------------------


Food::Food(Loc location, int add_length):
    Item(location),
    add_length(add_length) {}

void Food::changeAddLength(int newLen) {
    this->add_length = newLen;
}

void Food::action(Snake* snake) {
    if (snake == nullptr) { return; }
    snake->addLength(add_length);
}

itemType Food::getName() { return name; }


Magnet::Magnet(Loc location, int effective_time):
    Item(location),
    effective_time(effective_time),
    remain_time(effective_time) // 好像没什么用, 在 Snake 里有倒计时
    {}

void Magnet::action(Snake* snake) {
    if (snake == nullptr) { return; }
    snake->magnetic = effective_time;
}

itemType Magnet::getName() { return name; }


Shield::Shield(Loc location, int effective_time):
    Item(location),
    effective_time(effective_time) {}

void Shield::action(Snake* snake) {
    if (snake == nullptr) { return; }
    snake->revival = effective_time;
}

itemType Shield::getName() { return name; }


Firstaid::Firstaid(Loc location, int add_health):
    Item(location),
    add_health(add_health) {}

void Firstaid::changeAddHealth(int newHealth) {
    this->add_health = newHealth;
}

void Firstaid::action(Snake* snake) {
    if (snake == nullptr) { return; }
    // 最多加到满血
    snake->health = min(snake->MAX_health, snake->health + add_health);
}

itemType Firstaid::getName() { return name; }


Warning::Warning(Loc location, int counting, Item* nextItem):
    Item(location),
    counting(counting),
    nextItem(nextItem) {}

void Warning::action(Snake* snake) {
    // 在 Game 中通过 getName() 分辨出 Warning实例
    // 然后单独判断 action() 后是否 counting == 0
    // 是 --> 生成 nextItem 在原位置
    // 否 --> pass    
    counting -= 1;
}

itemType Warning::getName() { return name; }


Obstacle::Obstacle(Loc location, int injury):
    Item(location),
    injury(injury) {}

void Obstacle::action(Snake* snake) {
    if (snake == nullptr) { return; }
    snake->decreaseHealth(injury);
}

itemType Obstacle::getName() { return name; }


Wall::Wall(Loc location):
    Item(location) {}

void Wall::action(Snake* snake) {
    if (snake == nullptr) { return; }
    snake->death();
}

itemType Wall::getName() { return name; }


Aerolite::Aerolite(Loc location):
    Item(location) {}

void Aerolite::action(Snake* snake) {
    if (snake == nullptr) { return; }
    /*
        陨石的工作原理:
        在 Game 运行函数中每个时钟周期找到 Field.grid 中的所有陨石
        这些陨石运行 hitHeadSnake() 和 hitBodySnake()
        将它们返回的 Snake* (如果有的话) 传进 Aerolite.action() 里
    */
    if (location == snake->body[0]) {
        snake->death();
    } else {
        for (int i=snake->length-1; i>=0; --i) {
            if (location == snake->body[i]) {       // 砸中此段, 则从此处之后都截断
                snake->body.pop_back();
                snake->length = snake->body.size();
                break;
            }
            snake->body.pop_back();
        }
    }
}

itemType Aerolite::getName() { return name; }


Marsh::Marsh(Loc location, int decelerate):
    Item(location),
    decelerate(decelerate) {}

void Marsh::action(Snake* snake) {
    if (snake == nullptr) { return; }
    snake->speed -= decelerate;
}

itemType Marsh::getName() { return name; }
