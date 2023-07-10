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
    GameWidget(Game *game, QWidget *parent = nullptr);
    QRect getRect(int, int);
    void paintEvent(QPaintEvent *ev);
    void keyPressEvent(QKeyEvent* event);
    void keyReleaseEvent(QKeyEvent* event);
    void showTime(int);
    void showHp();
    void showMp();
    void paintItem(int, int, ItemType);
    void paintMySnake();
    void deleteLabel();
    void deleteFoodLabel();
    std::vector<QLabel*> static_label;
    std::vector<QLabel*> dynamic_label;
    std::vector<QLabel*> food_label;

    ~GameWidget();
    int border = 300;
private:
    Ui::GameWidget *ui;
    Game *game;
    int unitlen = 30;
    int cnt_time = 0;
    short int game_over = 0;
    bool is_emit = false;
    int countdown = 3;
    clock_t begin = 0;
    clock_t end = 0;
signals:
    void gameEnd(int);
};

#endif // GAMEWIDGET_H
