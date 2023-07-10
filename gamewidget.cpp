#include "gamewidget.h"
#include "ui_gamewidget.h"
#include <QThread>
#include <QColor>
#include <QDir>
#include <QTime>

using namespace std;

GameWidget::GameWidget(Game* game, QWidget *parent) : ui(new Ui::GameWidget)
{
    Q_UNUSED(parent);
    ui->setupUi(this);
    this->game = game;
    QPalette pal(this->palette());
    pal.setColor(QPalette::Background, QColor(204, 230, 199));
    this->setAutoFillBackground(true);
    this->setPalette(pal);

    //paint static items
    Field* mstate = game->getState();
    for (size_t i = 0; i < mstate->getWidth(); i++) {
        for (size_t j = 0; j < mstate->getHeight(); j++){
            if(mstate->getItemName(i, j) == MARSH){
                paintItem(i, j, MARSH);
            }
            if(mstate->getItemName(i, j) == WALL){
                paintItem(i, j, WALL);
            }
        }
    }
}

QRect GameWidget::getRect(int x, int y)
{
    int x1 = (x + 1) * unitlen + border;
    int y1 = (y + 1) * unitlen;
    int x2 = (x + 2) * unitlen + border;
    int y2 = (y + 2) * unitlen;
    return QRect(QPoint(x2,y2), QPoint(x1, y1));
}

