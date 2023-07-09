#ifndef FIELD_H
#define FIELD_H

#include <vector>
#include "item.h"
#include "snake.h"

typedef std::vector<std::vector<Item*>> Grid;


class Field
{
public:
    Field(int, int);
    ItemType getItemName(size_t w, size_t h);

    void createItem(ItemType type, Loc location, int info);
    Loc createRandomLoc();
    void deleteItem(Loc location);
    bool existFood(Loc location);

    Item* getItem(size_t w, size_t h);
    int getHeight();
    int getWidth();
    Grid* getMapPtr();

    void clearSnake();

    std::vector<Snake*> getSnakes();
    void addSnake(Snake* snake_ptr);
private:
    int height, width;
    Grid item_map;
    std::vector<Snake*> snakes;

};

#endif // FIELD_H
