#ifndef QLEVEL_H
#define QLEVEL_H

#include <QWidget>
#include <QPushButton>

namespace Ui {
class QLevel;
}

class QLevel : public QWidget
{
    Q_OBJECT

public:
    explicit QLevel(QWidget *parent = nullptr);
    //将背景设置为特定颜色
    void setBackground(QString s);
    //绘制关卡下面的星星
    void paintStars(int);
    //保存当前游戏进度
    void saveData();
    ~QLevel();
private:
    Ui::QLevel *ui;
    int difficulty = 1;
    //当前游戏进度
    std::vector<int> status;
    //当前关卡下星星数量
    std::vector<int> stars;
    std::vector<QPushButton*> buttons;
public slots:
    void on_btnReturn_clicked();
    void on_btnLev1_clicked();
    void on_btnLev2_clicked();
    void on_btnLev3_clicked();
    void on_btnLev4_clicked();
    void on_btnLev5_clicked();
    void on_btnLev6_clicked();
    void on_btnLev7_clicked();
    void on_btnLev8_clicked();
    void on_btnLev9_clicked();
    void on_btnLev10_clicked();
    void on_btnLev11_clicked();
    void on_btnLev12_clicked();
    void on_btnEasy_clicked();
    void on_btnHard_clicked();
    void on_btnNightmare_clicked();
    //游戏结束后对星星进行重绘
    void repaintStars(int, int, int);
};

#endif // QLEVEL_H
