/*
    该文件已放弃
    不再使用各个模式类虚函数来判断 reachTarget()
    而是直接在 Game 里用 if-else 手动判断
*/

#include <string>
using namespace std;

class Game {};
class GameMode
{
public:
    GameMode(string);

    // 在里面分 game_type 的情况判断不同目标是否达成
    // 所需要的信息都在 Game.field.snakes[0] 这个 Snake实例里
    bool reachTarget(Game*);           
    string game_type;
};