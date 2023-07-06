//
// Created by lenovo on 2023/7/3.
//

#ifndef SNAKE_SRC_FULL_AISNAKE_H
#define SNAKE_SRC_FULL_AISNAKE_H
#include "snake.h"
#include "field.h"


class AISnake:public Snake {
public:
    virtual Direction act(Field& state) = 0;

};


class GreedyFood:public AISnake{
private:
    std::pair<int, int> getNearestFood();
public:
    virtual Direction act(Field& state);
};

#endif //SNAKE_SRC_FULL_AISNAKE_H


class Hardhitted:public AISnake{

};

