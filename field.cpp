#include "field.h"
#include <ctime>
#include <random>

using namespace std;

Field::Field(int w, int h)
{
    height = h;
    width = w;
    Grid tmpfld;
    vector<Item*> vec;
    for(int i = 0; i < h; i++)
        vec.push_back(nullptr);
    for(int i = 0; i < w; i++)
        tmpfld.push_back(vec);
    item_map = tmpfld;
}

ItemType Field::getItemName(size_t w, size_t h)
{
    if(item_map[w][h] == nullptr)
        return BASIC;
    else
        return item_map[w][h]->getName();
}

void Field::createItem(ItemType type, Loc location, int info)
{
    Item* item = nullptr;
    switch(type){
    case BASIC:
        item = nullptr;
        break;
    case FOOD:
        item = new Food(location, info);
        break;
    case WALL:
        item = new Wall(location);
        break;
    case MARSH:
        item = new Marsh(location);
    }
    item_map[location.first][location.second] = item;
}



Item *Field::getItem(size_t w, size_t h)
{
    return item_map[w][h];
}

int Field::getHeight()
{
    return height;
}

int Field::getWidth()
{
    return width;
}

Grid* Field::getMapPtr()
{
    return &item_map;
}

Loc Field::createRandomLoc()
{
    srand(time(NULL));
    int x = rand() % this->height;
    int y = rand() % this->width;
    return make_pair(x, y);
}

std::vector<Snake *> Field::getSnakes()
{
    return snakes;
}

void Field::addSnake(Snake *snake_ptr)
{
    snakes.push_back(snake_ptr);
}


