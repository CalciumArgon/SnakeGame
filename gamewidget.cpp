#include "gamewidget.h"
#include "ui_gamewidget.h"
#include <QThread>
#include <QColor>

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

    //hide the label
    ui->labelBackground->hide();
    ui->labelFood->hide();
    ui->labelCntDn->hide();
    ui->labelTime->hide();

    //set the position of the label
    ui->labelCntDn->setGeometry(380, 400, 600, 600);
    ui->labelBackground->setGeometry(unitlen, unitlen+100, unitlen*game->getState()->getWidth(), unitlen*game->getState()->getHeight());
    //show game time
    QString s;
    ui->labelTime->setText(s.setNum(cnt_time));
    ui->labelTime->show();
    ui->labelMp->setText("mp值：" + s.setNum(game->getState()->getSnakes()[0]->getMp()));
    ui->labelMp->show();

    //set the painter of the main event
    QPainter painter(this);

    //paint the blocks
    Field* mstate = game->getState();
    painter.setPen(Qt::black);
    painter.setBrush(QColor(1, 1, 1, 0));
    //painter.setRenderHint(QPainter::Antialiasing);
    for (size_t i = 0; i < mstate->getWidth(); i++) {
        for (size_t j = 0; j < mstate->getHeight(); j++) {
            if(mstate->getItemName(i, j) == BASIC){
                //painter.setBrush(Qt::gray);
                QRect rect = getRect(i, j);
                painter.drawRect(rect);
            }
        }
    }

    //paint the background
    //ui->labelBackground->setStyleSheet("border-image: url(:/background.png)");
    //ui->labelBackground->show();

    //paint food
    for (size_t i = 0; i < mstate->getWidth(); i++) {
        for (size_t j = 0; j < mstate->getHeight(); j++){
            if(mstate->getItemName(i, j) == FOOD){
                /*ui->labelFood->setGeometry((i+1)*unitlen, (j+1)*unitlen+100, unitlen, unitlen);
                ui->labelFood->show();
                ui->labelFood->raise();*/
                painter.setBrush(Qt::red);
                QRect rect = getRect(i, j);
                painter.drawRect(rect);
            }
            if(mstate->getItemName(i, j) == MARSH){
                /*
                ui->labelMarsh->setStyleSheet("border-image: url(:/marsh.png)");
                ui->labelMarsh->setGeometry((i+1)*unitlen, (j+1)*unitlen+100, unitlen, unitlen);
                ui->labelMarsh->raise();*/
                painter.setBrush(Qt::blue);
                QRect rect = getRect(i, j);
                painter.drawRect(rect);
            }
            if(mstate->getItemName(i, j) == WALL){
                /*
                ui->labelMarsh->setStyleSheet("border-image: url(:/marsh.png)");
                ui->labelMarsh->setGeometry((i+1)*unitlen, (j+1)*unitlen+100, unitlen, unitlen);
                ui->labelMarsh->raise();*/
                painter.setBrush(Qt::black);
                QRect rect = getRect(i, j);
                painter.drawRect(rect);
            }
        }
    }

    //paint snake
    Snake* msnake = game->getState()->getSnakes()[0];
    painter.setBrush(Qt::green);
    painter.setPen(Qt::green);
    for (std::size_t i = 0; i < msnake->getLength(); i++) {
        QRect rect = getRect(msnake->getBody()[i].first, msnake->getBody()[i].second);
        painter.drawRect(rect);
    }

    //paint the countdown
    if(countdown >= -1)
    {
        ui->labelCntDn->show();
        ui->labelCntDn->raise();
        switch(countdown)
        {
        case 3:
            ui->labelCntDn->setStyleSheet("border-image: url(:/cd3.png)");
            break;
        case 2:
            ui->labelCntDn->setStyleSheet("border-image: url(:/cd2.png)");
            break;
        case 1:
            ui->labelCntDn->setStyleSheet("border-image: url(:/cd1.png)");
            break;
        case 0:
            ui->labelCntDn->setStyleSheet("border-image: url(:/cdgo.png)");
            break;
        default:
            break;
        }
        countdown--;
        if(countdown != 2){
        QThread::msleep(1000);
        update();

        return QWidget::paintEvent(ev);
        }
    }
    else {
        ui->labelCntDn->hide();
    }

    //move the determine whether the game is over
    if(!game_over) game_over = !game->runGame();
    if(game_over && !is_emit) {
        emit(gameover());
        is_emit = true;
    }
    QThread::msleep(5);
    if(!game_over) {
        cnt_time = game->getState()->getSnakes()[0]->getSpeed();
        update();
    }
    return QWidget::paintEvent(ev);
}

void GameWidget::keyPressEvent(QKeyEvent *event)
{
    event->accept();
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
    case Qt::Key_Control:
        game->getState()->getSnakes()[0]->speed_buff = true;
    }
}

void GameWidget::keyReleaseEvent(QKeyEvent *event)
{
    event->accept();
    if(event->key() == Qt::Key_Control)
        game->getState()->getSnakes()[0]->speed_buff = false;
}



GameWidget::~GameWidget()
{
    delete ui;
}
