#ifndef SNAKE_H
#define SNAKE_H

#include "item.h"
#include <vector>
using namespace std;

typedef pair<int, int> Loc;
enum Direction {up=0, down, left, right};

class Snake
{
public:
    Snake(vector<Loc>, int length, int MAX_health, Direction direction, Grid* item_map);
    // 给定头部坐标, 按长度默认初始化一条朝向指定方向的蛇
    Snake(Loc head, int length, int MAX_health, Direction direction, Grid* item_map);

    void initialize();

    void changeDirection(Direction);
    Loc nextLoc();
    bool move();    // 前进, 检查碰到的物体并 Item.action(this) , 更新身体坐标以及其他参数
    Item* hitItem();

    bool hitSelf();
    bool hitOtherSnake(vector<Snake*>);  // 在 Game 中可以把 field.snakes[1:] 传进来

    void addLength(int);   // 在当前尾部添加给定个长度, 并更新 length
    void decreaseHealth(int);
    void death();   // 死亡程序, 在 revival > 0 的情况下重新在地图中心初始化, 否则返回死亡

    int MAX_health;
    Grid* item_map;

    int length;
    vector<Loc> body;   // body[0] 是头部
    int health;

    int eaten = 0;  // 吃过的食物 与长度无直接关系
    int killed = 0; // 杀死的蛇

    /*
        加速时: speed 调成 5
        减速时: speed 调成 1
    */
    int speed = 3;
    int cycle_recorder = 1; //  cycle_recorder 每增加 (6 - speed) 该蛇进行一轮操作
    Direction direction;  // "up" "down" "left" "right"

    // 特殊能力倒计时, 随全局时钟变化
    int magnetic = 0;   
    int revival = 0;
};

#endif //SNAKE_H
