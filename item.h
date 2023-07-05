#ifndef ITEM_H
#define ITEM_H

#include <utility>
#include <vector>

typedef std::pair<int, int> Loc;

enum ItemType {BASIC=0, FOOD, MAGNET, SHIELD, FIRSTAID, OBSTACLE, WALL, AEROLITE, MARSH};

class Snake;

class Item
{
public:
    Item(Loc location);
    //virtual ~Item() = 0;
    Loc getLoc();
    Snake* hitHeadSnake(std::vector<Snake*>);
    Snake* hitBodySnake(std::vector<Snake*>);
    virtual void action(Snake*) = 0;
    virtual ItemType getName();
private:
    ItemType name = BASIC;
    Loc location;
};

class Food: public Item
{
public:
    Food(Loc location, int add_length);
    virtual void action(Snake*);
    virtual ItemType getName();
    void changeAddLength(int new_len);
private:
    ItemType name = FOOD;
    int add_length = 1;
};

class Wall: public Item
{
public:
    Wall(Loc location);
    virtual void action(Snake*);
    virtual ItemType getName();
private:
    ItemType name = WALL;
    Loc location;
};


#endif // ITEM_H
