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
    int difficulty = 1;
    void setBackground(QString s);
    std::vector<int> status;
    std::vector<int> stars;
    void paintStars(int);
    std::vector<QPushButton*> buttons;
    void saveData();
    ~QLevel();
private:
    Ui::QLevel *ui;
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
    void repaintStars(int, int, int);
};

#endif // QLEVEL_H
