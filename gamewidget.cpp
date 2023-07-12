#include "gamewidget.h"
#include "ui_gamewidget.h"
#include <QThread>
#include <QColor>
#include <QDir>
#include <QTime>
#include <QDebug>
#include <string>
#include <fstream>
#include "path.h"
#include <algorithm>

using namespace std;

GameWidget::GameWidget(Game* game, int level, QWidget *parent) : ui(new Ui::GameWidget)
{
    Q_UNUSED(parent);
    ui->setupUi(this);
    this->setWindowTitle("贪吃蛇大冒险");
    this->game = game;
    this->level = level;
}

void GameWidget::initialize()
{
    this->setFocusPolicy(Qt::ClickFocus);
    this->resize(1250+border, 1250);

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
            if(mstate->getItemName(i, j) == AEROLITE){
                QLabel *ql1 = new QLabel();
                ql1->setParent(this);
                ql1->setStyleSheet("border-image:url(:/asteroid.png)");
                ql1->setGeometry((i+1)*unitlen+border, (j+1)*unitlen, unitlen, unitlen);
                ql1->hide();
                aerolite_label.push_back(ql1);
                QLabel *ql2 = new QLabel();
                ql2->setParent(this);
                ql2->setStyleSheet("border-image:url(:/warning.png)");
                ql2->setGeometry((i+1)*unitlen+border, (j+1)*unitlen, unitlen, unitlen);
                ql2->hide();
                warning_label.push_back(ql2);
            }
        }
    }

    ui->labelCntDn->hide();

    vector<Snake*> msnakes = game->getState()->getSnakes();
    int num = msnakes.size();
    for(int i = 0; i < num; i++)
    {
        vector<QLabel*> vec;
        snake_label.push_back(vec);
        vector<Direction> vec2;
        snake_direction.push_back(vec2);
        snake_length.push_back(0);
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
    loop_counter++;
    Q_UNUSED(ev)

    //hide the label
    ui->progressBarMp->hide();
    /*ui->labelGuide->hide();
    ui->btnNext->hide();
    ui->btnSkip->hide();*/

    //get current time and show
    end = clock();
    if(countdown < 0 && game_start){
        if(!game_end)
        {
            showTime(int((end - begin) / CLK_TCK));
        }
    }
    else {
        showTime(0);
    }

    //set the painter of the main event
    QPainter painter(this);

    //set the position of the label
    ui->labelCntDn->setGeometry(380, 400, 600, 600);
    //show game time

    QString s;
    ui->labelTime->setText("速度" + s.setNum(game->getState()->getSnakes()[0]->speed));
    ui->labelTime->hide();

    //show hp level
    showHp();
    //show mp level
    showMp();

    showScore();

    //paint the blocks
    Field* mstate = game->getState();
    painter.setPen(Qt::black);
    painter.setBrush(QColor(1, 1, 1, 0));
    for (size_t i = 0; i < mstate->getWidth(); i++) {
        for (size_t j = 0; j < mstate->getHeight(); j++) {
                QRect rect = getRect(i, j);
                painter.drawRect(rect);
        }
    }

    //delete label
    deleteLabel();

    //delete food label
    deleteFoodLabel();
    deleteMagnetLabel();
    deleteObstacleLabel();
    deleteFirstaidLabel();
    deleteShieldLabel();

    //paint food
    for (size_t i = 0; i < mstate->getWidth(); i++) {
        for (size_t j = 0; j < mstate->getHeight(); j++){
            if(mstate->getItemName(i, j) == FOOD && mstate->getItem(i, j)->is_print == false){
                paintItem(i, j, FOOD);
            }
            if(mstate->getItemName(i, j) == MAGNET && mstate->getItem(i, j)->is_print == false){
                paintItem(i, j, MAGNET);
            }
            if(mstate->getItemName(i, j) == OBSTACLE && mstate->getItem(i, j)->is_print == false){
                paintItem(i, j, OBSTACLE);
            }
            if(mstate->getItemName(i, j) == FIRSTAID && mstate->getItem(i, j)->is_print == false){
                paintItem(i, j, FIRSTAID);
            }
            if(mstate->getItemName(i, j) == SHIELD && mstate->getItem(i, j)->is_print == false){
                paintItem(i, j, SHIELD);
            }
        }
    }

    if(game->isWarning())
    {
        for(auto ql : warning_label)
            ql->show();
    }
    else {
        for(auto ql : warning_label)
            ql->hide();
    }

    vector<Snake*> msnakes = game->getState()->getSnakes();

    //paint snake

    paintSnake(0, msnakes[0]->getLength() - snake_length[0]);
    for(int i = 1; i < msnakes.size(); i++){
        paintSnake(i, msnakes[i]->getLength() - snake_length[i]);
    }

    for(int i = 0; i < msnakes.size(); i++){
        snake_length[i] = msnakes[i]->getLength();
        vector<Direction> mlength_vec;
        for(int j = 0; j < msnakes[i]->getLength(); j++)
        {
            mlength_vec.push_back(msnakes[i]->getBodyDirection(j));
        }
        snake_direction[i] = mlength_vec;
    }

    if(game->isFall())
    {
        for(auto ql : aerolite_label)
            ql->show();
    }
    else {
        for(auto ql : aerolite_label)
            ql->hide();
    }


    if(countdown == 4 && !on_guide){
        enterGuide();
        //countdown--;
        update();
        return QWidget::paintEvent(ev);
    }

    //paint the countdown
    if(countdown >= -1 && countdown <= 3)
    {
        ui->labelCntDn->setGeometry(400, 200, 800, 800);
        ui->labelCntDn->show();
        ui->labelCntDn->raise();
        switch(countdown)
        {
        case 3:
            begin = clock();
            this->game->setBeginTime(begin);
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
    } else {
        ui->labelCntDn->hide();
    }

    //move the determine whether the game is over
    if(game_end  == 0 && !on_guide && countdown < 0) {
        if(!game_start){
            begin = clock();
            this->game->setBeginTime(begin);
            game_start = true;
        }
        game_end = game->runGame();
    }
    if(game_end != 0 && !is_emit) {
        if(game_end == 1){
            emit(gameEnd(1, this->level, game->level));
            is_emit = true;
        }
        else {
            emit(gameEnd(0, this->level, game->level));
            is_emit = true;
        }
    }

    QThread::msleep(5);
    if (game_end == 0) {
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

void GameWidget::enterGuide()
{
    readFile(level);
    startGuide();
}

void GameWidget::readFile(int level)
{
    string dir = WORKING_DIR + "\\guide\\" + to_string(level) + ".txt";
    ifstream guide_file;
    guide_file.open(dir.c_str());
    string s;
    while(getline(guide_file, s))
        guide_line.push_back(QString::fromStdString(s));
    page_num = 1;
}

void GameWidget::startGuide()
{
    ui->labelGuide->setStyleSheet("background-color:rgb(191, 255, 155, 200)");
    ui->labelGuide->setWordWrap(true);
    ui->labelGuide->setGeometry(590, 0, 950, 400);
    QFont font("华文新魏", 18);
    ui->labelGuide->setFont(font);
    ui->labelGuide->setText(guide_line[0]);
    ui->btnNext->setGeometry(1220, 340, 100, 40);
    ui->btnSkip->setGeometry(1350, 340, 100, 40);
    ui->labelGuide->show();
    ui->btnNext->show();
    ui->btnSkip->show();
    ui->labelGuide->raise();
    ui->btnNext->raise();
    ui->btnSkip->raise();
    on_guide = true;
}

void GameWidget::turnPage()
{
    if(page_num == guide_line.size())
        endGuide();
    else {
        page_num++;
    }
    ui->labelGuide->setText(guide_line[page_num-1]);
}

void GameWidget::endGuide()
{
    ui->labelGuide->close();
    ui->btnNext->close();
    ui->btnSkip->close();
    on_guide = false;
    countdown = 3;
}

void GameWidget::paintBlocks()
{
    QPainter painter(this);
    Field* mstate = game->getState();
    painter.setPen(Qt::black);
    painter.setBrush(QColor(1, 1, 1, 0));
    for (size_t i = 0; i < mstate->getWidth(); i++) {
        for (size_t j = 0; j < mstate->getHeight(); j++) {
                QRect rect = getRect(i, j);
                painter.drawRect(rect);
        }
    }
}

void GameWidget::showTime(int time)
{
    GameMode gm = game->game_mode;
    ui->labelTextTime->setGeometry(20, -30, 600, 200);
    if(gm == TIMELIMIT) {
        ui->labelTextTime->setText("Time Remained");
        time = game->target_time - time;
    }
    else {
        ui->labelTextTime->setText("Time Used");
    }
    ui->lcdNum4->setGeometry(0, 120, 81, 101);
    ui->lcdNum3->setGeometry(70, 120, 81, 101);
    ui->lcdNum2->setGeometry(140, 120, 81, 101);
    ui->lcdNum1->setGeometry(210, 120, 81, 101);
    ui->labelColon->setGeometry(130, 140, 21, 51);
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

void GameWidget::showScore()
{
    ui->labelTextScore->setGeometry(30, 500, 400, 100);

    QFont font("华文新魏", 30);
    QString s;

    int score = game->getState()->getSnakes()[0]->score();
    ui->labelScore->setGeometry(50, 560, 200, 150);
    ui->labelScore->setFont(font);
    ui->labelScore->setText(s.setNum(score));
    ui->labelScore->show();

    int target = game->target_score;
    ui->labelTarget->setGeometry(210, 560, 200, 150);
    ui->labelTarget->setFont(font);
    ui->labelTarget->setText(s.setNum(target));
    ui->labelTarget->show();

    ui->labelSlash->setText("/");
    ui->labelSlash->setGeometry(150, 560, 200, 150);
    ui->labelSlash->setFont(font);
    ui->labelSlash->show();

    ui->labelExplain->setGeometry(50, 680, 220, 60);
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
        game->getState()->getItem(i, j)->is_print = true;
        magnet_label.push_back(ql);
        break;
    case OBSTACLE:
        ql->setStyleSheet("border-image: url(:/obstacle.png)");
        game->getState()->getItem(i, j)->is_print = true;
        obstacle_label.push_back(ql);
        break;
    case FIRSTAID:
        ql->setStyleSheet("border-image: url(:/firstaid.png)");
        game->getState()->getItem(i, j)->is_print = true;
        firstaid_label.push_back(ql);
        break;
    case SHIELD:
        ql->setStyleSheet("border-image: url(:/shield.png)");
        game->getState()->getItem(i, j)->is_print = true;
        shield_label.push_back(ql);
        break;
    }


}

void GameWidget::paintSnake(int id, int change)
{
    Snake* msnake = game->getState()->getSnakes()[id];
    int length = snake_label[id].size();
    if(snake_label[id].size() == 0)
    {
        Loc head = msnake->getBody()[0];
        QLabel* ql = new QLabel();
        ql->setParent(this);
        ql->setGeometry((head.first+1)*unitlen+border, (head.second+1)*unitlen, unitlen, unitlen);
        switch(msnake->getDirection()){
        case UP:
            if(id == 0)
                ql->setStyleSheet("border-image: url(:/snakehead_up.png)");
            else
                ql->setStyleSheet("border-image: url(:/aisnakehead_up.png)");
            break;
        case DOWN:
            if(id == 0)
                ql->setStyleSheet("border-image: url(:/snakehead_down.png)");
            else
                ql->setStyleSheet("border-image: url(:/aisnakehead_down.png)");
            break;
        case LEFT:
            if(id == 0)
                ql->setStyleSheet("border-image: url(:/snakehead_left.png)");
            else
                ql->setStyleSheet("border-image: url(:/aisnakehead_left.png)");
            break;
        case RIGHT:
            if(id == 0)
                ql->setStyleSheet("border-image: url(:/snakehead_right.png)");
            else
                ql->setStyleSheet("border-image: url(:/aisnakehead_right.png)");
            break;
        }
        ql->show();
        ql->raise();
        snake_label[id].push_back(ql);
        change--;
    }
    if(change < 0)
    {
        reverse(snake_label[id].begin(), snake_label[id].end());
        vector<QLabel*>::iterator vec_it = snake_label[id].begin();
        for(int i = 0; i < -change; i++)
        {
            delete *vec_it;
            *vec_it = NULL;
            vec_it++;
        }
        reverse(snake_label[id].begin(), snake_label[id].end());
        for(int i = 0; i < -change; i++)
        {
            snake_label[id].pop_back();
        }
        length += change;
    }
    if(change > 0)
    {
        for(int i = msnake->getLength() - change; i < msnake->getLength(); i++)
        {
            QLabel* ql = new QLabel();
            ql->setParent(this);
            ql->setGeometry((msnake->getBody()[i].first+1)*unitlen+border, (msnake->getBody()[i].second+1)*unitlen, unitlen, unitlen);
            switch(msnake->getBodyDirection(i)){
            case UP:
                if(id == 0)
                    ql->setStyleSheet("border-image: url(:/snakebody_up.png)");
                else
                    ql->setStyleSheet("border-image: url(:/aisnakebody_up.png)");
                break;
            case DOWN:
                if(id == 0)
                    ql->setStyleSheet("border-image: url(:/snakebody_down.png)");
                else
                    ql->setStyleSheet("border-image: url(:/aisnakebody_down.png)");
                break;
            case LEFT:
                if(id == 0)
                    ql->setStyleSheet("border-image: url(:/snakebody_left.png)");
                else
                    ql->setStyleSheet("border-image: url(:/aisnakebody_left.png)");
                break;
            case RIGHT:
                if(id == 0)
                    ql->setStyleSheet("border-image: url(:/snakebody_right.png)");
                else
                    ql->setStyleSheet("border-image: url(:/aisnakebody_right.png)");
                break;
            }
            ql->show();
            ql->raise();
            snake_label[id].push_back(ql);
        }
    }
    for (std::size_t i = 0; i < length; i++) {
        snake_label[id][i]->setGeometry((msnake->getBody()[i].first+1)*unitlen+border, (msnake->getBody()[i].second+1)*unitlen, unitlen, unitlen);
        Direction old_direction = snake_direction[id][i];
        Direction new_direction = msnake->getBodyDirection(i);
        if(old_direction != new_direction)
        {
            switch(msnake->getBodyDirection(i)){
            case UP:
                if(id == 0){
                    if(i == 0)
                        snake_label[id][i]->setStyleSheet("border-image: url(:/snakehead_up.png)");
                    else
                        snake_label[id][i]->setStyleSheet("border-image: url(:/snakebody_up.png)");
                }
                else {
                    if(i == 0)
                        snake_label[id][i]->setStyleSheet("border-image: url(:/aisnakehead_up.png)");
                    else
                        snake_label[id][i]->setStyleSheet("border-image: url(:/aisnakebody_up.png)");
                }
                break;
            case DOWN:
                if(id == 0){
                    if(i == 0)
                        snake_label[id][i]->setStyleSheet("border-image: url(:/snakehead_down.png)");
                    else
                        snake_label[id][i]->setStyleSheet("border-image: url(:/snakebody_down.png)");
                }
                else {
                    if(i == 0)
                        snake_label[id][i]->setStyleSheet("border-image: url(:/aisnakehead_down.png)");
                    else
                        snake_label[id][i]->setStyleSheet("border-image: url(:/aisnakebody_down.png)");
                }
                break;
            case LEFT:
                if(id == 0){
                    if(i == 0)
                        snake_label[id][i]->setStyleSheet("border-image: url(:/snakehead_left.png)");
                    else
                        snake_label[id][i]->setStyleSheet("border-image: url(:/snakebody_left.png)");
                }
                else {
                    if(i == 0)
                        snake_label[id][i]->setStyleSheet("border-image: url(:/aisnakehead_left.png)");
                    else
                        snake_label[id][i]->setStyleSheet("border-image: url(:/aisnakebody_left.png)");
                }
                break;
            case RIGHT:
                if(id == 0){
                    if(i == 0)
                        snake_label[id][i]->setStyleSheet("border-image: url(:/snakehead_right.png)");
                    else
                        snake_label[id][i]->setStyleSheet("border-image: url(:/snakebody_right.png)");
                }
                else {
                    if(i == 0)
                        snake_label[id][i]->setStyleSheet("border-image: url(:/aisnakehead_right.png)");
                    else
                        snake_label[id][i]->setStyleSheet("border-image: url(:/aisnakebody_right.png)");
                }
                break;
            }
        }
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

void GameWidget::deleteMagnetLabel()
{
    vector<QLabel*>::iterator vec_it = magnet_label.begin();
    for(; vec_it != magnet_label.end(); vec_it++)
    {
        if(*vec_it == NULL) continue;
        int i = ((*vec_it)->x()-border)/unitlen - 1;
        int j = (*vec_it)->y()/unitlen - 1;
        if(game->getState()->getItemName(i, j) != MAGNET){
            delete *vec_it;
            *vec_it = NULL;
        }
    }
    vec_it = magnet_label.begin();
    vector<QLabel*> labeltmp;
    for(; vec_it != magnet_label.end(); vec_it++)
        labeltmp.push_back(*vec_it);
    magnet_label.clear();
    vec_it = labeltmp.begin();
    for(; vec_it != labeltmp.end(); vec_it++)
        if(*vec_it != NULL)
            magnet_label.push_back(*vec_it);
}

void GameWidget::deleteObstacleLabel()
{
    vector<QLabel*>::iterator vec_it = obstacle_label.begin();
    for(; vec_it != obstacle_label.end(); vec_it++)
    {
        if(*vec_it == NULL) continue;
        int i = ((*vec_it)->x()-border)/unitlen - 1;
        int j = (*vec_it)->y()/unitlen - 1;
        if(game->getState()->getItemName(i, j) != OBSTACLE){
            delete *vec_it;
            *vec_it = NULL;
        }
    }
    vec_it = obstacle_label.begin();
    vector<QLabel*> labeltmp;
    for(; vec_it != obstacle_label.end(); vec_it++)
        labeltmp.push_back(*vec_it);
    obstacle_label.clear();
    vec_it = labeltmp.begin();
    for(; vec_it != labeltmp.end(); vec_it++)
        if(*vec_it != NULL)
            obstacle_label.push_back(*vec_it);
}

void GameWidget::deleteFirstaidLabel()
{
    vector<QLabel*>::iterator vec_it = firstaid_label.begin();
    for(; vec_it != firstaid_label.end(); vec_it++)
    {
        if(*vec_it == NULL) continue;
        int i = ((*vec_it)->x()-border)/unitlen - 1;
        int j = (*vec_it)->y()/unitlen - 1;
        if(game->getState()->getItemName(i, j) != FIRSTAID){
            delete *vec_it;
            *vec_it = NULL;
        }
    }
    vec_it = firstaid_label.begin();
    vector<QLabel*> labeltmp;
    for(; vec_it != firstaid_label.end(); vec_it++)
        labeltmp.push_back(*vec_it);
    firstaid_label.clear();
    vec_it = labeltmp.begin();
    for(; vec_it != labeltmp.end(); vec_it++)
        if(*vec_it != NULL)
            firstaid_label.push_back(*vec_it);
}

void GameWidget::deleteShieldLabel()
{
    vector<QLabel*>::iterator vec_it = shield_label.begin();
    for(; vec_it != shield_label.end(); vec_it++)
    {
        if(*vec_it == NULL) continue;
        int i = ((*vec_it)->x()-border)/unitlen - 1;
        int j = (*vec_it)->y()/unitlen - 1;
        if(game->getState()->getItemName(i, j) != SHIELD){
            delete *vec_it;
            *vec_it = NULL;
        }
    }
    vec_it = shield_label.begin();
    vector<QLabel*> labeltmp;
    for(; vec_it != shield_label.end(); vec_it++)
        labeltmp.push_back(*vec_it);
    shield_label.clear();
    vec_it = labeltmp.begin();
    for(; vec_it != labeltmp.end(); vec_it++)
        if(*vec_it != NULL)
            shield_label.push_back(*vec_it);
}

GameWidget::~GameWidget()
{
    delete ui;
}

void GameWidget::on_btnNext_clicked()
{
    if(on_guide)
        turnPage();
}

void GameWidget::on_btnSkip_clicked()
{
    if(on_guide)
        endGuide();
}

