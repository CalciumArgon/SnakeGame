#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = nullptr);
    ~Dialog();

private:
    Ui::Dialog *ui;
signals:
    void gameClose();
public slots:
    void endGame(int, int, int);
    void on_btnOK_clicked();
};

#endif // DIALOG_H