void GameWidget::paintEvent(QPaintEvent *ev)
{
    Q_UNUSED(ev)

    //hide the label
    ui->labelBackground->hide();
    ui->labelCntDn->hide();
    ui->labelTime->hide();
    ui->labelHp1->hide();
    ui->labelHp2->hide();
    ui->labelHp3->hide();
    ui->labelHp4->hide();
    ui->progressBarMp->hide();



    //get current time and show
    end = clock();
    if(countdown < 0){
        if(!game_over)
            showTime(int((end - begin) / CLK_TCK));

    }
    else {
        showTime(0);
    }

    //set the painter of the main event
    QPainter painter(this);

    //set the position of the label
    ui->labelCntDn->setGeometry(380, 400, 600, 600);
    ui->labelBackground->setGeometry(unitlen, unitlen+100, unitlen*game->getState()->getWidth(), unitlen*game->getState()->getHeight());
    //show game time

    QString s;
    ui->labelTime->setText("游戏时间" + s.setNum(cnt_time));
    //ui->labelTime->show();

    //paint the area of hp and mp
    painter.setPen(QColor(222, 199, 12, 255));
    QRect hm = QRect(QPoint(5, 290), QPoint(320, 450));
    painter.drawRect(hm);
    //show hp level
    showHp();
    //show mp level
    showMp();

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

    //delete label
    deleteLabel();

    //delete food label
    deleteFoodLabel();

    //paint food
    for (size_t i = 0; i < mstate->getWidth(); i++) {
        for (size_t j = 0; j < mstate->getHeight(); j++){
            if(mstate->getItemName(i, j) == FOOD && mstate->getItem(i, j)->is_print == false){
                paintItem(i, j, FOOD);
            }
            if(mstate->getItemName(i, j) == MAGNET){
                paintItem(i, j, MAGNET);
            }
            if(mstate->getItemName(i, j) == FIRSTAID){
                painter.setBrush(Qt::black);
                QRect rect = getRect(i, j);
                painter.drawRect(rect);
            }
            if(mstate->getItemName(i, j) == OBSTACLE){
                painter.setBrush(Qt::blue);
                QRect rect = getRect(i, j);
                painter.drawRect(rect);
            }
        }
    }

    //paint snake
    paintMySnake();
    painter.setBrush(Qt::yellow);
    painter.setPen(Qt::yellow);
    for(int i = 1; i < game->getState()->getSnakes().size(); i++){
        Snake* osnake = game->getState()->getSnakes()[i];
        for (std::size_t i = 0; i < osnake->getLength(); i++) {
            QRect rect = getRect(osnake->getBody()[i].first, osnake->getBody()[i].second);
            painter.drawRect(rect);
        }
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
        {
            ui->labelCntDn->setStyleSheet("border-image: url(:/cdgo.png)");
            break;
        }
        case -1:
            begin = clock();
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
        cnt_time++;

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

void GameWidget::showTime(int time)
{
    ui->lcdNum4->setGeometry(0, 70, 81, 101);
    ui->lcdNum3->setGeometry(70, 70, 81, 101);
    ui->lcdNum2->setGeometry(140, 70, 81, 101);
    ui->lcdNum1->setGeometry(210, 70, 81, 101);
    ui->labelColon->setGeometry(135, 90, 21, 51);
    ui->labelColon->setText(":");
    QFont font;
    font.setPointSize(25);
    ui->labelColon->setFont(font);
    int minute = time / 60;
    int sec = time % 60;
    ui->lcdNum1->display(sec % 10);
    ui->lcdNum2->display(sec / 10);
    ui->lcdNum3->display(minute % 10);
    ui->lcdNum4->display(minute / 10);

}

void GameWidget::showHp()
{
    int hp = game->getState()->getSnakes()[0]->getHp();
    ui->labelHp1->setGeometry(10, 300, 70, 70);
    ui->labelHp2->setGeometry(90, 300, 70, 70);
    ui->labelHp3->setGeometry(170, 300, 70, 70);
    ui->labelHp4->setGeometry(250, 300, 70, 70);
    ui->labelHp1->setStyleSheet("border-image: url(:/no_health.png)");
    ui->labelHp2->setStyleSheet("border-image: url(:/no_health.png)");
    ui->labelHp3->setStyleSheet("border-image: url(:/no_health.png)");
    ui->labelHp4->setStyleSheet("border-image: url(:/no_health.png)");
    if(hp >= 1)
        ui->labelHp1->setStyleSheet("border-image: url(:/health.png)");
    if(hp >= 2)
        ui->labelHp2->setStyleSheet("border-image: url(:/health.png)");
    if(hp >= 3)
        ui->labelHp3->setStyleSheet("border-image: url(:/health.png)");
    if(hp >= 4)
        ui->labelHp4->setStyleSheet("border-image: url(:/health.png)");
    ui->labelHp1->show();
    ui->labelHp2->show();
    ui->labelHp3->show();
    ui->labelHp4->show();
}

void GameWidget::showMp()
{
    ui->progressBarMp->setValue(int(game->getState()->getSnakes()[0]->getMp()*100.0/240.0));
    ui->progressBarMp->setGeometry(10, 400, 300, 40);
    ui->progressBarMp->setStyleSheet("QProgressBar::chunk{background-color: rgb(0, 59, 255)}");
    ui->progressBarMp->show();
}

void GameWidget::paintItem(int i, int j, ItemType type)
{
    QLabel *ql = new QLabel();
    ql->setParent(this);
    ql->setGeometry((i+1)*unitlen+border, (j+1)*unitlen, unitlen, unitlen);
    ql->show();
    ql->raise();
    switch(type){
    case FOOD:
        ql->setStyleSheet("border-image: url(:/food1.png)");
        game->getState()->getItem(i, j)->is_print = true;
        food_label.push_back(ql);
        break;
    case WALL:
        ql->setStyleSheet("border-image: url(:/wall.png)");
        static_label.push_back(ql);
        break;
    case MARSH:
        ql->setStyleSheet("border-image: url(:/marsh.png)");
        static_label.push_back(ql);
        break;
    case MAGNET:
        ql->setStyleSheet("border-image: url(:/magnet.png)");
        dynamic_label.push_back(ql);
        break;
    }


}

void GameWidget::paintMySnake()
{
    Snake* msnake = game->getState()->getSnakes()[0];
    Loc head = msnake->getBody()[0];
    QLabel* ql = new QLabel();
    ql->setParent(this);
    ql->setGeometry((head.first+1)*unitlen+border, (head.second+1)*unitlen, unitlen, unitlen);
    switch(msnake->getDirection()){
    case UP:
        ql->setStyleSheet("border-image: url(:/snakehead_up.png)");
        break;
    case DOWN:
        ql->setStyleSheet("border-image: url(:/snakehead_down.png)");
        break;
    case LEFT:
        ql->setStyleSheet("border-image: url(:/snakehead_left.png)");
        break;
    case RIGHT:
        ql->setStyleSheet("border-image: url(:/snakehead_right.png)");
        break;
    }
    ql->show();
    ql->raise();
    dynamic_label.push_back(ql);
    for (std::size_t i = 1; i < msnake->getLength(); i++) {
        QLabel* ql = new QLabel();
        ql->setParent(this);
        ql->setGeometry((msnake->getBody()[i].first+1)*unitlen+border, (msnake->getBody()[i].second+1)*unitlen, unitlen, unitlen);
        switch(msnake->getBodyDirection(i)){
        case UP:
            ql->setStyleSheet("border-image: url(:/snakebody_up.png)");
            break;
        case DOWN:
            ql->setStyleSheet("border-image: url(:/snakebody_down.png)");
            break;
        case LEFT:
            ql->setStyleSheet("border-image: url(:/snakebody_left.png)");
            break;
        case RIGHT:
            ql->setStyleSheet("border-image: url(:/snakebody_right.png)");
            break;
        }
        ql->show();
        ql->raise();
        dynamic_label.push_back(ql);
    }
}

void GameWidget::deleteLabel()
{
    vector<QLabel*>::iterator vec_it = dynamic_label.begin();
    for(; vec_it != dynamic_label.end(); vec_it++)
    {
        if(*vec_it != NULL)
        {
          delete *vec_it;
          *vec_it = NULL;
        }
    }
    dynamic_label.clear();
}

void GameWidget::deleteFoodLabel()
{
    vector<QLabel*>::iterator vec_it = food_label.begin();
    for(; vec_it != food_label.end(); vec_it++)
    {
        if(*vec_it == NULL) continue;
        int i = ((*vec_it)->x()-border)/unitlen - 1;
        int j = (*vec_it)->y()/unitlen - 1;
        if(game->getState()->getItemName(i, j) != FOOD){
            delete *vec_it;
            *vec_it = NULL;
        }
    }
    vec_it = food_label.begin();
    vector<QLabel*> labeltmp;
    for(; vec_it != food_label.end(); vec_it++)
        labeltmp.push_back(*vec_it);
    food_label.clear();
    vec_it = labeltmp.begin();
    for(; vec_it != labeltmp.end(); vec_it++)
        if(*vec_it != NULL)
            food_label.push_back(*vec_it);
}



GameWidget::~GameWidget()
{
    delete ui;
}

