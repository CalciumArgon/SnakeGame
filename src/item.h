#ifndef ITEM_H
#define ITEM_H

#include <utility>
#include <vector>

typedef std::pair<size_t, size_t> Loc;

enum ItemType {BASIC=0, FOOD, WALL, AEROLITE, MARSH, MAGNET, SHIELD, FIRSTAID, OBSTACLE,};

class Snake;


/*
    物品类:
    检查击中了哪条蛇
    返回具体子类的物体类别 ItemType
    子类各自重写的效果函数 action()
*/
class Item
{
public:
    Item(Loc location);
    Loc getLoc();
    Snake* hitHeadSnake(std::vector<Snake*>);
    Snake* hitBodySnake(std::vector<Snake*>);
    virtual void action(Snake*) = 0;
    virtual ItemType getName();
    bool is_print = false;
protected:
    Loc location;
};

/*
    普通食物
    增加的长度默认为 1, 可通过 changeAddLength() 修改
*/
class Food: public Item
{
public:
    Food(Loc location, int add_length);
    virtual void action(Snake*);
    virtual ItemType getName();
    void changeAddLength(int new_len);
protected:
    int add_length = 1;
};

/*
    吸铁石
    让 Snake实例在 effective_time 时间里能吸收头部四周九宫格的 Item
    effective_time 跟随全局时钟变化
*/
class Magnet: public Item
{
public:
    // 以指定有效时长来初始化
    Magnet(Loc, int);
    virtual void action(Snake*);
    virtual ItemType getName();

protected:
    int effective_time;
};

/*
    护盾
    让 Snake实例具有护盾, 保护一次死亡
    (护盾的保护机制在 Snake类中实现)
*/
class Shield: public Item
{
public:
    Shield(Loc, int);
    virtual void action(Snake*);
    virtual ItemType getName();

protected:
    int effective_time;
};

/*
    血包
    增加的血量默认为 1, 最多达到最大血量
*/
class Firstaid: public Item
{
public:
    // 以能增加的血量值初始化
    Firstaid(Loc, int);

    virtual void action(Snake*);
    virtual ItemType getName();
    void changeAddHealth(int);

protected:
    int add_health = 1;
};

/*
    障碍
    不同障碍物有不同伤害量
*/
class Obstacle: public Item
{
public:
    // 以伤害量初始化
    Obstacle(Loc, int);
    virtual void action(Snake*);
    virtual ItemType getName();

protected:
    int injury;
};

/*
    墙壁
    和地图边界相同 撞击死亡
*/
class Wall: public Item
{
public:
    Wall(Loc location);
    virtual void action(Snake*);
    virtual ItemType getName();
protected:
    ItemType name = WALL;
};


/*
    陨石
    砸中头部死亡, 砸中身体则从砸中处截断以后部分
*/
class Aerolite: public Item
{
public:
    Aerolite(Loc);
    virtual void action(Snake*);
    virtual ItemType getName();
};


/*
    沼泽
    只要蛇任何一部分在沼泽上 都将被减速
*/
class Marsh: public Item
{
public:
    // 以减速程度初始化
    Marsh(Loc);

    virtual void action(Snake*);
    virtual ItemType getName();
protected:
    int decelerate = 5;
};


#endif // ITEM_H
