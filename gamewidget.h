#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include <QWidget>
#include "field.h"
#include "snake.h"
#include "game.h"
#include <QPainter>
#include <QKeyEvent>
#include <QLabel>
#include <ctime>

namespace Ui {
class GameWidget;
}

class GameWidget : public QWidget
{
    Q_OBJECT

public:
    GameWidget(Game *game, int level, QWidget *parent = nullptr);
    void initialize();

    //游戏界面绘图控制
    QRect getRect(int, int);
    void paintEvent(QPaintEvent *ev);

    //键盘事件控制
    void keyPressEvent(QKeyEvent* event);
    void keyReleaseEvent(QKeyEvent* event);

    //导引部分
    void enterGuide();
    void readFile(int);
    void startGuide();
    void turnPage();
    void endGuide();

    //绘制游戏界面左侧属性栏
    void showTime(int);
    void showHp();
    void showMp();
    void showScore();
    void showShield();
    void showMagnetic();

    //物品与蛇的绘制
    void paintItem(int, int, ItemType);
    void paintSnake(int, int);
    void deleteFoodLabel();
    void deleteMagnetLabel();
    void deleteObstacleLabel();
    void deleteFirstaidLabel();
    void deleteShieldLabel();

    ~GameWidget();

private:
    Ui::GameWidget *ui;
    Game *game;
    //格子的宽度
    int unitlen = 30;
    int cnt_time = 0;
    short game_end = 0;
    //是否已发送游戏结束的弹窗，防止多次发送
    bool is_emit = false;
    //倒计时，等于4表示正在游戏引导，等于0到3表示正在倒计时
    int countdown = 4;
    //计时变量，用于打印游戏时间
    clock_t begin = 0;
    clock_t end = 0;

    std::vector<QLabel*> static_label;
    std::vector<QLabel*> food_label;
    std::vector<QLabel*> magnet_label;
    std::vector<QLabel*> obstacle_label;
    std::vector<QLabel*> firstaid_label;
    std::vector<QLabel*> shield_label;
    std::vector<QLabel*> aerolite_label;
    std::vector<QLabel*> warning_label;
    //储存引导界面每一页的字符串
    std::vector<QString> guide_line;
    //蛇打印相关变量
    std::vector<std::vector<QLabel*>> snake_label;
    std::vector<int> snake_length;
    std::vector<std::vector<Direction>> snake_direction;

    //左侧属性栏的宽度
    int border = 300;
    //当前关卡
    int level = 0;

    bool on_guide = false;
    bool open = false;
    bool game_start = false;
    int page_num = 1;
    int loop_counter = 0;
signals:
    void gameEnd(int, int, int);
public slots:
    void on_btnNext_clicked();
    void on_btnSkip_clicked();
};

#endif // GAMEWIDGET_H
