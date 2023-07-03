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

/*
    警报
    在倒计时结束后出现 *nextItem
*/
class Warning: public Item
{
    Warning(int, Item*);    // 初始化倒计时长 和 终点物体

    virtual void action(Snake);
    int counting;
    Item* nextItem;
    string name = "warning";
};

/*
    障碍
    不同障碍物有不同伤害量
*/
class Obstacle: public Item
{
    Obstacle(int);  // 以伤害量初始化

    virtual void action(Snake);
    int injury;
    string name = "obstacle";
};

/*
    墙壁
    和地图边界相同 撞击死亡
*/
class Wall: public Item
{
    virtual void action(Snake);
    string name = "wall";
};

/*
    陨石
    砸中头部死亡, 砸中身体则从砸中处截断以后部分
*/
class Aerolite: public Item
{
    virtual void action(Snake);
    string name = "aerolite";
};

/*
    沼泽
    只要蛇任何一部分在沼泽上 都将被减速
*/
class Marsh: public Item
{
    Marsh(int);     // 以减速程度初始化

    virtual void action(Snake);
    int decelerate;
    string name = "marsh";
};


#endif //ITEM_H
