#ifndef SNAKE_H
#define SNAKE_H
#include "item.h"
#include <vector>
#include <assert.h>

class Field;
enum Direction {UP, DOWN, LEFT, RIGHT};
typedef std::vector<std::vector<Item*>> Grid;

bool isWithin(int target, int low, int high);

class Snake
{
public:
    Snake(std::vector<Loc> body, size_t length, int health, Direction direction, Grid* item_map_ptr);
    Snake(Loc head, size_t length, int health, Direction direction, Grid* item_map_ptr);
    virtual ~Snake();

    void initialize();
    bool operator == (const Snake* other);

    size_t getLength() const;
    int getHealth() const;
    std::vector<Loc> &getBody();
    Direction getDirection();
    Direction getBodyDirection(size_t);

    void changeDireciton(Direction new_direction);
    // 蛇向 direction 方向移动
    Loc nextLoc();
    bool move();
    void setLevel(int);

    // 检查碰撞
    Item* hitItem();    // 返回蛇碰到的物体种类
    bool hitSelf();
    bool hitEdge();
    bool hitOtherSnake(std::vector<Snake*>);
    // 判断蛇是否经过沼泽或与陨石发生碰撞
    Marsh* touchMarsh();
    Aerolite* touchAerolite();

    bool isPartOfSnake(Loc loc);

    void addLength(int adding);
    void addHealth(int adding);
    void incEaten();
    void incKilled();
    int getEaten();
    int getKilled();

    //当前分数
    int score();

    // 设置特殊能力
    void setMagnetic(int);
    bool ableMagnetic();
    void setRevival(int);

    int getHp();
    void addSpeed(int adding);
    int getMp();
    void incMp();
    void decMp();
    bool ableMove();

    int getMagnetic();
    int getRevival();
    void decMagnectic();

    bool death();

    //蛇在每次判断加速或减速前先返回到初始速度
    void recover();
    virtual bool isAI() {return false;}
    bool speed_buff = false;
    virtual Direction act(Field* state);

    //当前难度
    int level = 1;

protected:
    // 身体
    std::vector<Loc> body;
    size_t length;
    Direction direction;
    Loc rebornLocation;         //蛇重生和初始化的位置
    Direction rebornDirection;  //蛇重生和初始化的方向

    int max_health = 4;
    int health;

    //包含所有物品的地图的指针
    Grid* item_map_ptr;
    size_t width;
    size_t height;

    int eaten = 0;  //蛇吃过的食物数量
    int killed = 0; //蛇杀死的其他蛇的数量

    //控制蛇移动的时钟循环, 决定速度
    int cycle_recorder = 1;
    int mp = 240;
    int speed = -30;
    //蛇在改变方向后是否已完成移动，用于防止一次移动改变了两次方向
    bool finishmove = true;

    // 特殊能力
    int magnetic = 0;
    int revival = 0;
};

#endif // SNAKE_H
