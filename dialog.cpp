#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    this->setWindowTitle("贪吃蛇大冒险");
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::endGame(int result, int, int)
{
    switch(result){
    case 0:
        //ui->label->setText("游戏失败");
        this->setStyleSheet("#Dialog {border-image:url(:/defeat.png);} #Dialog * {border-image:url();}");
        break;
    case 1:
        //ui->label->setText("游戏胜利");
        this->setStyleSheet("#Dialog {border-image:url(:/victory.png);} #Dialog * {border-image:url();}");
        break;
    }
    this->show();
}

void Dialog::on_btnOK_clicked()
{
    emit(gameClose());
}
