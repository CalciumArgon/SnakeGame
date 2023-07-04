#include "field.h"
using namespace std;

// 检查是否在闭区间内
bool isWithin(int target, int low, int high) {
    return (target >= low && target <= high);
}

Field::Field(int height, int width): height(height), width(width) {}
Field::~Field() {
    for (int i=0; i<this->height; ++i) {
        for (int j=0; j<this->width; ++j) {
            delete this->itemMap[i][j];
        }
    }
    itemMap.clear();
    return;
}

// 检查坐标点是否在界内
bool Field::inBound(Loc location) {
    return (isWithin(location.first, 0, height - 1) && isWithin(location.second, 0, width));
}

// itemMap 中记录物体, 注意 Item 实例中已经记录了自己的坐标
void Field::setItem(Item* item) {
    itemMap[item->location.first][item->location.second] = item;
}

Item* Field::createItem(ItemType type, Loc location, int info) {
    Item* item;
    if (type == ItemType::FOOD) {
        item = new Food(location, info);
    } else if (type == ItemType::MAGNET) {
        item = new Magnet(location, info);
    } else if (type == ItemType::SHIELD) {
        item = new Shield(location, info);
    } else if (type == ItemType::FIRSTAID) {
        item = new Firstaid(location, info);
    } else if (type == ItemType::OBSTACLE) {
        item = new Obstacle(location, info);
    } else if (type == ItemType::WALL) {
        item = new Wall(location);
    } else if (type == ItemType::AEROLITE) {
        item = new Aerolite(location);
    } else if (type == ItemType::MARSH) {
        item = new Marsh(location, info);
    }
    return item;
}


Loc Field::createRandomLoc() {
    srand(time(NULL));
    int x = rand() % this->height;
    int y = rand() % this->width;
    return make_pair(x, y);
}
