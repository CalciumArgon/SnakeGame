#ifndef SNAKE_SRC_FULL_AISNAKE_H
#define SNAKE_SRC_FULL_AISNAKE_H
#include "snake.h"
#include "field.h"
#include <queue>

typedef std::pair<size_t, size_t> Loc;
class AISnake: public Snake {
public:
    Direction go_to(Loc target);
    bool reachtarget(Loc target);
    AISnake(std::vector<Loc> body, int length, int max_health, Direction direction, Grid* item_map_ptr):Snake(body, length, max_health, direction, item_map_ptr){};
    AISnake(Loc head, int length, int max_health, Direction direction, Grid* item_map_ptr):Snake(head, length, max_health, direction, item_map_ptr){};
    virtual Direction act(Field* state) = 0;
    virtual bool isAI() {return true;}

protected:
    virtual ~AISnake();
    Loc next_target = {-1, -1};
};


class GreedyFood:public AISnake{
private:
    std::pair<int, int> getNearestFood();
public:
    GreedyFood(std::vector<Loc> body, int length, int max_health, Direction direction, Grid* item_map_ptr):AISnake(body, length, max_health, direction, item_map_ptr){};
    GreedyFood(Loc head, int length, int max_health, Direction direction, Grid* item_map_ptr):AISnake(head, length, max_health, direction, item_map_ptr){};
    virtual Direction act(Field* state);
};


class WalkingSnake: public AISnake{
private:
    std::queue<Loc> path;
public:
    WalkingSnake(std::queue<Loc> path, std::vector<Loc> body, int length, int max_health, Direction direction, Grid* item_map_ptr):
    AISnake(body, length, max_health, direction, item_map_ptr) { this->path = path;}
    WalkingSnake(std::queue<Loc> path, Loc head, int length, int max_health, Direction direction, Grid* item_map_ptr):
    AISnake(head, length, max_health, direction, item_map_ptr) {this->path = path;}
    virtual Direction act(Field* state);
    void setPath(std::queue<Loc> path);
};
#endif //SNAKE_SRC_FULL_AISNAKE_H
