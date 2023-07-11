#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::endGame(int result, int, int)
{
    switch(result){
    case 0:
        ui->label->setText("游戏失败");
        break;
    case 1:
        ui->label->setText("游戏胜利");
        break;
    }
    this->show();
}
