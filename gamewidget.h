#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include <QWidget>
#include "field.h"
#include "snake.h"
#include "game.h"
#include <QPainter>
#include <QKeyEvent>

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

    ~GameWidget();
private:
    Ui::GameWidget *ui;
    Game *game;
    int unitlen = 30;
    int cnt_time = 0;
    bool game_over = false;
    bool is_emit = false;
    int countdown = 3;
signals:
    void gameover();
};

#endif // GAMEWIDGET_H
