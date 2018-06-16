#include "dialog.h"
#include "ui_dialog.h"

#include <QMessageBox>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    //ui->lineEdit->setText("abc");
    ui->progressBar->setValue(0);
    isLogin = true;

    //2. 读入升级程序
    /////////////////////
    pUpdate = new UpdateByNetwork();
    pUpdate->setBaseAddress("http://localhost:8080/download/");
    pUpdate->setDownloadFileName("setup.exe");


    pUpdate->startDownload();
//    QMessageBox::information(this, "下载", "正在下载。");  //法1：可以挂起当前 dialog 线程，可以实现下载线程完成下载。
//    qDebug() <<"dialog中";
    while (!pUpdate->bDownloaded) {      //法2：可以等待下载线程完成下载。
        QCoreApplication::processEvents();
        //QThread::currentThread()->msleep(300);
        //QThread::currentThread()->yieldCurrentThread();    //不放弃当前线程，让它执行myFinished
     }

     if (pUpdate->bDownloaded)
         qDebug() << " 已下载";

    // 1.托盘图标
    // 1.1 初始化图标
    iconCorrect = QIcon(":/images/correct.png");
    iconError = QIcon(":/images/error.png");

    // 1.2 初始化托盘图标
    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setToolTip(tr("单击显示系统信息\n右击显示功能菜单"));
    setBothIcons(0);
    trayIcon->setVisible(true);

    // 显示托盘图标功能菜单
    setupTrayMenu();

    // 实现托盘图标左击功能
    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(myIconActivated(QSystemTrayIcon::ActivationReason)));
}

Dialog::~Dialog()
{
    delete ui;
    delete trayIcon;
    delete userMsg;
    delete deviceMsg;
    delete logoutAction;
    delete quitAction;
    delete trayIconMenu;
}

void Dialog::on_pushButton_clicked()
{
    // 1.判断用户/密码是否为空并获取相应的值
    if(ui->lineEdit->text().isEmpty() || ui->lineEdit_2->text().isEmpty())
    {
        QMessageBox::information(this, tr("登录"), tr("用户名与密码不能为空"));
        return;
    }

    ui->pushButton->setEnabled(false);
    // 2.使用Hessian登录
    // 3.其他内容
    isLogin = false;

    ui->pushButton->setEnabled(true);
}

void Dialog::on_pushButton_2_clicked()
{
    // 升级
}

void Dialog::setBothIcons(int index)
{
    // index = 0; 无错误
    // index = 1; 有错误,显示错误图标
    (index == 0) ? setWindowIcon(iconCorrect) : setWindowIcon(iconError);
    (index == 0) ? trayIcon->setIcon(iconCorrect) : trayIcon->setIcon(iconError);
}

void Dialog::closeEvent(QCloseEvent *event)
{
    if(trayIcon->isVisible())
    {
        if(isLogin)
            hide();
        else
        {
            QMessageBox::information(this, tr("远程传输与控制系统"), tr("请登录。\n\n退出程序：请右击系统托盘图标，选择退出程序"));
        }
        event->ignore();
    }
}

void Dialog::setupTrayMenu()
{
    // 要检索INI和设备信息后才处理
    userMsg = new QAction(tr("当前用户："), this);
    deviceMsg = new QAction(tr("当前设备："), this);
    logoutAction = new QAction(tr("注销用户"), this);
    quitAction = new QAction(tr("退出程序"), this);

    trayIconMenu = new QMenu(this);
    trayIconMenu->addAction(userMsg);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(deviceMsg);
    trayIconMenu->addSeparator();

    trayIconMenu->addAction(logoutAction);
    connect(logoutAction, SIGNAL(triggered()), this, SLOT(logout()));

    trayIconMenu->addAction(quitAction);
    connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));     // 非常重要,必须是qApp,不能是this

    trayIcon->setContextMenu(trayIconMenu);
}

void Dialog::logout()
{
    isLogin = false;
    showNormal();
}

void Dialog::myIconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch(reason){
    case QSystemTrayIcon::Trigger:
    case QSystemTrayIcon::DoubleClick:
        showMessage();
        break;
    default:
        break;
    }
}

void Dialog::showMessage()
{
    deviceInfo.clear();
    QString title = tr("系统状态信息");
    deviceInfo.append(tr("1. aaa"));
    deviceInfo.append(tr("2. bbb"));
    int duration = 3;
    trayIcon->showMessage(title, deviceInfo.join("\n"),     // bodyEdit->toPlainText()
                          QSystemTrayIcon::Information,     // icon
                          duration * 1000);
}
