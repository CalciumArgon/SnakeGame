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
    QRect getRect(int, int);
    void paintEvent(QPaintEvent *ev);
    void keyPressEvent(QKeyEvent* event);
    void keyReleaseEvent(QKeyEvent* event);

    void enterGuide();
    void readFile(int);
    void startGuide();
    void turnPage();
    void endGuide();

    void paintBlocks();
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
    std::vector<QString> guide_line;

    ~GameWidget();
    int border = 300;
    int level = 0;

    bool on_guide = false;
    bool open = false;
    int page_num = 1;
private:
    Ui::GameWidget *ui;
    Game *game;
    int unitlen = 30;
    int cnt_time = 0;
    short game_end = 0;
    bool is_emit = false;
    int countdown = 4;
    clock_t begin = 0;
    clock_t end = 0;
signals:
    void gameEnd(int);
public slots:
    void on_btnNext_clicked();
    void on_btnSkip_clicked();
};

#endif // GAMEWIDGET_H
