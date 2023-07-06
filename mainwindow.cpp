#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qlevel.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

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
