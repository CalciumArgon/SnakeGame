//
// Created by lenovo on 2023/7/3.
//

#ifndef SNAKE_SRC_FULL_AISNAKE_H
#define SNAKE_SRC_FULL_AISNAKE_H
#include "snake.h"
#include "field.h"


class AISnake:public Snake {
public:
    AISnake(std::vector<Loc> body, int length, int max_health, Direction direction, Grid* item_map_ptr):Snake(body, length, max_health, direction, item_map_ptr){};
    AISnake(Loc head, int length, int max_health, Direction direction, Grid* item_map_ptr):Snake(head, length, max_health, direction, item_map_ptr){};
    virtual Direction act(Field& state) = 0;

};


class GreedyFood:public AISnake{
private:
    std::pair<int, int> getNearestFood();
public:
    GreedyFood(std::vector<Loc> body, int length, int max_health, Direction direction, Grid* item_map_ptr):AISnake(body, length, max_health, direction, item_map_ptr){};
    GreedyFood(Loc head, int length, int max_health, Direction direction, Grid* item_map_ptr):AISnake(head, length, max_health, direction, item_map_ptr){};
    virtual Direction act(Field& state);
};

#endif //SNAKE_SRC_FULL_AISNAKE_H


class Hardhitted:public AISnake{

};

