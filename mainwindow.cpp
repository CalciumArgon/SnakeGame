#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qlevel.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setStyleSheet("#centralWidget{border-image: url(:/background2.png);}#centralWidget * {border-image: url();}");
    ui->btnStart->setStyleSheet("background-color:rgb(240, 240, 240, 200)");
    ui->btnQuit->setStyleSheet("background-color:rgb(240, 240, 240, 200)");
    this->setWindowTitle("贪吃蛇大冒险");
}

MainWindow::~MainWindow()
{
    delete ui;
}

#include "qlevel.h"
void MainWindow::on_btnStart_clicked()
{
    QLevel *qlv = new QLevel();
    qlv->show();
}
