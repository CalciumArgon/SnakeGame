#ifndef SNAKE_H
#define SNAKE_H
#include "item.h"
#include <vector>

enum Direction {UP, DOWN, LEFT, RIGHT};
typedef std::vector<std::vector<Item*>> Grid;

bool isWithin(int target, int low, int high);

class Snake
{
public:
    Snake(std::vector<Loc> body, int length, int max_health, Direction direction, Grid* item_map_ptr);
    Snake(Loc head, int length, int max_health, Direction direction, Grid* item_map_ptr);
    int getLength();
    std::vector<Loc> &getBody();
    void changeDireciton(Direction new_direction);
    Loc nextLoc();
    bool move();
    Item* hitItem();
    bool hitSelf();
    bool hitEdge();
    void addLength(int adding);
    void initialize();
    bool death();
    void incEaten();
    void incKilled();
    int getEaten();
    int getKilled();
private:
    std::vector<Loc> body;
    int length;
    int max_health;
    Direction direction;
    Grid* item_map_ptr;
    int speed = 3;
    int health;
    int eaten = 0;
    int killed = 0;
    int cycle_recorder = 1;
    int magnetic = 0;
    int revival = 0;
};

#endif // SNAKE_H
