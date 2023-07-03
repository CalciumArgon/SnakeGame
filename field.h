#ifndef FIELD_H
#define FIELD_H

#include "snake.h"

#include <vector>
#include <utility>
#include <random>
#include <ctime>
#include <iostream>
using namespace std;

bool isWithin(int, int, int);
class Item;

// 二维数组, 即地图的坐标索引, 内部是指向 Item实例的指针
typedef vector<vector<Item* >> Grid;

class Field
{
public:
    Field(int, int);
    ~Field();

    bool inBound(Loc);

    void loadMap();     // 某种 文件读取并存储将要被加载的物体 的方式

    void createItem(int type, Loc);
    Loc createRandomLoc();

    int height, width;
    Grid itemMap;
    vector<Snake*> snakes;
};

#endif //FIELD_H
