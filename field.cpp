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
        break;
    case MAGNET:
        item = new Magnet(location, info);
        break;
    }
    item_map[location.first][location.second] = item;
}



Item *Field::getItem(size_t w, size_t h)
{
    if (w < 0 || w >= this->getWidth() || h < 0 || h >= this->getHeight()) {
        return nullptr;
    }
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

void Field::clearSnake()
{
    for (auto snake_ptr=snakes.begin(); snake_ptr!=snakes.end(); ) {
        if ((*snake_ptr)->getHealth() <= 0) {
            snakes.erase(snake_ptr);
        } else {
            snake_ptr++;
        }
    }
}

Loc Field::createRandomLoc()
{
    srand(time(NULL));
    int x, y;
    do {
        x = rand() % this->height;
        y = rand() % this->width;
    } while (existFood(make_pair(x, y)));
    return make_pair(x, y);
}

void Field::deleteItem(Loc location)
{
    this->item_map[location.first][location.second] = nullptr;
}

bool Field::existFood(Loc location)
{
    return (this->item_map[location.first][location.second] != nullptr);
}

std::vector<Snake *> Field::getSnakes()
{
    return snakes;
}

void Field::addSnake(Snake *snake_ptr)
{
    snakes.push_back(snake_ptr);
}


