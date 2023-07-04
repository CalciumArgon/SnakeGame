#include "gamewidget.h"
#include "ui_gamewidget.h"
#include <QThread>

using namespace std;

GameWidget::GameWidget(Game* game, QWidget *parent) : ui(new Ui::GameWidget)
{
    Q_UNUSED(parent);
    ui->setupUi(this);
    this->game = game;
}

QRect GameWidget::getRect(int x, int y)
{
    int x1 = (x + 1) * unitlen;
    int y1 = (y + 1) * unitlen + 100;
    int x2 = (x + 2) * unitlen;
    int y2 = (y + 2) * unitlen + 100;
    return QRect(QPoint(x2,y2), QPoint(x1, y1));
}

void GameWidget::paintEvent(QPaintEvent *ev)
{
    Q_UNUSED(ev)
    //ui->labelTime->setText("游戏时间：");
    //ui->labelTime->show();
    QPainter painter(this);
    Field* mstate = game->getState();
    painter.setPen(Qt::black);
    painter.setBrush(Qt::gray);
    painter.setRenderHint(QPainter::Antialiasing);
    /* draw map */
    for (size_t i = 0; i < mstate->getWidth(); i++) {
        for (size_t j = 0; j < mstate->getHeight(); j++) {
                painter.setBrush(Qt::gray);
                QRect rect = getRect(i, j);
                painter.drawRect(rect);
        }
    }
    /* draw target
    painter.setBrush(Qt::green);
    painter.setPen(Qt::green);
    QRect rect1 = getRect(board.xt, board.yt);
    painter.drawRect(rect1);*/
    /* draw snake */
    Snake* msnake = game->getState()->getSnakes()[0];
    painter.setBrush(Qt::red);
    painter.setPen(Qt::red);
    for (std::size_t i = 0; i < msnake->getLength(); i++) {
        QRect rect = getRect(msnake->getBody()[i].first, msnake->getBody()[i].second);
        painter.drawRect(rect);
    }
    /* move */
    /*this->play2();*/
    QThread::msleep(50);
    return QWidget::paintEvent(ev);
}

void GameWidget::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
    case Qt::Key_Up:
    case Qt::Key_W:
        game->getState()->getSnakes()[0]->changeDireciton(UP);
        break;
    case Qt::Key_Down:
    case Qt::Key_S:
        game->getState()->getSnakes()[0]->changeDireciton(DOWN);
        break;
    case Qt::Key_Left:
    case Qt::Key_A:
        game->getState()->getSnakes()[0]->changeDireciton(LEFT);
        break;
    case Qt::Key_Right:
    case Qt::Key_D:
        game->getState()->getSnakes()[0]->changeDireciton(RIGHT);
        break;
    }
}

GameWidget::~GameWidget()
{
    delete ui;
}

