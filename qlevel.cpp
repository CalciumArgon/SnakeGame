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
}

QLevel::~QLevel()
{
    delete ui;
}

void QLevel::on_btnReturn_clicked()
{
    MainWindow *qmw = new MainWindow();
    qmw->show();
}

void QLevel::on_btnLev1_clicked(){
    Field* fld = new Field(40, 40);
    Game* game = new Game(fld, TIMEFREE, {5, 30});
    game->initializeGame(1);
    GameWidget *gw = new GameWidget(game, 1);
    Dialog *dlg = new Dialog();
    connect(gw, &GameWidget::gameEnd, dlg, &Dialog::endGame);
    connect(dlg, &Dialog::accepted, gw, &GameWidget::close);
    connect(dlg, &Dialog::rejected, gw, &GameWidget::close);
    gw->setFocusPolicy(Qt::ClickFocus);
    gw->resize(1250+gw->border, 1250);
    gw->show();
}
void QLevel::on_btnLev2_clicked()
{
    Field* fld = new Field(40, 40);
    Game* game = new AddWallGame(fld, TIMEFREE, {5, 30});
    game->initializeGame(1);
    GameWidget *gw = new GameWidget(game, 2);
    Dialog *dlg = new Dialog();
    connect(gw, &GameWidget::gameEnd, dlg, &Dialog::endGame);
    connect(dlg, &Dialog::accepted, gw, &GameWidget::close);
    connect(dlg, &Dialog::rejected, gw, &GameWidget::close);
    gw->setFocusPolicy(Qt::ClickFocus);
    gw->resize(1250+gw->border, 1250);
    gw->show();
}

void QLevel::on_btnLev3_clicked()
{
    Field* fld = new Field(40, 40);
    Game* game = new Level3(fld, TIMEFREE, {15, 30});
    game->initializeGame(1);
    GameWidget *gw = new GameWidget(game, 3);
    Dialog *dlg = new Dialog();
    connect(gw, &GameWidget::gameEnd, dlg, &Dialog::endGame);
    connect(dlg, &Dialog::accepted, gw, &GameWidget::close);
    connect(dlg, &Dialog::rejected, gw, &GameWidget::close);
    gw->setFocusPolicy(Qt::ClickFocus);
    gw->resize(1250+gw->border, 1250);
    gw->show();
}

void QLevel::on_btnLev4_clicked()
{
    Field* fld = new Field(40, 40);
    Game* game = new Level4(fld, TIMEFREE, {5, 30});
    game->initializeGame(1);
    GameWidget *gw = new GameWidget(game, 4);
    Dialog *dlg = new Dialog();
    connect(gw, &GameWidget::gameEnd, dlg, &Dialog::endGame);
    connect(dlg, &Dialog::accepted, gw, &GameWidget::close);
    connect(dlg, &Dialog::rejected, gw, &GameWidget::close);
    gw->setFocusPolicy(Qt::ClickFocus);
    gw->resize(1250+gw->border, 1250);
    gw->show();
}

void QLevel::on_btnLev5_clicked()
{
    Field* fld = new Field(40, 40);
    Game* game = new Level5(fld, TIMEFREE, {30, 30});
    game->initializeGame(1);
    GameWidget *gw = new GameWidget(game, 5);
    Dialog *dlg = new Dialog();
    connect(gw, &GameWidget::gameEnd, dlg, &Dialog::endGame);
    connect(dlg, &Dialog::accepted, gw, &GameWidget::close);
    connect(dlg, &Dialog::rejected, gw, &GameWidget::close);
    gw->setFocusPolicy(Qt::ClickFocus);
    gw->resize(1250+gw->border, 1250);
    gw->show();
}

void QLevel::on_btnLev6_clicked()
{
    Field* fld = new Field(40, 40);
    Game* game = new Level6(fld, TIMEFREE, {10, 100});
    game->initializeGame(1);
    GameWidget *gw = new GameWidget(game, 6);
    Dialog *dlg = new Dialog();
    connect(gw, &GameWidget::gameEnd, dlg, &Dialog::endGame);
    connect(dlg, &Dialog::accepted, gw, &GameWidget::close);
    connect(dlg, &Dialog::rejected, gw, &GameWidget::close);
    gw->setFocusPolicy(Qt::ClickFocus);
    gw->resize(1250+gw->border, 1250);
    gw->show();
}

void QLevel::on_btnLev7_clicked()
{
    Field* fld = new Field(40, 40);
    Game* game = new Level7(fld, TIMEFREE, {10, 100});
    game->initializeGame(1);
    GameWidget *gw = new GameWidget(game, 7);
    Dialog *dlg = new Dialog();
    connect(gw, &GameWidget::gameEnd, dlg, &Dialog::endGame);
    connect(dlg, &Dialog::accepted, gw, &GameWidget::close);
    connect(dlg, &Dialog::rejected, gw, &GameWidget::close);
    gw->setFocusPolicy(Qt::ClickFocus);
    gw->resize(1250+gw->border, 1250);
    gw->show();
}

void QLevel::on_btnLev8_clicked()
{

}

void QLevel::on_btnLev9_clicked()
{
    Field* fld = new Field(40, 40);
    Game* game = new Level9(fld, TIMEFREE, {20, 100});
    game->initializeGame(1);
    GameWidget *gw = new GameWidget(game, 9);
    Dialog *dlg = new Dialog();
    connect(gw, &GameWidget::gameEnd, dlg, &Dialog::endGame);
    connect(dlg, &Dialog::accepted, gw, &GameWidget::close);
    connect(dlg, &Dialog::rejected, gw, &GameWidget::close);
    gw->setFocusPolicy(Qt::ClickFocus);
    gw->resize(1250+gw->border, 1250);
    gw->show();
}

void QLevel::on_btnLev10_clicked()
{
    Field* fld = new Field(40, 40);
    Game* game = new Level10(fld, TIMEFREE, {1, 100});
    game->initializeGame(1);
    GameWidget *gw = new GameWidget(game, 10);
    Dialog *dlg = new Dialog();
    connect(gw, &GameWidget::gameEnd, dlg, &Dialog::endGame);
    connect(dlg, &Dialog::accepted, gw, &GameWidget::close);
    connect(dlg, &Dialog::rejected, gw, &GameWidget::close);
    gw->setFocusPolicy(Qt::ClickFocus);
    gw->resize(1250+gw->border, 1250);
    gw->show();
}

