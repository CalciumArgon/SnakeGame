#include "field.h"
using namespace std;

// 检查是否在闭区间内
bool isWithin(int target, int low, int high) {
    return (target >= low && target <= high);
}

Field::Field(int height, int width): height(height), width(width) {}
Field::~Field() {
    // for (int i=0; i<this->height; ++i) {
    //     for (int j=0; j<this->width; ++j) {
    //         delete this->field[i][j];
    //     }
    // }
    return;
}

// 检查坐标点是否在界内
bool Field::inBound(Loc location) {
    return (isWithin(location.first, 0, height - 1) && isWithin(location.second, 0, width));
}

/*
TODO: 可能要把 int type 改成对所有物品的枚举类型
在 (x, y) 处产生一个物品
    0: 食物
    1: 增益
    2: 削弱
*/
void Field::createItem(int type, Loc location) {
    if (type == 0) {
        return;
    } else if (type == 1 ) {
        return;
    } else if (type == 2) {
        return;
    }
    cout << "Wrong";
}

Loc Field::createRandomLoc() {
    srand(time(NULL));
    int x = rand() % this->height;
    int y = rand() % this->width;
    return make_pair(x, y);
}