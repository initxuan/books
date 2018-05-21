#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QSystemTrayIcon>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void setBothIcons(int);

private:
    Ui::Dialog *ui;

    bool isLogin;

    QIcon iconCorrect, iconError;
    QSystemTrayIcon *trayIcon;
};

#endif // DIALOG_H
