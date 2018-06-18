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

// 引用"程序升级包网络下载"的头文件
#include "updatebynetwork.h"

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

    bool isLoginFunc();
    bool isThereNewUpdateFunc();

private slots:
    void on_pushButton_clicked();   // 登陆

    void on_pushButton_2_clicked(); // 升级程序
    void myDownloadProgress(qint64, qint64);    // 进度条

    // 设置系统托盘图标和程序最小化图标
    void setBothIcons(int);

    // 劫持"关闭"事件
    void closeEvent(QCloseEvent *); // 框架默认自带函数

    // 配制功能菜单,鼠标右键功能
    void setupTrayMenu();

    // 鼠标左键功能
    void myIconActivated(QSystemTrayIcon::ActivationReason);

    void logout();      // 注销

private:
    void showMessage();     // 显示消息

private:
    Ui::Dialog *ui;

    // 用户信息
    volatile bool isLogin;      // 登录状态标志，如用户登录，（且无更新升级程序）：不显示登录对话框
    QString curUserName;        // 当前用户，如果已登录则记录在INI中，下次无需再次登录
    double curVersion;          // 程序版本号
    QString curInstalledDir;    // 程序安装目录
    QString curUserZone;        // 用户所在地区

    //升级信息
    volatile bool isThereNewUpdate;             //是否有新版本升级程序
    QString baseUrl;                            //http://localhost:8080/download/
    QString exeFileName;                        //=xjx_jxs_setup.exe
    QString tmpExeFileName;
    QString versionFileName;                    //=update.txt
    QString updateDir;                          //=update       //升级程序被下载到当前目录\update中

    //UpdateByNetwork * pUpdateExeFile;
    //UpdateByNetworkThread * pUpdateThread;    //不再需要线程，直接控制UpdateByNetwork


    QStringList deviceInfo; // 用于显示设备信息

    // 托盘图标
    // 系统托盘图标
    QIcon iconCorrect, iconError;
    QSystemTrayIcon *trayIcon;

    // 右击托盘显示的功能菜单
    QAction *userMsg, *deviceMsg, *logoutAction, *quitAction;
    QMenu *trayIconMenu;

//    UpdateByNetwork *pUpdate;   // 处理升级包网络下载的handler
};

#endif // DIALOG_H
