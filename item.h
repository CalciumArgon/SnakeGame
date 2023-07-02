#ifndef ITEM_H
#define ITEM_H

#include "field.h"

#include <string>
#include <map>
using namespace std;

map<string, Item> item;

/*
    物品类:
        检查击中了哪条蛇
*/
class Item
{
public:
    Item(int, int);

    Snake* hitHeadSnake(vector<Snake*>);  // 需要把 Field 里存蛇的数组传进去
    Snake* hitBodySnake(vector<Snake*>);

    virtual void action(Snake) = 0;
    string name = "basic";
    Loc location;
};

/*
    普通食物
    增加的长度默认为 1, 可通过 changeAddLength() 修改
*/
class Food: public Item
{
    Food(int);  // 以指定增加长度来初始化

    virtual void action(Snake);
    void changeAddLength(int);

    string name = "food";
    int add_length = 1;
};

/*
    吸铁石
    让 Snake实例在 effective_time 时间里能吸收头部四周九宫格的 Item
    effective_time 跟随全局时钟变化
*/
class Magnet: public Item
{
    Magnet(int);    // 以指定有效时长来初始化

    virtual void action(Snake);
    string name = "magnet";
    int effective_time;
};

/*
    护盾
    让 Snake实例在 effective_time 时间里具有护盾, 保护一次死亡
    effective_time 跟随全局时钟变化
    (护盾的保护机制在 Snake类中实现)
*/
class Shield: public Item
{
    Shield(int);    // 以指定有效时长来初始化

    virtual void action(Snake);
    string name = "shield";
    int effective_time;
};

/*
    血包
    增加的血量默认为 1, 最多达到最大血量
*/
class Firstaid: public Item
{
    virtual void action(Snake);
    void changeAddHealth(int);

    string name = "firstaid";
    int add_health = 1;
};

class Warning: public Item
{
    virtual void action(Snake);
    string name = "warning";
};

class Obstacle: public Item
{
    virtual void action(Snake);
    string name = "obstacle";
};

class Wall: public Item
{
    virtual void action(Snake);
    string name = "wall";
};

class Aerolite: public Item
{
    virtual void action(Snake);
    string name = "aerolite";
};

class Marsh: public Item
{
    virtual void action(Snake);
    string name = "marsh";
};


#endif //ITEM_H