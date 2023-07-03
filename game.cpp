#include "game.h"

void Game::snakeAction(Snake* snake) {
    /*
        1. 接收键盘按键 (方向和 Ctrl)
        2. 更改方向或加速效果
        3. 向所选方向运行
        4. 判断碰撞蛇--是否需要调用 death()
        5. 判断遇到物体--调用物体的 action()
        6. 判断是否达成游戏目标
        7. 读取地图, 生成计划好的物体 (无论是否 hitItem() 按全局时钟生成)
        8. 一些特殊 Item 的自己的效果
        9. 暂停时间, 全局时钟运行一轮   <-- 不用每条蛇 Action 中运行, 在 Game 域中整体运行一次
        10. 返回一些其他需要反馈的信息 ....
    */
    /*****
        注意区分 snakeAction() 和 runGame() 的功能区别
    *****/
    return;
}