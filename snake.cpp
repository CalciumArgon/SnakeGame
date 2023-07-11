#include "snake.h"

using namespace std;

inline int min(int x1, int x2) {
    return (x1 <= x2) ? x1 : x2;
}

inline int max(int x1, int x2) {
    return (x1 >= x2) ? x1 : x2;
}

Snake::Snake(vector<Loc> body, int length, int health, Direction direction, Grid* item_map_ptr) :
    body(body),
    length(length),
    direction(direction),
    item_map_ptr(item_map_ptr),
    health(health)
{
    width = item_map_ptr->size();
    height = (*item_map_ptr)[0].size();
    rebornLocation = body[0];
    rebornDirection = direction;
}

Snake::Snake(Loc head, int length, int health, Direction direction, Grid* item_map_ptr) :
    length(length),
    direction(direction),
    item_map_ptr(item_map_ptr),
    health(health),
    rebornLocation(head),
    rebornDirection(direction)
{
    body.clear();
    body.push_back(head);
    int x = head.first, y = head.second;
    switch(direction)
    {
    case UP:
        for (int i=1; i<length; i++) {
            body.push_back(make_pair(x, y+i));
        }
        break;

     case DOWN:
        for (int i=1; i<length; i++) {
            body.push_back(make_pair(x, y-i));
        }
        break;
    case LEFT:
        for (int i=1; i<length; i++) {
            body.push_back(make_pair(x+i, y));
        }
        break;
    case RIGHT:
        for (int i=1; i<length; i++) {
            body.push_back(make_pair(x-i, y));
        }
        break;
    }
    width = item_map_ptr->size();
    height = (*item_map_ptr)[0].size();
}

bool Snake::operator == (const Snake* other) {
    if (this->length != other->length) {
        return false;
    }
    for (int i=0; i<length; ++i) {
        if (this->body[i] != other->body[i]) {
            return false;
        }
    }
    return true;
}

int Snake::getLength() const
{
    return body.size();
}

int Snake::getHealth() const
{
    return health;
}

vector<Loc> &Snake::getBody()
{
    return body;
}

Direction Snake::getDirection()
{
    return direction;
}

Direction Snake::getBodyDirection(int i)
{
    if(i == 0) return getDirection();
    Loc loc2 = body[i];
    Loc loc1 = body[i-1];
    if(loc1.first - loc2.first == 0 && loc1.second - loc2.second == -1)
        return UP;
    if(loc1.first - loc2.first == 0 && loc1.second - loc2.second == 1)
        return DOWN;
    if(loc1.first - loc2.first == -1 && loc1.second - loc2.second == 0)
        return LEFT;
    if(loc1.first - loc2.first == 1 && loc1.second - loc2.second == 0)
        return RIGHT;
}

void Snake::changeDireciton(Direction new_direction)
{
    if(!finishmove) return;
    switch (direction)
    {
        case UP:
            if (new_direction == LEFT || new_direction == RIGHT)
                direction = new_direction;
            break;
        case DOWN:
            if (new_direction == LEFT || new_direction == RIGHT)
                direction = new_direction;
            break;
        case LEFT:
            if (new_direction == UP || new_direction == DOWN)
                direction = new_direction;
            break;
        case RIGHT:
            if (new_direction == UP || new_direction == DOWN)
                direction = new_direction;
            break;
    }
    finishmove = false;
}

Loc Snake::nextLoc()
{
    int x = body[0].first, y = body[0].second;
    switch(direction){
    case UP:
        return make_pair(x, y-1);
    case DOWN:
        return make_pair(x, y+1);
    case LEFT:
        return make_pair(x-1, y);
    case RIGHT:
        return make_pair(x+1, y);
    }
}

bool Snake::move()
{
    Loc new_head = nextLoc();
    body.insert(body.begin(), new_head);
    body.pop_back();
    finishmove = true;
    return true;
}

Item *Snake::hitItem()
{
    Loc head = body[0];
    if(!isWithin(head.first, 0, item_map_ptr->size()-1) || !isWithin(head.second, 0, (*item_map_ptr)[0].size()-1))
        return nullptr;
    return (*item_map_ptr)[body[0].first][body[0].second];
}

bool Snake::hitSelf()
{
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
        if (this == other) {
            continue;  // 不和自己比较
        }
        for (Loc other_body: other->body) {
            if (head == other_body) {
                return true;
            }
        }
    }
    return false;
}

