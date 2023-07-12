#include "qlevel.h"
#include "ui_qlevel.h"
#include "mainwindow.h"
#include "gamewidget.h"
#include "field.h"
#include "snake.h"
#include "game.h"
#include "dialog.h"
#include "path.h"
#include <fstream>

using namespace std;

QLevel::QLevel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QLevel)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_StyledBackground);
    this->setWindowTitle("贪吃蛇大冒险");
    string dir_save = WORKING_DIR + "\\" + "save.txt";
    ifstream f;
    f.open(dir_save.c_str());
    for(int i = 0; i < 12; i++)
    {
        int num;
        f >> num;
        status.push_back(num);
        stars.push_back(0);
    }
    buttons = {ui->btnLev1, ui->btnLev2, ui->btnLev3, ui->btnLev4, ui->btnLev5, ui->btnLev6, ui->btnLev7, ui->btnLev8, ui->btnLev9, ui->btnLev10, ui->btnLev11, ui->btnLev12};
    for(int i = 1; i <= 12; i++)
    {
        paintStars(i);
    }
}

void QLevel::setBackground(QString color)
{
    for(auto btn : buttons)
        btn->setStyleSheet("background-color: "+color);
}

void QLevel::paintStars(int level)
{
    int id = level - 1;
    int x = buttons[id]->x();
    int y = buttons[id]->y();
    int star_pre = stars[id];
    int star_now = status[id];
    while(star_pre < star_now)
    {
        star_pre++;
        QLabel* ql = new QLabel();
        ql->setParent(this);
        switch(star_pre)
        {
        case 1:
            ql->setGeometry(x+5, y+125, 30, 30);
            break;
        case 2:
            ql->setGeometry(x+45, y+125, 30, 30);
            break;
        case 3:
            ql->setGeometry(x+85, y+125, 30, 30);
            break;
        }
        ql->setStyleSheet("border-image:url(:/star.png)");
        ql->show();
    }
    stars[id] = status[id];
}

void QLevel::saveData()
{
    string dir_save = WORKING_DIR + "\\" + "save.txt";
    ofstream f;
    f.open(dir_save.c_str());
    for(int i = 0; i < 12; i++)
    {
        f << status[i] << " ";
    }
}

QLevel::~QLevel()
{
    delete ui;
}

void QLevel::on_btnReturn_clicked()
{
    MainWindow *mw = new MainWindow();
    mw->show();
}

void QLevel::on_btnLev1_clicked(){
    Field* fld = new Field(40, 40);
    Game* game = new Level1(fld, TIMELIMIT, {5, 45});
    game->initializeGame(difficulty);
    GameWidget *gw = new GameWidget(game, 1);
    gw->initialize();
    Dialog *dlg = new Dialog();
    connect(gw, &GameWidget::gameEnd, dlg, &Dialog::endGame);
    connect(dlg, &Dialog::gameClose, gw, &GameWidget::close);
    connect(gw, &GameWidget::gameEnd, this, &QLevel::repaintStars);
    //gw->setFocusPolicy(Qt::ClickFocus);
    //gw->resize(1250+gw->border, 1250);
    gw->show();
}
void QLevel::on_btnLev2_clicked()
{
    Field* fld = new Field(40, 40);
    Game* game = new Level2(fld, TIMELIMIT, {5, 60});
    game->initializeGame(difficulty);
    GameWidget *gw = new GameWidget(game, 2);
    gw->initialize();
    Dialog *dlg = new Dialog();
    connect(gw, &GameWidget::gameEnd, dlg, &Dialog::endGame);
    connect(dlg, &Dialog::gameClose, gw, &GameWidget::close);
    connect(gw, &GameWidget::gameEnd, this, &QLevel::repaintStars);
    gw->show();
}

void QLevel::on_btnLev3_clicked()
{
    Field* fld = new Field(40, 40);
    Game* game = new Level3(fld, TIMELIMIT, {15, 75});
    game->initializeGame(difficulty);
    GameWidget *gw = new GameWidget(game, 3);
    gw->initialize();
    Dialog *dlg = new Dialog();
    connect(gw, &GameWidget::gameEnd, dlg, &Dialog::endGame);
    connect(dlg, &Dialog::gameClose, gw, &GameWidget::close);
    connect(gw, &GameWidget::gameEnd, this, &QLevel::repaintStars);
    gw->show();
}

void QLevel::on_btnLev4_clicked()
{
    Field* fld = new Field(40, 40);
    Game* game = new Level4(fld, TIMELIMIT, {5, 30});
    game->initializeGame(difficulty);
    GameWidget *gw = new GameWidget(game, 4);
    gw->initialize();
    Dialog *dlg = new Dialog();
    connect(gw, &GameWidget::gameEnd, dlg, &Dialog::endGame);
    connect(dlg, &Dialog::gameClose, gw, &GameWidget::close);
    connect(gw, &GameWidget::gameEnd, this, &QLevel::repaintStars);
    gw->show();
}

