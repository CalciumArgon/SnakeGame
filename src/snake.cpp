#include "snake.h"

using namespace std;

inline int min(int x1, int x2) {
    return (x1 <= x2) ? x1 : x2;
}

inline int max(int x1, int x2) {
    return (x1 >= x2) ? x1 : x2;
}

Snake::Snake(vector<Loc> body, size_t length, int health, Direction direction, Grid* item_map_ptr) :
    body(body),
    length(length),
    direction(direction),
    rebornLocation(body[0]),
    rebornDirection(direction),
    health(health),
    item_map_ptr(item_map_ptr)
{
    this->width = this->item_map_ptr->size();
    this->height = (*this->item_map_ptr)[0].size();
}

Snake::Snake(Loc head, size_t length, int health, Direction direction, Grid* item_map_ptr) :
    length(length),
    direction(direction),
    rebornLocation(head),
    rebornDirection(direction),
    health(health),
    item_map_ptr(item_map_ptr)
{
    this->body.clear();
    this->body.push_back(head);
    int x = head.first, y = head.second;
    switch (direction)
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
    this->width = item_map_ptr->size();
    this->height = (*item_map_ptr)[0].size();
}

Snake::~Snake() {
    delete this->item_map_ptr;
}

bool Snake::operator == (const Snake* other) {
    if (this->length != other->length) {
        return false;
    }
    for (size_t i=0; i<length; ++i) {
        if (this->body[i] != other->body[i]) {
            return false;
        }
    }
    return true;
}

size_t Snake::getLength() const {
    return this->body.size();
}

int Snake::getHealth() const {
    return this->health;
}

vector<Loc> &Snake::getBody() {
    return this->body;
}

Direction Snake::getDirection() {
    return this->direction;
}

Direction Snake::getBodyDirection(size_t i) {
    if(i == 0) return getDirection();
    Loc loc2 = this->body[i];
    Loc loc1 = this->body[i-1];
    if(loc1.first - loc2.first == 0 && loc1.second - loc2.second == -1)
        return UP;
    if(loc1.first - loc2.first == 0 && loc1.second - loc2.second == 1)
        return DOWN;
    if(loc1.first - loc2.first == -1 && loc1.second - loc2.second == 0)
        return LEFT;
    if(loc1.first - loc2.first == 1 && loc1.second - loc2.second == 0)
        return RIGHT;
}

void Snake::changeDireciton(Direction new_direction) {
    if (!this->finishmove) return;
    switch (this->direction)
    {
        case UP:
            if (new_direction == LEFT || new_direction == RIGHT)
                this->direction = new_direction;
            break;
        case DOWN:
            if (new_direction == LEFT || new_direction == RIGHT)
                this->direction = new_direction;
            break;
        case LEFT:
            if (new_direction == UP || new_direction == DOWN)
                this->direction = new_direction;
            break;
        case RIGHT:
            if (new_direction == UP || new_direction == DOWN)
                this->direction = new_direction;
            break;
    }
    this->finishmove = false;
}