Marsh* Snake::touchMarsh()
{
    for(int i=0; i<getLength(); ++i) {
        Item* it = (*item_map_ptr)[body[i].first][body[i].second];
        if(it != nullptr && it->getName() == MARSH) {
            Marsh* msh = new Marsh(it->getLoc());
            return msh;
        }
    }
    return nullptr;
}

Aerolite *Snake::touchAerolite()
{
    for (int i=0; i<getLength(); ++i) {
        Item* it = (*item_map_ptr)[body[i].first][body[i].second];
        if (it != nullptr && it->getName() == AEROLITE) {
            Aerolite* aerolite = new Aerolite(it->getLoc());
            return aerolite;
        }
    }
    return nullptr;
}

bool Snake::hitEdge()
{
    Loc head = body[0];
    return (!isWithin(head.first, 0, width-1) || !isWithin(head.second, 0, height-1));
}

bool Snake::isPartOfSnake(Loc loc)
{
    for(int i = 0; i < getLength(); i++)
    {
        Loc mloc = body[i];
        if(mloc == loc)
            return true;
    }
    return false;
}

void Snake::addLength(int adding)
{
    // second last X/Y 倒数第二段身体
    int sl_x = body[getLength()-2].first, sl_y = body[getLength()-2].second;
    // last X/Y 最后一段身体
    int l_x = body[getLength()-1].first, l_y = body[getLength()-1].second;

    // new - last = last - slast
    // new = last + delta
    int delta_x = l_x - sl_x, delta_y = l_y - sl_y;
    for (int i=1; i<=adding; ++i) {
        int newX = l_x + i*delta_x, newY = l_y + i*delta_y;
        if (!isWithin(newX, 0, item_map_ptr->size()-1) || !isWithin(newY, 0, (*item_map_ptr)[0].size()-1)) {
            // 新坐标不在地图边界里
            break;
        }
        if ((*item_map_ptr)[newX][newY] != nullptr) {
            // 新坐标上有物体
            break;
        }
        if (isPartOfSnake(make_pair(newX, newY))) {
            break;
        }
        body.push_back(make_pair(newX, newY));
    }
    length += adding;
}

void Snake::addHealth(int adding) {
    this->health = min(this->health + adding, this->max_health);
    if (health <= 0) {
        death();
    }
}

void Snake::initialize()
{
    health = 3;
    speed = -30;
    cycle_recorder = 1;
    direction = UP;
    magnetic = 0;
    revival = 0;
    body = Snake( rebornLocation,
                  min(max(2, 0.5 * length), 7),
                  health,
                  rebornDirection,
                  item_map_ptr ).body;   // 用指定位置初始化的一条新蛇的身体来更新当前蛇的复活状态至地图中央
}

bool Snake::death()
{
    if (revival > 0) {
        initialize();
        return false;
    } else {
        if (isAI())
        {
            vector<Loc> empty;
            body = empty;

        }
        // 返回接口 传递死亡的信息
        health = 0;
        return true;
    }
}

void Snake::incEaten()
{
    eaten++;
}

void Snake::incKilled()
{
    killed++;
}

int Snake::getEaten()
{
    return eaten;
}

int Snake::getKilled()
{
    return killed;
}

int Snake::score()
{
    // 加权分数, 杀一条蛇等于吃五个食物
    return eaten + 5 * killed;
}

int Snake::getHp()
{
    return health;
}

void Snake::addSpeed(int adding)
{
    this->speed += adding;
}

int Snake::getMp()
{
    return mp;
}

void Snake::incMp()
{
    if(mp < 240)
        mp++;
}

void Snake::decMp()
{
    mp -= 4;
    if(mp < 0)
        mp = 0;
}

bool Snake::ableMove()
{
    // ===== 全局时钟走过 (12 - speed) 个周期蛇才会进行动作
    if (cycle_recorder != (12 - speed)) {
        cycle_recorder += 1;
        return false;
    } else {
        cycle_recorder = 1;
        return true;
    }
}

void Snake::setMagnetic(int effective_time) {
    this->magnetic = effective_time;
}

bool Snake::ableMagnetic()
{
    return (this->magnetic > 0);
}

void Snake::setRevival(int effective_time) {
    this->revival = effective_time;
}

void Snake::recover()
{
    speed = 6;
}

bool isWithin(int target, int low, int high)
{
    return (target >= low && target <= high);
}
