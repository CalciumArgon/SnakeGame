#ifndef QLEVEL_H
#define QLEVEL_H

#include <QWidget>

namespace Ui {
class QLevel;
}

class QLevel : public QWidget
{
    Q_OBJECT

public:
    explicit QLevel(QWidget *parent = nullptr);
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
};

#endif // QLEVEL_H