Loc Snake::nextLoc() {
    int x = this->body[0].first, y = this->body[0].second;
    switch (this->direction) {
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

bool Snake::move() {
    Loc new_head = nextLoc();
    this->body.insert(this->body.begin(), new_head);
    this->body.pop_back();
    this->finishmove = true;
    return true;
}

void Snake::setLevel(int l) {
    this->level = l;
}

Item *Snake::hitItem() {
    Loc head = this->body[0];
    if ( !isWithin(head.first, 0, this->item_map_ptr->size()-1) ||
         !isWithin(head.second, 0, (*item_map_ptr)[0].size()-1) )
        return nullptr;
    return (*this->item_map_ptr)[this->body[0].first][this->body[0].second];
}

bool Snake::hitSelf() {
    Loc head = body[0];
    for (int i=1; i<this->getLength(); ++i) {
        if (head == this->body[i]) {
            return true;
        }
    }
    return false;
}

bool Snake::hitOtherSnake(vector<Snake*> snakes) {
    Loc head = this->body[0];
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

Marsh* Snake::touchMarsh() {
    for(int i=0; i<this->getLength(); ++i) {
        Item* it = (*this->item_map_ptr)[this->body[i].first][this->body[i].second];
        if(it != nullptr && it->getName() == MARSH) {
            Marsh* msh = new Marsh(it->getLoc());
            return msh;
        }
    }
    return nullptr;
}

Aerolite *Snake::touchAerolite() {
    for (int i=0; i<this->getLength(); ++i) {
        Item* it = (*this->item_map_ptr)[this->body[i].first][this->body[i].second];
        if (it != nullptr && it->getName() == AEROLITE) {
            Aerolite* aerolite = new Aerolite(it->getLoc());
            return aerolite;
        }
    }
    return nullptr;
}

bool Snake::hitEdge() {
    Loc head = this->this->body[0];
    return ( !isWithin(head.first, 0, width-1) ||
             !isWithin(head.second, 0, height-1) );
}

bool Snake::isPartOfSnake(Loc loc) {
    for (int i=0; i<this->getLength(); ++i) {
        Loc mloc = body[i];
        if(mloc == loc)
            return true;
    }
    return false;
}

void Snake::addLength(int adding) {
    if (adding == 0) {
        return;
    } else if (adding > 0) {
        // second last X/Y 倒数第二段身体
        int sl_x = this->body[getLength()-2].first, sl_y = this->body[getLength()-2].second;
        // last X/Y 最后一段身体
        int l_x = this->body[getLength()-1].first, l_y = this->body[getLength()-1].second;

        // new - last = last - slast
        // new = last + delta
        int delta_x = l_x - sl_x, delta_y = l_y - sl_y;
        for (int i=1; i<=adding; ++i) {
            int newX = l_x + i*delta_x, newY = l_y + i*delta_y;
            if ( !isWithin(newX, 0, this->item_map_ptr->size()-1) ||
                 !isWithin(newY, 0, (*this->item_map_ptr)[0].size()-1)) {
                // 新坐标不在地图边界里
                break;
            }
            if ((*this->item_map_ptr)[newX][newY] != nullptr) {
                // 新坐标上有物体
                break;
            }
            if (this->isPartOfSnake(make_pair(newX, newY))) {
                break;
            }
            body.push_back(make_pair(newX, newY));
        }
    } else if (adding < 0) {
        while (adding--) {
            this->body.pop_back();
        }
    }
    this->length += adding;
}

void Snake::addHealth(int adding) {
    this->health = min(this->health + adding, this->max_health);
    if (this->health <= 0) {
        death();
    }
}

void Snake::initialize() {
    this->health = 3;
    this->speed = -30;
    this->cycle_recorder = 1;
    this->direction = this->rebornDirection;
    this->magnetic = 0;
    this->revival = 0;
    this->body = Snake( rebornLocation,
                  min(max(2, 0.5 * length), 7),
                  health,
                  rebornDirection,
                  item_map_ptr ).body;   // 用指定位置初始化的一条新蛇的身体来更新当前蛇的复活状态至地图中央
}

bool Snake::death() {
    if (this->revival > 0) {
        this->initialize();
        return false;
    } else {
        if (this->isAI()) {
            vector<Loc> empty;
            this->body = empty;
        }
        // 返回接口 传递死亡的信息
        this->health = 0;
        return true;
    }
}

void Snake::incEaten() {
    this->eaten += 1;
}

void Snake::incKilled() {
    this->killed += 1;
}

int Snake::getEaten() {
    return eaten;
}

int Snake::getKilled() {
    return killed;
}

int Snake::score() {
    // 加权分数, 杀一条蛇等于吃五个食物
    return this->eaten + 5 * this->killed;
}

int Snake::getHp() {
    return this->health;
}

void Snake::addSpeed(int adding) {
    this->speed += adding;
}

int Snake::getMp() {
    return mp;
}

void Snake::incMp() {
    if (this->mp < 240)
        this->mp += 1;
}

void Snake::decMp() {
    this->mp -= 4;
    if (this->mp < 0)
        this->mp = 0;
}

bool Snake::ableMove() {
    // ===== 全局时钟走过 (12 - speed) 个周期蛇才会进行动作
    if (this->cycle_recorder != (12 - this->speed)) {
        this->cycle_recorder += 1;
        return false;
    } else {
        this->cycle_recorder = 1;
        return true;
    }
}

int Snake::getMagnetic() {
    return this->magnetic;
}

int Snake::getRevival() {
    return this->revival;
}

void Snake::decMagnectic() {
    this->magnetic -= 1;
}

void Snake::setMagnetic(int effective_time) {
    this->magnetic = effective_time;
}

bool Snake::ableMagnetic() {
    return (this->magnetic > 0);
}

void Snake::setRevival(int effective_time) {
    this->revival = effective_time;
}

void Snake::recover() {
    this->speed = 3 * (this->level - 1);
}

Direction Snake::act(Field *state) {
    if (!this->isAI() || !state) {
        assert(false);
    }
    return this->direction;
}

bool isWithin(int target, int low, int high) {
    return (target >= low && target <= high);
}
