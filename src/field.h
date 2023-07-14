#ifndef FIELD_H
#define FIELD_H

#include <vector>
#include "item.h"
#include "snake.h"

typedef std::vector<std::vector<Item*>> Grid;


class Field
{
public:
    Field(size_t, size_t);
    ItemType getItemName(size_t w, size_t h);

    bool inBound(Loc);
    //在指定地点创建物品
    void createItem(ItemType type, Loc location, int info);

    Loc createRandomLoc();
    void deleteItem(Loc location);
    bool existFood(Loc location);

    Item* getItem(size_t w, size_t h);
    size_t getHeight();
    size_t getWidth();

    Grid* getMapPtr();

    void clearSnake();

    std::vector<Snake*> getSnakes();
    void addSnake(Snake* snake_ptr);
private:
    size_t height, width;
    Grid item_map;
    std::vector<Snake*> snakes;
};

#endif // FIELD_H
