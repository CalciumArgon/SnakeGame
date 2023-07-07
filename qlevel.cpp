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
    Game* game = new Game(fld, TIMEFREE, {10});
    game->initializeGame(1);
    GameWidget *gw = new GameWidget(game);
    Dialog *dlg = new Dialog();
    connect(gw, &GameWidget::gameover, dlg, &Dialog::show);
    connect(dlg, &Dialog::accepted, gw, &GameWidget::close);
    connect(dlg, &Dialog::rejected, gw, &GameWidget::close);
    gw->setFocusPolicy(Qt::ClickFocus);
    gw->resize(1250, 1350);
    gw->show();
}
void QLevel::on_btnLev2_clicked()
{
    Field* fld = new Field(40, 40);
    Loc loc1 = make_pair(10, 10);
    Loc loc2 = make_pair(10, 30);
    fld->createItem(MARSH, loc1, 1);
    fld->createItem(WALL, loc2, 1);
    Game* game = new Game(fld, TIMEFREE, {10});
    game->initializeGame(1);
    GameWidget *gw = new GameWidget(game);
    Dialog *dlg = new Dialog();
    connect(gw, &GameWidget::gameover, dlg, &Dialog::show);
    connect(dlg, &Dialog::accepted, gw, &GameWidget::close);
    connect(dlg, &Dialog::rejected, gw, &GameWidget::close);
    gw->setFocusPolicy(Qt::ClickFocus);
    gw->resize(1250, 1350);
    gw->show();
}

void QLevel::on_btnLev3_clicked()
{

}

void QLevel::on_btnLev4_clicked()
{

}

void QLevel::on_btnLev5_clicked()
{

}

void QLevel::on_btnLev6_clicked()
{

}

void QLevel::on_btnLev7_clicked()
{

}

void QLevel::on_btnLev8_clicked()
{

}



