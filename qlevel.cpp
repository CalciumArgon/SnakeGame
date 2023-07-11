#include "qlevel.h"
#include "ui_qlevel.h"
#include "mainwindow.h"
#include "gamewidget.h"
#include "field.h"
#include "snake.h"
#include "game.h"
#include "dialog.h"

using namespace std;

QLevel::QLevel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QLevel)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_StyledBackground);
}

void QLevel::setBackground(QString color)
{
    ui->btnLev1->setStyleSheet("background-color: "+color);
    ui->btnLev2->setStyleSheet("background-color: "+color);
    ui->btnLev3->setStyleSheet("background-color: "+color);
    ui->btnLev4->setStyleSheet("background-color: "+color);
    ui->btnLev5->setStyleSheet("background-color: "+color);
    ui->btnLev6->setStyleSheet("background-color: "+color);
    ui->btnLev7->setStyleSheet("background-color: "+color);
    ui->btnLev8->setStyleSheet("background-color: "+color);
    ui->btnLev9->setStyleSheet("background-color: "+color);
    ui->btnLev10->setStyleSheet("background-color: "+color);
    ui->btnLev11->setStyleSheet("background-color: "+color);
    ui->btnLev12->setStyleSheet("background-color: "+color);
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
    Game* game = new Game(fld, TIMEFREE, {5, 30});
    game->initializeGame(difficulty);
    GameWidget *gw = new GameWidget(game, 1);
    gw->initialize();
    Dialog *dlg = new Dialog();
    connect(gw, &GameWidget::gameEnd, dlg, &Dialog::endGame);
    connect(dlg, &Dialog::accepted, gw, &GameWidget::close);
    connect(dlg, &Dialog::rejected, gw, &GameWidget::close);
    //gw->setFocusPolicy(Qt::ClickFocus);
    //gw->resize(1250+gw->border, 1250);
    gw->show();
}
void QLevel::on_btnLev2_clicked()
{
    Field* fld = new Field(40, 40);
    Game* game = new AddWallGame(fld, TIMEFREE, {5, 30});
    game->initializeGame(difficulty);
    GameWidget *gw = new GameWidget(game, 2);
    gw->initialize();
    Dialog *dlg = new Dialog();
    connect(gw, &GameWidget::gameEnd, dlg, &Dialog::endGame);
    connect(dlg, &Dialog::accepted, gw, &GameWidget::close);
    connect(dlg, &Dialog::rejected, gw, &GameWidget::close);
    gw->show();
}

void QLevel::on_btnLev3_clicked()
{
    Field* fld = new Field(40, 40);
    Game* game = new Level3(fld, TIMEFREE, {15, 30});
    game->initializeGame(difficulty);
    GameWidget *gw = new GameWidget(game, 3);
    gw->initialize();
    Dialog *dlg = new Dialog();
    connect(gw, &GameWidget::gameEnd, dlg, &Dialog::endGame);
    connect(dlg, &Dialog::accepted, gw, &GameWidget::close);
    connect(dlg, &Dialog::rejected, gw, &GameWidget::close);
    gw->show();
}

void QLevel::on_btnLev4_clicked()
{
    Field* fld = new Field(40, 40);
    Game* game = new Level4(fld, TIMEFREE, {5, 30});
    game->initializeGame(difficulty);
    GameWidget *gw = new GameWidget(game, 4);
    gw->initialize();
    Dialog *dlg = new Dialog();
    connect(gw, &GameWidget::gameEnd, dlg, &Dialog::endGame);
    connect(dlg, &Dialog::accepted, gw, &GameWidget::close);
    connect(dlg, &Dialog::rejected, gw, &GameWidget::close);
    gw->show();
}

void QLevel::on_btnLev5_clicked()
{
    Field* fld = new Field(40, 40);
    Game* game = new Level5(fld, TIMEFREE, {30, 30});
    game->initializeGame(difficulty);
    GameWidget *gw = new GameWidget(game, 5);
    gw->initialize();
    Dialog *dlg = new Dialog();
    connect(gw, &GameWidget::gameEnd, dlg, &Dialog::endGame);
    connect(dlg, &Dialog::accepted, gw, &GameWidget::close);
    connect(dlg, &Dialog::rejected, gw, &GameWidget::close);
    gw->show();
}

void QLevel::on_btnLev6_clicked()
{
    Field* fld = new Field(40, 40);
    Game* game = new Level6(fld, TIMEFREE, {10, 100});
    game->initializeGame(difficulty);
    GameWidget *gw = new GameWidget(game, 6);
    gw->initialize();
    Dialog *dlg = new Dialog();
    connect(gw, &GameWidget::gameEnd, dlg, &Dialog::endGame);
    connect(dlg, &Dialog::accepted, gw, &GameWidget::close);
    connect(dlg, &Dialog::rejected, gw, &GameWidget::close);
    gw->show();
}

void QLevel::on_btnLev7_clicked()
{
    Field* fld = new Field(40, 40);
    Game* game = new Level7(fld, TIMEFREE, {10, 100});
    game->initializeGame(difficulty);
    GameWidget *gw = new GameWidget(game, 7);
    gw->initialize();
    Dialog *dlg = new Dialog();
    connect(gw, &GameWidget::gameEnd, dlg, &Dialog::endGame);
    connect(dlg, &Dialog::accepted, gw, &GameWidget::close);
    connect(dlg, &Dialog::rejected, gw, &GameWidget::close);
    gw->show();
}

void QLevel::on_btnLev8_clicked()
{
    Field* fld = new Field(40, 40);
    Game* game = new Level8(fld, TIMEFREE, {20, 100});
    game->initializeGame(difficulty);
    GameWidget *gw = new GameWidget(game, 9);
    gw->initialize();
    Dialog *dlg = new Dialog();
    connect(gw, &GameWidget::gameEnd, dlg, &Dialog::endGame);
    connect(dlg, &Dialog::accepted, gw, &GameWidget::close);
    connect(dlg, &Dialog::rejected, gw, &GameWidget::close);
    gw->show();
}

void QLevel::on_btnLev9_clicked()
{
    Field* fld = new Field(40, 40);
    Game* game = new Level9(fld, TIMEFREE, {20, 100});
    game->initializeGame(difficulty);
    GameWidget *gw = new GameWidget(game, 9);
    gw->initialize();
    Dialog *dlg = new Dialog();
    connect(gw, &GameWidget::gameEnd, dlg, &Dialog::endGame);
    connect(dlg, &Dialog::accepted, gw, &GameWidget::close);
    connect(dlg, &Dialog::rejected, gw, &GameWidget::close);
    gw->show();
}

void QLevel::on_btnLev10_clicked()
{
    Field* fld = new Field(40, 40);
    Game* game = new Level10(fld, TIMEFREE, {1, 100});
    game->initializeGame(difficulty);
    GameWidget *gw = new GameWidget(game, 10);
    gw->initialize();
    Dialog *dlg = new Dialog();
    connect(gw, &GameWidget::gameEnd, dlg, &Dialog::endGame);
    connect(dlg, &Dialog::accepted, gw, &GameWidget::close);
    connect(dlg, &Dialog::rejected, gw, &GameWidget::close);
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

