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
};

#endif // QLEVEL_H
