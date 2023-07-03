
#include "snake.h"

Snake::Snake(vector<Loc> body, int length, int MAX_health, Direction direction, Grid itemMap):
    body(body),
    length(length),
    MAX_health(MAX_health),
    direction(direction),
    itemMap(itemMap) {}

Snake::Snake(Loc head, int length, int MAX_health, Direction direction, Grid itemMap):
    length(length),
    MAX_health(MAX_health),
    direction(direction),
    itemMap(itemMap)
{
    body.clear();
    body.push_back(head);
    int x = head.first, y = head.second;
    if (direction == Direction::up) {    // 头方向朝上, 身体在头下方
        for (int i=1; i<length; i++) {
            body.push_back(make_pair(x-i, y));
        }
    } else if (direction == Direction::down) {
        for (int i=1; i<length; i++) {
            body.push_back(make_pair(x+i, y));
        }
    } else if (direction == Direction::left) {
        for (int i=1; i<length; i++) {
            body.push_back(make_pair(x, y+i));
        }
    } else if (direction == Direction::right) {
        for (int i=1; i<length; i++) {
            body.push_back(make_pair(x, y-i));
        }
    }
    
    if (length != body.size()) { cout << "Initialization body fail, length goes wrong !!!\n"; }
}

void Snake::initialize() {
    health = MAX_health;
    speed = 3;
    cycle_recorder = 1;
    eaten = 0;
    direction = up;
    magnetic = 0;
    revival = 0;
}

void Snake::changeDirection(Direction newDirection) {
    // 必须是 "up" "down" "left" "right" 中的一个
    // 之后所有函数都不写错误处理了, 自己写的时候小心一点
    direction = newDirection;
}

Loc Snake::nextLoc() {
    int x = body[0].first, y = body[0].second;
    if (direction == Direction::up) { return make_pair(x-1, y); }
    else if (direction == Direction::down) { return make_pair(x+1, y); }
    else if (direction == Direction::left) { return make_pair(x, y-1); }
    else if (direction == Direction::right) { return make_pair(x, y+1); }
}

void Snake::move() {
    Loc new_head = nextLoc();
    body.insert(body.begin(), new_head);
    body.pop_back();

    Item* hit_item = hitItem();
    if (hit_item != nullptr && hit_item->getName() != "aerolite" && hit_item->getName() != "Marsh") {
        hit_item->action(this);
    }
}

/*
    在身体最后沿倒数两段身体的连线方向增加 adding 段
    超出地图边界/碰到任何 Item 的身体及之后的部分都不再加载
*/
void Snake::addLength(int adding) {
    // second last X/Y 倒数第二段身体
    int sl_x = body[length-2].first, sl_y = body[length-2].second;
    // last X/Y 最后一段身体
    int l_x = body[length-1].first, l_y = body[length-1].second;

    // new - last = last - slast
    // new = last + delta
    int delta_x = l_x - sl_x, delta_y = l_y - sl_y;
    for (int i=1; i<=adding; ++i) {
        int newX = l_x + i*delta_x, newY = l_y + i*delta_y;
        if (!isWithin(newX, 0, itemMap.size()-1) || !isWithin(newY, 0, itemMap[0].size()-1)) {
            // 新坐标不在地图边界里
            break;
        }
        if (itemMap[newX][newY] != nullptr) {
            // 新坐标上有物体
            break;
        }
        body.push_back(make_pair(newX, newY));
    }
}

Item* Snake::hitItem() {
    return itemMap[body[0].first][body[0].second];
}

bool Snake::hitSelf() {
    Loc head = body[0];
    for (int i=1; i<length; ++i) {
        if (head == body[i]) {
            return true;
        }
    }
    return false;
}

bool Snake::hitOtherSnake(vector<Snake*> snakes) {
    Loc head = body[0];
    for (Snake* other: snakes) {
        for (Loc other_body: other->body) {
            if (head == other_body) {
                return true;
            }
        }
    }
    return false;
}

void Snake::decreaseHealth(int injury) {
    this->health -= injury;
    if (health <= 0) {
        this->death();
    }
}

void Snake::death() {
    if (revival > 0) {
        int previous_eaten = eaten; // 之前吃过的食物数量不清零
        initialize();
        health = 0.8 * MAX_health;
        eaten = previous_eaten;
        body = Snake( make_pair(itemMap.size() / 2, itemMap[0].size() / 2),
                      length,
                      MAX_health,
                      Direction::up,
                      itemMap ).body;   // 用中间初始化的一条新蛇的身体来更新当前蛇的复活状态至地图中央
    } else {
        // TODO: 返回接口 传递死亡的信息
        cout << "====== Death ======\n";
    }
}
