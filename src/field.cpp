#include "field.h"
#include <ctime>
#include <random>

using namespace std;

Field::Field(size_t w, size_t h)
{
    height = h;
    width = w;
    Grid tmpfld;
    vector<Item*> vec;
    for(size_t i = 0; i < h; i++)
        vec.push_back(nullptr);
    for(size_t i = 0; i < w; i++)
        tmpfld.push_back(vec);
    item_map = tmpfld;
}

ItemType Field::getItemName(size_t w, size_t h)
{
    if (!inBound(make_pair(w, h)) || item_map[w][h] == nullptr) {
        return BASIC;
    } else {
        return item_map[w][h]->getName();
    }
}

bool Field::inBound(Loc location)
{
    if (location.first >= this->width || location.second >= this->height) {
        return false;
    }
    return true;
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
    case OBSTACLE:
        item = new Obstacle(location, info);
        break;
    case FIRSTAID:
        item = new Firstaid(location, info);
        break;
    case SHIELD:
        item = new Shield(location, info);
        break;
    case AEROLITE:
        item = new Aerolite(location);
    }
    item_map[location.first][location.second] = item;
}



Item *Field::getItem(size_t w, size_t h)
{
    if (w >= this->getWidth() || h >= this->getHeight()) {
        return nullptr;
    }
    return item_map[w][h];
}

size_t Field::getHeight()
{
    return height;
}

size_t Field::getWidth()
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
    srand(static_cast<unsigned int>(time(nullptr)));
    size_t x, y;
    do {
        x = size_t(rand()) % this->height;
        y = size_t(rand()) % this->width;
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