void QLevel::on_btnLev5_clicked()
{
    Field* fld = new Field(40, 40);
    Game* game = new Level5(fld, TIMELIMIT, {30, 30});
    game->initializeGame(difficulty);
    GameWidget *gw = new GameWidget(game, 5);
    gw->initialize();
    Dialog *dlg = new Dialog();
    connect(gw, &GameWidget::gameEnd, dlg, &Dialog::endGame);
    connect(dlg, &Dialog::gameClose, gw, &GameWidget::close);
    connect(gw, &GameWidget::gameEnd, this, &QLevel::repaintStars);
    gw->show();
}

void QLevel::on_btnLev6_clicked()
{
    Field* fld = new Field(40, 40);
    Game* game = new Level6(fld, TIMELIMIT, {10, 100});
    game->initializeGame(difficulty);
    GameWidget *gw = new GameWidget(game, 6);
    gw->initialize();
    Dialog *dlg = new Dialog();
    connect(gw, &GameWidget::gameEnd, dlg, &Dialog::endGame);
    connect(dlg, &Dialog::gameClose, gw, &GameWidget::close);
    connect(gw, &GameWidget::gameEnd, this, &QLevel::repaintStars);
    gw->show();
}

void QLevel::on_btnLev7_clicked()
{
    Field* fld = new Field(40, 40);
    Game* game = new Level7(fld, TIMELIMIT, {10, 100});
    game->initializeGame(difficulty);
    GameWidget *gw = new GameWidget(game, 7);
    gw->initialize();
    Dialog *dlg = new Dialog();
    connect(gw, &GameWidget::gameEnd, dlg, &Dialog::endGame);
    connect(dlg, &Dialog::gameClose, gw, &GameWidget::close);
    connect(gw, &GameWidget::gameEnd, this, &QLevel::repaintStars);
    gw->show();
}

void QLevel::on_btnLev8_clicked()
{
    Field* fld = new Field(40, 40);
    Game* game = new Level8(fld, TIMELIMIT, {20, 100});
    game->initializeGame(difficulty);
    GameWidget *gw = new GameWidget(game, 9);
    gw->initialize();
    Dialog *dlg = new Dialog();
    connect(gw, &GameWidget::gameEnd, dlg, &Dialog::endGame);
    connect(dlg, &Dialog::gameClose, gw, &GameWidget::close);
    connect(gw, &GameWidget::gameEnd, this, &QLevel::repaintStars);
    gw->show();
}

void QLevel::on_btnLev9_clicked()
{
    Field* fld = new Field(40, 40);
    Game* game = new Level9(fld, TIMELIMIT, {7, 1000});
    game->initializeGame(difficulty);
    GameWidget *gw = new GameWidget(game, 9);
    gw->initialize();
    Dialog *dlg = new Dialog();
    connect(gw, &GameWidget::gameEnd, dlg, &Dialog::endGame);
    connect(dlg, &Dialog::gameClose, gw, &GameWidget::close);
    connect(gw, &GameWidget::gameEnd, this, &QLevel::repaintStars);
    gw->show();
}

void QLevel::on_btnLev10_clicked()
{
    Field* fld = new Field(40, 40);
    Game* game = new Level10(fld, TIMELIMIT, {1, 100});
    game->initializeGame(difficulty);
    GameWidget *gw = new GameWidget(game, 10);
    gw->initialize();
    Dialog *dlg = new Dialog();
    connect(gw, &GameWidget::gameEnd, dlg, &Dialog::endGame);
    connect(dlg, &Dialog::gameClose, gw, &GameWidget::close);
    connect(gw, &GameWidget::gameEnd, this, &QLevel::repaintStars);
    gw->show();
}

void QLevel::on_btnLev11_clicked()
{

}

void QLevel::on_btnLev12_clicked()
{

}

void QLevel::on_btnEasy_clicked()
{
    setBackground("rgba(220, 246, 255, 220)");
    difficulty = 1;
}

void QLevel::on_btnHard_clicked()
{
    setBackground("rgba(255, 138, 117, 220)");
    difficulty = 2;
}

void QLevel::on_btnNightmare_clicked()
{
    setBackground("rgba(192, 114, 255, 220)");
    difficulty = 3;
}

void QLevel::repaintStars(int result, int level, int difficulty)
{
    if(!result) return;
    status[level-1] = difficulty;
    paintStars(level);
    saveData();
}

