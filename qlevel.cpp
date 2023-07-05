#include "qlevel.h"
#include "ui_qlevel.h"
#include "mainwindow.h"
#include "gamewidget.h"
#include "field.h"
#include "snake.h"
#include "game.h"
#include "dialog.h"

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

void QLevel::on_btnLev1_clicked()
{
    Field* fld = new Field(40, 40);
    Loc head = std::make_pair(20, 20);
    Snake* snk = new Snake(head, 5, 1, LEFT, fld->getMapPtr());
    fld->addSnake(snk);
    Game* game = new Game(fld, TIMEFREE, {10});
    game->initializeGame(1);
    GameWidget *gw = new GameWidget(game);
    Dialog *dlg = new Dialog();
    connect(gw, &GameWidget::gameover, dlg, &Dialog::show);
    gw->setFocusPolicy(Qt::ClickFocus);
    gw->resize(1250, 1350);
    gw->show();

    //game->runGame();

}



