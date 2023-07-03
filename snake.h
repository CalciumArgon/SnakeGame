#ifndef SNAKE_H
#define SNAKE_H

#include "item.h"
#include <vector>
using namespace std;

typedef pair<int, int> Loc;

class Snake
{
public:
    Snake(vector<Loc>, int length);

    void changeDirection(string);
    void move();    // 前进, 检查碰到的物体并 Item.action(this) , 更新身体坐标以及其他参数
    Item* hitItem();

    bool hitSelf();
    bool hitOtherSnake(vector<Snake>);  // 在 Game 中可以把 field.snakes[1:] 传进来

    void death();   // 死亡程序, 在 revival > 0 的情况下重新在地图中心初始化, 否则返回死亡


    int length;
    vector<Loc> body;   // body[0] 是头部
    int health;
    int eaten = 0;  // 吃过的食物 与长度无直接关系

    /*
        加速时: speed 调成 5
        减速时: speed 调成 1
    */
    int speed = 3;
    int cycle_recorder = 1; //  cycle_recorder 每增加 (6 - speed) 该蛇进行一轮操作
    int direction;  // "up" "down" "left" "right"

    // 特殊能力倒计时, 随全局时钟变化
    int magnetic = 0;   
    int revival = 0;
};

#endif //SNAKE_H