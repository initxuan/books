#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

// 系统托盘图标相关头文件
#include <QSystemTrayIcon>

// 关闭窗口相关的头文件
#include <QCloseEvent>

// 实现功能菜单所需的头文件
#include <QAction>
#include <QMenu>

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

    // 设置系统托盘图标和程序最小化图标
    void setBothIcons(int);

    // 劫持"关闭"事件
    void closeEvent(QCloseEvent *); // 框架默认自带函数

    // 配制功能菜单,鼠标右键功能
    void setupTrayMenu();

    // 鼠标左键功能
    void myIconActivated(QSystemTrayIcon::ActivationReason);

    void logout();

private:
    void showMessage();     // 显示消息

private:
    Ui::Dialog *ui;

    bool isLogin;           // 登录状态标志

    QStringList deviceInfo; // 用于显示系统消息

    // 系统托盘图标
    QIcon iconCorrect, iconError;
    QSystemTrayIcon *trayIcon;

    // 右击托盘显示的功能菜单
    QAction *userMsg, *deviceMsg, *logoutAction, *quitAction;
    QMenu *trayIconMenu;
};

#endif // DIALOG_H
